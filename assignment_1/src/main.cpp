#include "HashTable.h"
#include "TextProcessor.h"
#include "PerformanceTimer.h"
#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Main function to process text from a book and populate a hash table.
 * 
 * This function downloads a book, extracts words, and populates a hash table. It also 
 * measures performance for specific operations, such as retrieving the first and last 
 * inserted words, as well as word counts for selected words.
 * 
 * @return int Returns 0 on successful execution.
 */
int main() {
    // URL and output path for the book to be downloaded
    string url = "https://www.gutenberg.org/files/98/98-0.txt";
    string output_path = "data/gutenberg_98-0.txt";
    std::string checksum_file = "data/gutenberg_98-0_checksum.txt";

    // Create a TextProcessor instance
    TextProcessor text_processor;

    cout << "Downloading book" << endl;
    // Download the book
    text_processor.download_book(url, output_path);

    // Create a hash table with an initial size of 5000
    HashTable hash_table(5000);

    // Process the file by comparing the checksum and either loading or building the hash table
    text_processor.process_file(hash_table, output_path, checksum_file);
    cout << "Done processing book" << endl;

    // Display some data and use PerformanceTimer to measure performance
    try {
        // Get hash table statistics
        auto stats = hash_table.get_stats();
        cout << "Detected word count: " << stats.first << ", hash_table size: " << stats.second << endl;

        PerformanceTimer timer;

        // Time the retrieval of the first inserted word
        timer.start();
        auto first = hash_table.get_first();
        double timeTaken = timer.stop();
        cout << "First inserted: " << first.first << " -> " << first.second << " in " << timeTaken << " ms" << endl;

        // Time the retrieval of the last inserted word
        timer.start();
        auto last = hash_table.get_last();
        timeTaken = timer.stop();
        cout << "Last inserted: " << last.first << " -> " << last.second << " in " << timeTaken << " ms" << endl;

        // Time the search for specific words
        std::vector<std::string> words = {"london", "manette", "dover"};

        // Loop to handle repeated queries for the words in the list
        for (const auto& word : words) {
            timer.start();
            int count = hash_table.get(word);
            double timeTaken = timer.stop();
            std::cout << "Count of '" << word << "': " << count << " in " << timeTaken << " ms" << std::endl;
        }

    } catch (const exception& e) {
        cerr << e.what() << endl;
    }

    return 0;
}
