#!/usr/bin/env python3

import argparse
import csv
import sys
import os

def condense_log_per_id(input_path, output_path=None):
    """
    Reads a CAN/Serial log file and removes messages that are identical
    (Type and Data) to the last printed message *with the same identifier*
    (CAN ID or "SERIAL"). Keeps the first occurrence's timestamp for each
    unique data payload per identifier.

    Args:
        input_path (str): Path to the input log file (CSV format).
        output_path (str, optional): Path to the output file.
                                     If None, prints to standard output.
    """
    # Dictionary to store the data tuple of the last printed message for each ID/Type
    # Key: Identifier (int CAN ID or string "SERIAL")
    # Value: Tuple of data fields (e.g., (DLC, D1, D2...) or (SerialDataLine,))
    last_printed_data_per_id = {}
    lines_processed = 0
    lines_written = 0
    duplicates_skipped = 0

    try:
        # Determine output target
        if output_path:
            outfile = open(output_path, 'w', newline='', encoding='utf-8')
            writer = csv.writer(outfile)
            output_target_name = output_path
        else:
            outfile = None
            writer = None
            output_target_name = "stdout"

        print(f"Processing log file: {input_path}")
        print(f"Output target: {output_target_name}")

        with open(input_path, 'r', newline='', encoding='utf-8', errors='replace') as infile:
            reader = csv.reader(infile)

            # --- Handle Header ---
            try:
                header = next(reader)
                lines_processed += 1
                if writer:
                    writer.writerow(header)
                else:
                    print(",".join(header))
                lines_written += 1
            except StopIteration:
                print("Warning: Input file is empty or has no header.")
                if outfile:
                    outfile.close()
                return

            # --- Process Data Rows ---
            for row in reader:
                lines_processed += 1
                if lines_processed % 50000 == 0: # Progress indicator
                    print(f".", end='', flush=True)

                if len(row) < 3:
                    # print(f"\nWarning: Skipping malformed line {lines_processed}: {row}")
                    continue # Skip lines that don't fit expected format

                current_type = row[0]
                identifier = None
                current_data_tuple = None

                try:
                    # --- Identify and Extract ---
                    if current_type == "CAN" and len(row) >= 3:
                        # CAN ID is usually hex in logs, convert to int for consistent key
                        identifier = int(row[2], 16) # Column 2 is CAN ID
                        # Data tuple includes DLC and all subsequent D1-D8 fields present
                        current_data_tuple = tuple(row[3:])
                    elif current_type == "SERIAL" and len(row) >= 3:
                        identifier = "SERIAL" # Use the type string as identifier
                        # Data tuple is the rest of the line's content
                        current_data_tuple = tuple(row[2:])
                    else:
                        # Unknown type or malformed CAN/SERIAL row
                        # print(f"\nWarning: Skipping unknown type or malformed line {lines_processed}: {row}")
                        continue # Skip this line

                    # --- Compare and Decide ---
                    last_payload = last_printed_data_per_id.get(identifier)

                    if last_payload is None or current_data_tuple != last_payload:
                        # Either first time seeing this ID/Type OR data has changed
                        if writer:
                            writer.writerow(row)
                        else:
                            print(",".join(row))
                        lines_written += 1
                        # Update the dictionary with the data of the message *we just printed*
                        last_printed_data_per_id[identifier] = current_data_tuple
                    else:
                        # Duplicate data for this specific ID/Type compared to the last printed one
                        duplicates_skipped += 1

                except (ValueError, IndexError) as e:
                     # print(f"\nWarning: Skipping line {lines_processed} due to parsing error ({e}): {row}")
                     continue # Skip this line on parsing errors

        print(f"\nProcessing complete.")
        print(f"Total lines processed    : {lines_processed}")
        print(f"Lines written (condensed): {lines_written}")
        print(f"Same-ID duplicates skipped: {duplicates_skipped}")

    except FileNotFoundError:
        print(f"Error: Input file not found: {input_path}")
    except Exception as e:
        print(f"An error occurred: {e}")
    finally:
        if outfile:
            outfile.close()
            if lines_written > 1 : # Only confirm saving if data was written
                print(f"Condensed log saved to: {output_path}")
            else:
                 print(f"Output file created, but no data written (Input likely empty or only header).")


def main():
    parser = argparse.ArgumentParser(description="Condense CAN/Serial log files by removing consecutive duplicate messages *for the same identifier*.")
    parser.add_argument("input_log", help="Path to the input log file (CSV format).")
    parser.add_argument("-o", "--output", help="Path to the output file (optional, prints to console if omitted).")

    args = parser.parse_args()

    condense_log_per_id(args.input_log, args.output)

if __name__ == "__main__":
    main()