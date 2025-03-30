import argparse
import os
import re
import csv

def analyze_can_log(log_file, output_file=None, id_filter=None):
    """
    Analyzes a CAN log file in the specified CSV format, extracts unique
    messages, and optionally filters by CAN ID.

    Args:
        log_file (str): Path to the CAN log file.
        output_file (str, optional): Path to the output file. If None, prints to stdout.
        id_filter (str, optional):  CAN ID to filter for (in hex, e.g., "0x123").
                                     If None, all IDs are processed.

    Returns:
        None.  Prints or writes the unique messages.
    """

    unique_messages = {}  # Dictionary to store unique messages: { (id, data): count }
    total_messages = 0
    unique_count = 0
    filtered_count = 0  # Count of messages matching the ID filter (if any)

    try:
        with open(log_file, 'r', newline='') as f:  # Use newline='' for correct CSV handling
            reader = csv.reader(f)
            header = next(reader, None)  # Read the header row (skip it)

            if header is None:
                print("Error: Empty input file.")
                return

            for row in reader:
                total_messages += 1
                # --- Parse the CSV Row ---
                try:
                    # Extract data based on the expected CSV format
                    timestamp_str = row[0].strip()
                    can_id_str = row[1].strip()
                    extended_str = row[2].strip()
                    direction = row[3].strip()  # Rx or Tx
                    bus_str = row[4].strip()     # bus id
                    data_length_str = row[5].strip() # data lenght

                    # Convert to appropriate types
                    timestamp = int(timestamp_str)  # Timestamp as integer
                    can_id = int(can_id_str, 16)  # CAN ID as integer (from hex)
                    extended = extended_str.lower() == 'true'  # Convert to boolean
                    data_length = int(data_length_str)

                    # Extract data bytes (D1-D8).  Handle missing bytes gracefully.
                    data_bytes = []

                    for i in range(6, min(len(row), 14)): # 6 is first data column, maximum to D8 in the log format
                        byte_str = row[i].strip()
                        if byte_str:  # Check if the string is not empty
                            try:
                                byte_val = int(byte_str, 16)
                                data_bytes.append(byte_val)
                            except ValueError:
                                #print(f"Skipping invalid byte value: {byte_str}")
                                pass; # in a case data is not in hex.

                    data_bytes = bytes(data_bytes)  # Convert to bytes object.
                    # if len(data_bytes) != data_length:
                    #    #print(f"Skipping invalid line, lenght mismatch data:{len(data_bytes)} expected:{data_length}  : {row}")
                    #    continue;

                except (IndexError, ValueError) as e:
                    #print(f"Skipping invalid line {row} - {e}")
                    continue #skip the current line.


                # --- Filtering ---
                if id_filter is not None:
                    if can_id != id_filter:
                        continue  # Skip this message
                    filtered_count += 1

                # --- Uniqueness Check ---
                message_key = (can_id, data_bytes)  # Tuple of (ID, data) is the unique key
                if message_key not in unique_messages:
                    unique_messages[message_key] = 0
                    unique_count += 1
                unique_messages[message_key] += 1

    except FileNotFoundError:
        print(f"Error: File not found: {log_file}")
        return
    except IOError as e:
        print(f"Error reading file: {e}")
        return


    # --- Output Results ---
    output_lines = []  # Store lines for output

    output_lines.append(f"Total messages processed: {total_messages}")
    output_lines.append(f"Total unique messages found: {unique_count}")
    if id_filter is not None:
        output_lines.append(f"Messages matching filter (ID 0x{id_filter:X}): {filtered_count}")
    output_lines.append("Unique Messages:")

    for (can_id, data), count in unique_messages.items():
        data_str = ' '.join([f"{byte:02X}" for byte in data])  # Format as hex with spaces
        output_lines.append(f"ID: 0x{can_id:X}, Data: {data_str}, Count: {count}")

    if output_file:
        try:
            with open(output_file, 'w') as outfile:
                for line in output_lines:
                    outfile.write(line + "\n")
            print(f"Unique messages written to {output_file}")
        except IOError as e:
            print(f"Error writing to output file: {e}")
    else:
        for line in output_lines:
            print(line)



def main():
    parser = argparse.ArgumentParser(description="Analyze CAN log files for unique messages.")
    parser.add_argument("log_file", help="Path to the CAN log file.")
    parser.add_argument("-o", "--output", help="Path to the output file (optional).")
    parser.add_argument("-f", "--filter", help="CAN ID to filter for (in hex, e.g., 0x123).")

    args = parser.parse_args()

    # Convert filter to integer if provided:
    id_filter = None
    if args.filter:
        try:
            id_filter = int(args.filter, 16)  # Convert hex string to integer
        except ValueError:
            print("Error: Invalid CAN ID filter.  Must be a hexadecimal value (e.g., 0x123).")
            return

    analyze_can_log(args.log_file, args.output, id_filter)


if __name__ == "__main__":
    main()