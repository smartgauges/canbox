# CAN Log Analyzer

This Python script (`can_analyzer.py`) analyzes CAN bus log files, extracts unique CAN messages, and optionally filters the output by CAN ID. It supports the specific CSV format, and provides a concise summary of the results.

## Features

*   **Parses CAN Log Format:**
    *   CSV format: `Time Stamp,ID,Extended,Dir,Bus,LEN,D1,D2,D3,D4,D5,D6,D7,D8`
*   **Identifies Unique Messages:**  Determines unique CAN messages based on the combination of CAN ID and data bytes.
*   **Counts Message Occurrences:**  Counts how many times each unique message appears in the log.
*   **Filters by CAN ID:**  Optionally filters the output to show only messages matching a specific CAN ID (provided in hexadecimal format).
*   **Outputs to File or Console:**  Writes the results to a specified output file or prints them to the console.
*   **Clear Summary:**  Provides a summary of:
    *   Total messages processed.
    *   Total unique messages found.
    *   Number of messages matching the filter (if a filter is used).
*   **Error Handling:**  Includes basic error handling for file operations.
*   **Command-Line Arguments:** Uses `argparse` for easy and flexible command-line usage.

## Requirements

*   Python 3 (3.6 or later recommended)

## Installation
No library needed

## Usage

```bash
./can_analyzer.py <log_file> [-o <output_file>] [-f <can_id>]
```

*   **`<log_file>`:**  (Required) Path to the CAN log file.
*   **`-o <output_file>` or `--output <output_file>`:** (Optional) Path to the output file.  If not provided, results are printed to the console.
*   **`-f <can_id>` or `--filter <can_id>`:** (Optional) CAN ID to filter for (in hexadecimal format, e.g., `0x123`). If specified, *only* messages with this ID will be processed and included in the output.
*   **`-h` or `--help`:** Show help message.

**Examples:**

1.  **Analyze all messages, print to console:**

    ```bash
    ./can_analyzer.py my_can_log.log
    ```

2.  **Analyze all messages, save to `unique_messages.txt`:**

    ```bash
    ./can_analyzer.py my_can_log.log -o unique_messages.txt
    ```

3.  **Analyze messages with ID `0x1B0`, save to `messages_1B0.txt`:**

    ```bash
    ./can_analyzer.py my_can_log_file.log -f 0x1B0 -o unique_1B0.txt
    ```
   or
    ```bash
    ./can_analyzer.py my_can_log_file.log --filter 0x1B0 --output messages_1B0.txt
    ```

4. **Show help message**
   ```
   ./can_analyzer.py -h
   ```

**Supported Log Formats:**

The script parses CAN log in CSV format:

```
Time Stamp,ID,Extended,Dir,Bus,LEN,D1,D2,D3,D4,D5,D6,D7,D8
39966311,00000167,false,Rx,0,8,09,06,FF,FF,00,00,00,00,
39989355,0000024C,false,Rx,0,5,00,00,00,00,00,00,00,00,
```

**Output Format:**

The output (either to the console or to a file) will be in the following format:

```
Total messages processed: <total_count>
Total unique messages found: <unique_count>
Messages matching filter (ID 0x<filter_id>): <filtered_count>  (Only if a filter is used)
Unique Messages:
ID: 0x<can_id>, Data: <data_bytes>, Count: <count>
ID: 0x<can_id>, Data: <data_bytes>, Count: <count>
...
```

*   **`Total messages processed`:** The total number of lines/messages read from the input log file.
*   **`Total unique messages found`:** The number of unique CAN ID and data combinations.
*   **`Messages matching filter`:**  The number of messages that matched the specified CAN ID filter (if a filter was used).
*   **`ID`:** The CAN ID in hexadecimal format (e.g., `0x123`).
*   **`Data`:** The CAN data bytes, represented as a space-separated hexadecimal string (e.g., `DE AD C0 DE`).
*   **`Count`:** The number of times this specific CAN ID and data combination appeared in the log.

**Example Output:**
```
Total messages processed: 53201
Total unique messages found: 120
Unique Messages:
ID: 0x167, Data: 09 06 FF FF 00 00 00 00, Count: 1330
ID: 0x1A8, Data: 00 FF FF 00 00 FF FF FF, Count: 933
ID: 0xB6, Data: FF FF FF FF 00 00 00 D0, Count: 3730
ID: 0x217, Data: F1 00 00 00 00 FF FF E0, Count: 1828
ID: 0x2A5, Data: 00 00 00 00 00 00 00 00, Count: 494
...
```

**Error Handling:**

*   If the input file cannot be found or opened, an error message will be printed.
*   If an invalid CAN ID filter is provided, an error message will be printed.
*   If a line in the log file cannot be parsed, script will continue to next line.
*   Script will check for correct data length in log file.

**Notes:**

*   This script is designed for analyzing *raw* CAN data. It does *not* interpret the meaning of the data bytes.
*   The script assumes that the CAN IDs and data are in hexadecimal format.

