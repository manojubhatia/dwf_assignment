# Hash Table and Binance API Assignments

This repository contains two main assignments developed as part of the assignments:
1. **Hash Table Implementation with Linear Probing and Performance Analysis**
2. **Binance USD(S)-M Futures API Connectivity and Trade Parsing**

---

## 1. Hash Table Implementation with Linear Probing

### Description
The first assignment involves creating a fixed-sized hash table using **linear probing** to resolve collisions. The goal is to efficiently store and retrieve key-value pairs (in this case, words and their occurrences) using constant-time operations (O(1)) whenever possible.

### Key Features:
- **Fixed-sized Hash Table**: Handles collisions using linear probing.
- **Dynamic Resizing**: Hash table expands dynamically by increments of 2000 when it becomes full.
- **Basic Operations**: Insert, delete, and retrieve operations (`insert`, `remove`, `get`).
- **Performance Timer**: Measures the time taken for specific hash table operations to validate O(1) performance.
- **File Persistence**: Save and load the hash table from a file, along with an MD5 checksum to ensure data consistency across runs.
- **Error Handling**: Handles hash table overflow, key not found, and file errors.

### Files:
- `src/HashTable.cpp`: Implementation of the hash table with linear probing, dynamic resizing, and file I/O.
- `src/PerformanceTimer.cpp`: Measures the time taken for operations (in milliseconds).
- `src/TextProcessor.cpp`: Handles file download, word extraction, and MD5 checksum computation.
- `src/main.cpp`: The main entry point for downloading the book, populating the hash table, and measuring performance.

### Key Optimizations:
- **Checksum Matching**: The hash table is loaded from a saved file if the MD5 checksum of the input file matches, avoiding redundant processing.
- **Collision-Free Probing**: Track and query entries in the hash table that were inserted without any collisions to observe near-constant-time lookups.
- **Efficient Word Extraction**: A cleaning function removes non-alphanumeric characters and extracts words from the text corpus.

### How to Run:
1. Install the necessary libraries using the following commands (I have debian based Linux systems, haven't tested it on Windows)

sudo apt update
sudo apt install libssl-dev libcurl4-openssl-dev

2. Compile the program using the provided `Makefile`.
3. Run the program to download "A Tale of Two Cities" from Project Gutenberg, extract words, and populate the hash table.
4. Performance results for word retrievals are displayed, with time taken for each operation.

---

## 2. Binance USD(S)-M Futures API Connectivity and Trade Parsing

### Description
The second assignment focuses on connecting to the **Binance USD(S)-M Futures API** and retrieving aggregate trade data. The trades are parsed and printed in a structured JSON format. The goal is to ensure proper API connectivity, error handling, and efficient trade parsing with performance measurement.

### Key Features:
- **API Connectivity**: Uses `libcurl` to connect to the Binance API.
- **Trade Parsing**: Parses the JSON response for aggregate trade data using the **nlohmann/json** library.
- **Performance Timer**: Measures the speed at which trade data is parsed.
- **Error Handling**: Handles network issues, malformed JSON, and HTTP error codes.
- **Trade Structure**: Parses each trade with fields like `price`, `quantity`, `timestamp`, and `isBuyerMaker` status.

### Files:
- `src/BinanceAPI.cpp`: Handles the API connection and GET requests using `libcurl`.
- `src/TradeParser.cpp`: Parses the JSON response into structured trade data.
- `src/PerformanceTimer.cpp`: Measures the time taken for parsing trades.
- `src/main.cpp`: The main entry point for querying Binance futures trades and measuring performance.

### Error Handling:
- **APIException**: Custom exception class that handles network and API errors.
- **Malformed JSON**: The parser checks for missing fields and malformed JSON, throwing exceptions if the response is incomplete.
- **Network Issues**: Catches network failures and provides meaningful error messages for debugging.

### How to Run:
1. Compile the program using the provided `Makefile`.
2. Run the program to connect to the Binance Futures API, retrieve trade data for `BTCUSDT`, and print it in a structured format.
3. Observe the performance results for trade parsing operations.

---

## Build and Run Instructions

### Requirements:
- **Compiler**: GCC or any C++ compiler supporting C++11.
- **Libraries**: 
  - **libcurl** for API connectivity
  - **openssl** for MD5 checksum computation
  - **nlohmann/json** for JSON parsing in the Binance API project.

### Compilation:
Both projects come with a `Makefile` for easy compilation. Ensure that the required libraries are installed.

```bash
# Compile the first project (Hash Table and Text Processing)
make all

# Run the program
./bin/hash_table_program

# Compile the second project (Binance API)
make all

# Run the program
./bin/binance_api_test

# Algorithmic complexity of parsing
That's O(n+m) where:
1. n is the size of the json string returned on the API call.
2. m is the number of trades in the returned json
Since these two operations are sequential, the O(n+m)


### Further enhancements possible:

1. Turn the programs into libraries, to be used with different application programs.
2. Take command line arguments to get data on different text for the first assignment, or traded item for the second.
3. I don't mention that we could make a micro-service out of this, since we're talking of O(1) complexity, so time is of essence.
4. Some test-scoped functions could be added, to check how much time is lost in hashing and what exactly is the retrieval time.
5. The results show retrieval to the order of fraction of a microsecond.
