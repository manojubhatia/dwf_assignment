#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

/**
 * @class HashTable
 * @brief A class to represent a hash table with linear probing for collision resolution.
 * 
 * This class provides functionality for basic hash table operations like insertion,
 * deletion, and retrieval of key-value pairs. It also supports saving and loading
 * the hash table to/from a file and resizing when the table becomes full.
 */
class HashTable {
public:
    /**
     * @brief Constructs a new HashTable object.
     * 
     * @param size The initial size of the hash table.
     */
    HashTable(int size);

    /**
     * @brief Inserts a key-value pair into the hash table.
     * 
     * If the key already exists, its value is updated. If the table is full, it resizes.
     * 
     * @param key The key to be inserted.
     * @param value The value associated with the key.
     */
    void insert(const string& key, int value);

    /**
     * @brief Removes a key-value pair from the hash table.
     * 
     * @param key The key to be removed.
     * @throw invalid_argument if the key is not found.
     */
    void remove(const string& key);

    /**
     * @brief Retrieves the value associated with a key.
     * 
     * @param key The key to search for.
     * @return The value associated with the key.
     * @throw invalid_argument if the key is not found.
     */
    int get(const string& key) const;

    /**
     * @brief Returns the last inserted key-value pair.
     * 
     * @return A pair containing the last inserted key and its value.
     * @throw runtime_error if the hash table is empty.
     */
    pair<string, int> get_last() const;

    /**
     * @brief Returns the first inserted key-value pair.
     * 
     * @return A pair containing the first inserted key and its value.
     * @throw runtime_error if the hash table is empty.
     */
    pair<string, int> get_first() const;

    /**
     * @brief Returns statistics about the hash table.
     * 
     * @return A pair containing the number of occupied slots and the total size of the table.
     */
    pair<int, int> get_stats() const;

    /**
     * @brief Saves the current hash table to a file.
     * 
     * @param filename The name of the file to save the table.
     * @throw runtime_error if the file cannot be opened.
     */
    void save_to_file(const string& filename) const;

    /**
     * @brief Loads the hash table from a file.
     * 
     * @param filename The name of the file to load the table from.
     * @return True if the hash table was loaded successfully, otherwise false.
     */
    bool load_from_file(const string& filename);

private:
    /**
     * @brief Resizes the hash table when it becomes full.
     * 
     * The table size is increased by a fixed amount and all existing keys are rehashed.
     */
    void resize();

    /**
     * @brief Computes the hash value for a given key.
     * 
     * @param key The key to hash.
     * @param table_size The size of the table.
     * @return The hash value (index) for the key.
     */
    int hash(const string& key, int table_size) const;

    /**
     * @brief Resolves hash collisions using linear probing.
     * 
     * @param index The starting index of the probe.
     * @param table_size The size of the table.
     * @param table_occupied The vector indicating whether a slot is occupied.
     * @return The next available index for insertion.
     */
    int linear_probe(int index, int table_size, const vector<char>& table_occupied) const;

    /** 
     * @brief The hash table storing key-value pairs.
     */
    vector<pair<string, int>> table;

    /** 
     * @brief Vector indicating which slots are occupied.
     */
    vector<char> occupied;

    /** 
     * @brief The index of the first inserted element.
     */
    int first_index;

    /** 
     * @brief The index of the last inserted element.
     */
    int last_index;

    /** 
     * @brief The current size of the hash table.
     */
    int size;

    /** 
     * @brief The current number of elements in the hash table.
     */
    int elements_count;
};

#endif
