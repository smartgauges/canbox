# Condensed CAN/Serial Log Tool (`condense_log.py`)

## 1. Overview

This Python script analyzes combined CAN bus and Serial communication log files (in a specific CSV format) and creates a condensed version. It intelligently removes redundant consecutive messages *for each unique identifier* (CAN ID or "SERIAL" type), keeping only the first occurrence or messages where the data payload changes compared to the last message seen *from that same identifier*.

This is particularly useful for reducing the size of large logs where status messages repeat frequently, making it easier to spot meaningful changes and analyze sequences of events.

## 2. Why Use This Tool?

*   **Reduce Log Size:** Significantly shrinks log files by removing repetitive status updates.
*   **Focus on Changes:** Makes it easier to identify when the data payload for a specific CAN ID or the Serial line actually changes.
*   **Improve Readability:** Creates cleaner logs for manual inspection or comparison (diffing).
*   **Faster Analysis:** Smaller logs can be processed more quickly by other tools or scripts.

## 3. How it Works (Filtering Logic)

The script reads the input CSV log line by line. It maintains a record (dictionary) of the *last data payload seen for each unique identifier*. An identifier is either:
    *   The **CAN Arbitration ID** (hex converted to int) for `CAN` type messages.
    *   The literal string `"SERIAL"` for `SERIAL` type messages.

For each new line read:
1.  It identifies the message `Type` and its `Identifier`.
2.  It extracts the relevant `Data` payload portion of the message.
3.  It compares the `current Data` to the `last seen Data` stored for that specific `Identifier`.
4.  **If** the `current Data` is **different** from the `last seen Data` for that `Identifier` (or if it's the first time seeing that `Identifier`), the current line (including its original timestamp) is written to the output. The `last seen Data` for that `Identifier` is then updated.
5.  **If** the `current Data` is **identical** to the `last seen Data` for that `Identifier`, the current line is **skipped** (not written to the output), effectively removing the consecutive duplicate.

**Example:**

| Input Line                     | Identifier | Data Payload | Last Seen (ID: 123) | Last Seen (ID: 456) | Action       | Output Line                    |
| :----------------------------- | :--------- | :----------- | :------------------ | :------------------ | :----------- | :----------------------------- |
| CAN,ts1,0x123,8,AAA...        | 123        | (8,AAA...)   | *None*              | *None*              | **Keep**     | CAN,ts1,0x123,8,AAA...        |
| CAN,ts2,0x456,4,BBB...        | 456        | (4,BBB...)   | (8,AAA...)          | *None*              | **Keep**     | CAN,ts2,0x456,4,BBB...        |
| CAN,ts3,0x123,8,AAA...        | 123        | (8,AAA...)   | (8,AAA...)          | (4,BBB...)          | **Skip**     | *(No output)*                  |
| CAN,ts4,0x123,8,CCC...        | 123        | (8,CCC...)   | (8,AAA...)          | (4,BBB...)          | **Keep**     | CAN,ts4,0x123,8,CCC...        |
| SERIAL,ts5,Some Text Data     | SERIAL     | (Some Text,) | (8,CCC...)          | (4,BBB...)          | **Keep**     | SERIAL,ts5,Some Text Data     |
| CAN,ts6,0x456,4,BBB...        | 456        | (4,BBB...)   | (8,CCC...)          | (4,BBB...)          | **Skip**     | *(No output)*                  |
| SERIAL,ts7,Some Text Data     | SERIAL     | (Some Text,) | (8,CCC...)          | (4,BBB...)          | **Skip**     | *(No output)*                  |
| SERIAL,ts8,Different Text Data| SERIAL     | (Different,)| (8,CCC...)          | (4,BBB...)          | **Keep**     | SERIAL,ts8,Different Text Data|

## 4. Features

*   Parses CSV logs with columns like `Type,Timestamp,Identifier/Data...`.
*   Handles both `CAN` and `SERIAL` message types.
*   Filters based on changes in data payload **per unique CAN ID** or **for the SERIAL type**.
*   Preserves the original timestamp of the *first* occurrence of a specific data payload for each identifier.
*   Retains the original CSV header row.
*   Outputs condensed log to file or standard output (console).
*   Prints summary statistics upon completion (lines processed, written, skipped).
*   Basic error handling for file access and malformed lines.

## 5. Requirements

*   **Python 3** (3.6 or later recommended)
*   No external libraries required beyond standard Python modules (`csv`, `argparse`, `sys`, `os`).

## 6. Installation

No installation is typically needed. Just ensure you have Python 3 installed. You might want to make the script executable:

```bash
chmod +x condense_log.py
```

## 7. Usage

```bash
python condense_log.py <input_log> [-o <output_file>]
# or if executable:
./condense_log.py <input_log> [-o <output_file>]
```

*   **`<input_log>`:** (Required) Path to the input CSV log file.
*   **`-o <output_file>` or `--output <output_file>`:** (Optional) Path to the output file for the condensed log. If omitted, the condensed log is printed to the standard output (console).
*   **`-h` or `--help`:** Show the help message.

**Examples:**

1.  **Condense `can_serial_log.csv` and print results to console:**
    ```bash
    python condense_log.py can_serial_log.csv
    ```

2.  **Condense `can_serial_log.csv` and save to `condensed_log.csv`:**
    ```bash
    python condense_log.py can_serial_log.csv -o condensed_log.csv
    ```

## 8. Input Log Format

The script expects a CSV file with a header row, where the relevant columns are typically:

*   **Column 0:** `Type` ("CAN" or "SERIAL")
*   **Column 1:** `Timestamp` (Numerical timestamp)
*   **Column 2:** `Identifier` (Hex CAN ID for "CAN" type, or the first data field for "SERIAL")
*   **Column 3 onwards:** Data payload fields (e.g., DLC, D1-D8 for CAN; text/hex data for SERIAL)

**Example Input (`input.csv`):**

```csv
Type,Timestamp,Identifier,DLC,D1,D2,D3,D4,D5,D6,D7,D8
CAN,1743874896.997287,161,8,00,00,A1,30,FF,FF,FF,FF
CAN,1743874896.997448,221,7,00,00,2E,01,A1,02,3A,
SERIAL,1743874896.998507,FD 0B 56 00 0C 0E 09 09 05 00 0F A1 FD 0B
CAN,1743874896.998392,261,7,22,08,36,00,3F,00,00,
SERIAL,1743874896.998712,FD 0B 55 74 65 73 74 00 00 00 00 20 FD 0B
CAN,1743874896.997448,221,7,00,00,2E,01,A1,02,3A, // Duplicate CAN 221
SERIAL,1743874896.999112,FD 0B 55 74 65 73 74 00 00 00 00 20 FD 0B // Duplicate SERIAL
SERIAL,1743874896.999137,FD 0B 56 00 0C 0E 09 09 05 00 08 9A FD 0B // New SERIAL Data
```

*(Note: The script automatically handles variations in the number of data columns after the Identifier)*

## 9. Output Format

The output file (or console output) will have the **same CSV format as the input**, including the header row, but with the consecutive duplicate rows (per identifier) removed.

**Example Output (for the input above):**

```csv
Type,Timestamp,Identifier,DLC,D1,D2,D3,D4,D5,D6,D7,D8
CAN,1743874896.997287,161,8,00,00,A1,30,FF,FF,FF,FF
CAN,1743874896.997448,221,7,00,00,2E,01,A1,02,3A,
SERIAL,1743874896.998507,FD 0B 56 00 0C 0E 09 09 05 00 0F A1 FD 0B
CAN,1743874896.998392,261,7,22,08,36,00,3F,00,00,
SERIAL,1743874896.998712,FD 0B 55 74 65 73 74 00 00 00 00 20 FD 0B
SERIAL,1743874896.999137,FD 0B 56 00 0C 0E 09 09 05 00 08 9A FD 0B
```
*(Note: Duplicate CAN 221 and SERIAL lines were removed)*

At the end of processing, summary statistics will be printed to the console (stderr).

## 10. Limitations and Notes

*   **Format Specific:** Strictly expects the described CSV input format. Will likely fail or produce incorrect results with other formats (like `candump -l` log format).
*   **Consecutive Duplicates Only:** Only removes duplicates that occur *immediately* after the previous identical message *from the same identifier*. Non-consecutive identical messages are kept.
*   **No Protocol Interpretation:** Does not understand the meaning of CAN IDs or serial data payloads. Filtering is purely based on identical raw data strings for the payload columns.
*   **Performance:** For extremely large log files (many millions of lines), performance might degrade due to the dictionary lookups, but it should be reasonably efficient for typical log sizes.
