#include "TextProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <regex>
#include <openssl/md5.h>

using namespace std;

/**
 * @brief Constructs a new TextProcessor object.
 */
TextProcessor::TextProcessor() {}

/**
 * @brief Helper function to handle data writing when downloading a file.
 * 
 * @param ptr Pointer to the data received.
 * @param size Size of each data chunk.
 * @param nmemb Number of chunks.
 * @param stream Output file stream.
 * @return The number of bytes written.
 */
size_t TextProcessor::write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

/**
 * @brief Checks if a directory exists.
 * 
 * @param dir The path of the directory to check.
 * @return True if the directory exists, false otherwise.
 */
bool TextProcessor::directory_exists(const std::string& dir) {
    struct stat info;
    if (stat(dir.c_str(), &info) != 0) {
        return false;
    }
    return (info.st_mode & S_IFDIR) != 0;
}

/**
 * @brief Creates a directory with the given path.
 * 
 * @param dir The path of the directory to be created.
 */
void TextProcessor::create_directory(const std::string& dir) {
    if (mkdir(dir.c_str(), 0755) != 0) {
        cerr << "Error: Could not create directory: " << dir << endl;
    }
}

/**
 * @brief Downloads a book from the specified URL and saves it to the output path.
 * 
 * @param url The URL to download the book from.
 * @param output_path The file path where the downloaded book will be saved.
 */
void TextProcessor::download_book(const std::string& url, const std::string& output_path) {
    string dir = "data";
    if (!directory_exists(dir)) {
        create_directory(dir);
    }

    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(output_path.c_str(), "wb");
        if (!fp) {
            cerr << "Error: Could not open file for writing: " << output_path << endl;
            return;
        }
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "Failed to download book: " << curl_easy_strerror(res) << endl;
        }
        curl_easy_cleanup(curl);
        fclose(fp);
    }
}

/**
 * @brief Cleans the input text by replacing non-alphanumeric characters with spaces.
 * 
 * @param text The input text to clean.
 * @return A cleaned version of the text.
 */
string TextProcessor::clean_text(const string& text) {
    regex non_alpha_numeric("[^a-zA-Z0-9]+");
    return regex_replace(text, non_alpha_numeric, " ");
}

/**
 * @brief Extracts words from the file, cleans them, and adds them to the hash table.
 * 
 * @param file_path The path to the file from which to extract words.
 * @param hash_table The hash table to store the extracted words.
 */
void TextProcessor::extract_words(const string& file_path, HashTable& hash_table) {
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file: " << file_path << endl;
        return;
    }

    string word;
    int word_count = 0;

    while (file >> word) {
        word_count++;
        string cleaned_word = clean_text(word);
        stringstream ss(cleaned_word);
        string processed_word;
        while (ss >> processed_word) {
            transform(processed_word.begin(), processed_word.end(), processed_word.begin(), ::tolower);
            if (processed_word.empty()) continue;
            try {
                int count = hash_table.get(processed_word);
                hash_table.insert(processed_word, count + 1);
            } catch (const invalid_argument&) {
                hash_table.insert(processed_word, 1);
            }
        }
    }
    file.close();
    cout << "Finished processing " << word_count << " words." << endl;
}

/**
 * @brief Computes the MD5 checksum of a file.
 * 
 * @param filename The name of the file for which the MD5 checksum is to be computed.
 * @return A string representing the MD5 checksum of the file.
 * @throw runtime_error if the file cannot be opened.
 */
string TextProcessor::compute_md5(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        throw runtime_error("Could not open file to compute checksum");
    }

    MD5_CTX md5_ctx;
    MD5_Init(&md5_ctx);

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        MD5_Update(&md5_ctx, buffer, file.gcount());
    }

    unsigned char result[MD5_DIGEST_LENGTH];
    MD5_Final(result, &md5_ctx);

    ostringstream md5_string;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        md5_string << hex << static_cast<int>(result[i]);
    }

    return md5_string.str();
}

/**
 * @brief Processes the file by comparing its checksum with an existing one, then either loads a previously
 * saved hash table or processes the file and rebuilds the hash table.
 * 
 * This function computes the MD5 checksum of the specified text file, compares it with an existing checksum
 * stored in the given hash file. If the checksums match and a saved hash table is available, it loads the
 * hash table from a file. Otherwise, it processes the file to extract words and build the hash table, and 
 * saves both the checksum and the hash table for future runs.
 * 
 * @param hash_table The hash table to be populated or loaded.
 * @param file_path The path to the text file being processed.
 * @param hash_file The path to the file that stores the checksum of the previously processed file.
 */
void TextProcessor::process_file(HashTable& hash_table, const std::string& file_path, const std::string& hash_file) {
    std::string checksum = compute_md5(file_path);
    std::ifstream existing_checksum(hash_file);
    std::string saved_checksum;
    if (existing_checksum.is_open()) {
        existing_checksum >> saved_checksum;
        existing_checksum.close();
    }
    if (saved_checksum == checksum && hash_table.load_from_file("hash_table.dat")) {
        std::cout << "Checksum matches, loaded hash table from file." << std::endl;
    } else {
        std::cout << "Checksum mismatch or no previous data, processing file and building hash table." << std::endl;
        // Extract words from the book and populate the hash table
        extract_words(file_path, hash_table);
        // Save the checksum and hash table for future runs
        std::ofstream checksum_file(hash_file);
        checksum_file << checksum;
        checksum_file.close();
        hash_table.save_to_file("hash_table.dat");
    }
}