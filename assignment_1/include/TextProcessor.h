#ifndef TEXTPROCESSOR_H
#define TEXTPROCESSOR_H

#include <string>
#include "HashTable.h"

/**
 * @class TextProcessor
 * @brief A class that processes text files, including downloading, cleaning, and extracting words.
 * 
 * This class provides various methods for downloading text files, extracting words,
 * calculating checksums (MD5), and interacting with a hash table for word storage.
 */
class TextProcessor {
public:
    /**
     * @brief Constructs a new TextProcessor object.
     * 
     * Initializes a new instance of the TextProcessor class.
     */
    TextProcessor();
    
    /**
     * @brief Downloads a book from a given URL and saves it to a specified file path.
     * 
     * @param url The URL to download the book from.
     * @param output_path The file path where the downloaded book will be saved.
     */
    void download_book(const std::string& url, const std::string& output_path);

    /**
     * @brief Extracts words from a file and inserts them into the provided hash table.
     * 
     * @param file_path The path to the file from which to extract words.
     * @param hash_table The hash table to store the extracted words.
     */
    void extract_words(const std::string& file_path, HashTable& hash_table);

    /**
     * @brief Computes the MD5 checksum of a given file.
     * 
     * @param filename The name of the file for which the MD5 checksum is to be computed.
     * @return A string representing the MD5 checksum of the file.
     */
    std::string compute_md5(const std::string& filename);

    /**
     * @brief Processes the file by comparing its checksum with an existing one, then either loads a previously
     * saved hash table or processes the file and rebuilds the hash table.
     * 
     * @param hash_table The hash table to be populated or loaded.
     * @param file_path The path to the text file being processed.
     * @param hash_file The path to the file that stores the checksum of the previously processed file.
     */
    void process_file(HashTable& hash_table, const std::string& file_path, const std::string& hash_file);

    /**
     * @brief Checks if a directory exists.
     * 
     * @param dir The path of the directory to check.
     * @return True if the directory exists, false otherwise.
     */
    bool directory_exists(const std::string& dir);

    /**
     * @brief Creates a directory with the given path.
     * 
     * @param dir The path of the directory to be created.
     */
    void create_directory(const std::string& dir);

    /**
     * @brief Cleans the given text by removing non-alphanumeric characters.
     * 
     * @param text The input text to clean.
     * @return A cleaned version of the text.
     */
    std::string clean_text(const std::string& text);
    
private:
    /**
     * @brief Callback function to handle data writing during download.
     * 
     * @param ptr Pointer to the data received.
     * @param size Size of each data chunk.
     * @param nmemb Number of chunks.
     * @param stream Output file stream.
     * @return The number of bytes written.
     */
    static size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream);
};

#endif // TEXTPROCESSOR_H
