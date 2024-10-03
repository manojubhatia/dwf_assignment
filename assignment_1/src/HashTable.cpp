#include "HashTable.h"
#include <stdexcept>
#include <iostream>

/**
 * @brief Constructs a new HashTable object with a specified size.
 * 
 * @param size The initial size of the hash table.
 */
HashTable::HashTable(int size)
    : table(size, {"", 0}), occupied(size, false), first_index(-1), last_index(-1), size(size), elements_count(0) {}

/**
 * @brief Computes the hash value for a given key.
 * 
 * @param key The key to be hashed.
 * @param table_size The size of the hash table.
 * @return The hash value (index) for the given key.
 */
int HashTable::hash(const std::string& key, int table_size) const {
    std::hash<std::string> hash_fn;
    return hash_fn(key) % table_size;
}

/**
 * @brief Linear probing to resolve collisions.
 * 
 * @param index The current index.
 * @param table_size The size of the hash table.
 * @param table_occupied A vector representing occupied slots in the hash table.
 * @return The next available index.
 * @throw overflow_error if the table is full and no available slot is found.
 */
int HashTable::linear_probe(int index, int table_size, const std::vector<char>& table_occupied) const {
    int original_index = index;
    while (table_occupied[index]) {
        index = (index + 1) % table_size;
        if (index == original_index) {
            throw std::overflow_error("HashTable is full during probing");
        }
    }
    return index;
}

/**
 * @brief Inserts a key-value pair into the hash table.
 * 
 * @param key The key to be inserted.
 * @param value The value associated with the key.
 * @throw overflow_error if the table is full.
 */
void HashTable::insert(const std::string& key, int value) {
    // Resize the table if it is full
    if (elements_count == size) {
        resize();
    }

    int index = hash(key, size);

    // Linear probing if collision occurs
    for (int i = 0; i < size; ++i) {
        if (occupied[index]) {
            if (table[index].first == key) {
                // Update existing entry
                table[index].second = value;
                return;
            }
        } else {
            // Insert new entry
            table[index] = {key, value};
            occupied[index] = true;
            elements_count++;
            if (first_index == -1) {
                first_index = index;
            }
            last_index = index;
            return;
        }
        index = (index + 1) % size;
    }
    throw std::overflow_error("HashTable is full");
}

/**
 * @brief Removes a key-value pair from the hash table.
 * 
 * @param key The key to be removed.
 * @throw invalid_argument if the key is not found.
 */
void HashTable::remove(const std::string& key) {
    int index = hash(key, size);
    
    for (int i = 0; i < size; ++i) {
        if (occupied[index] && table[index].first == key) {
            occupied[index] = false;
            table[index] = {"", 0};
            elements_count--;
            if (index == first_index) first_index = -1;
            if (index == last_index) last_index = -1;
            return;
        }
        index = (index + 1) % size;
    }
    throw std::invalid_argument("Key not found");
}

/**
 * @brief Retrieves the value associated with a key.
 * 
 * @param key The key to search for.
 * @return The value associated with the key.
 * @throw invalid_argument if the key is not found.
 */
int HashTable::get(const std::string& key) const {
    int index = hash(key, size);

    for (int i = 0; i < size; ++i) {
        if (occupied[index] && table[index].first == key) {
            return table[index].second;
        }
        index = (index + 1) % size;
    }
    throw std::invalid_argument("Key not found");
}

/**
 * @brief Gets statistics about the hash table.
 * 
 * @return A pair containing the number of occupied slots and the size of the table.
 */
pair<int, int> HashTable::get_stats() const {
    int count = 0;
    for (bool slot : occupied) {
        if (slot) {count++;}
    }
    return {count, size};
}

/**
 * @brief Retrieves the last inserted key-value pair.
 * 
 * @return A pair containing the last inserted key and its value.
 * @throw runtime_error if the hash table is empty.
 */
std::pair<std::string, int> HashTable::get_last() const {
    if (last_index == -1) throw std::runtime_error("HashTable is empty");
    return table[last_index];
}

/**
 * @brief Retrieves the first inserted key-value pair.
 * 
 * @return A pair containing the first inserted key and its value.
 * @throw runtime_error if the hash table is empty.
 */
std::pair<std::string, int> HashTable::get_first() const {
    if (first_index == -1) throw std::runtime_error("HashTable is empty");
    return table[first_index];
}

/**
 * @brief Resizes the hash table when it becomes full.
 * 
 * The table size is increased, and all existing keys are rehashed.
 */
void HashTable::resize() {
    int new_size = size + 2000;  // Increase size by 2000
    std::vector<std::pair<std::string, int>> new_table(new_size, {"", 0});
    std::vector<char> new_occupied(new_size, false);

    int new_first_index = -1;
    int new_last_index = -1;

    // Rehash all existing keys into the new table
    for (int i = 0; i < size; ++i) {
        if (occupied[i]) {
            int new_index = hash(table[i].first, new_size);
            new_index = linear_probe(new_index, new_size, new_occupied);
            new_table[new_index] = table[i];
            new_occupied[new_index] = 1;

            // Update new_first_index and new_last_index accordingly
            if (i == first_index) {
                new_first_index = new_index;  // Track the new index for the first inserted element
            }
            new_last_index = new_index;  // Always update for the last inserted element
        }
    }

    // Replace old table with new table
    table = std::move(new_table);
    occupied = std::move(new_occupied);
    size = new_size;

    // Update the first and last index in the resized table
    first_index = new_first_index;
    last_index = new_last_index;

    std::cout << "HashTable resized to " << size << " slots." << std::endl;
}

/**
 * @brief Saves the hash table to a file.
 * 
 * @param filename The name of the file where the hash table will be saved.
 * @throw runtime_error if the file cannot be opened.
 */
void HashTable::save_to_file(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file to save hash table");
    }

    std::cout << "Saving hash table to file..." << std::endl;

    // Save the size and the number of elements
    file.write(reinterpret_cast<const char*>(&size), sizeof(size));
    file.write(reinterpret_cast<const char*>(&elements_count), sizeof(elements_count));
    file.write(reinterpret_cast<const char*>(&first_index), sizeof(first_index));  // Save first_index
    file.write(reinterpret_cast<const char*>(&last_index), sizeof(last_index));    // Save last_index

    // Save the table and the occupied array
    for (int i = 0; i < size; ++i) {
        int key_size = table[i].first.size();
        file.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));  // Write key size
        file.write(table[i].first.c_str(), key_size);  // Write the key
        file.write(reinterpret_cast<const char*>(&table[i].second), sizeof(int));  // Write the value
        // Write occupancy status as a char (1 for true, 0 for false)
        char is_occupied = occupied[i] ? 1 : 0;
        file.write(&is_occupied, sizeof(char));  // Write occupied status    
    }

    std::cout << "Hash table saved with " << elements_count << " elements." << std::endl;
    file.close();
}

/**
 * @brief Loads the hash table from a file.
 * 
 * @param filename The name of the file to load the hash table from.
 * @return True if the hash table was loaded successfully, otherwise false.
 */
bool HashTable::load_from_file(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;  // File doesn't exist
    }

    std::cout << "Loading hash table from file..." << std::endl;

    // Load the size and the number of elements
    file.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!file) { std::cerr << "Error reading size from file." << std::endl; return false; }

    file.read(reinterpret_cast<char*>(&elements_count), sizeof(elements_count));
    if (!file) { std::cerr << "Error reading elements_count from file." << std::endl; return false; }

    file.read(reinterpret_cast<char*>(&first_index), sizeof(first_index));
    if (!file) { std::cerr << "Error reading first_index from file." << std::endl; return false; }

    file.read(reinterpret_cast<char*>(&last_index), sizeof(last_index));
    if (!file) { std::cerr << "Error reading last_index from file." << std::endl; return false; }

    // Resize the table and occupied array
    table.resize(size);
    occupied.resize(size);
    std::cout << "Hash table size is: " << size << std::endl;

    // Load the table and the occupied array
    for (int i = 0; i < size; ++i) {
        int key_size;
        file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        if (!file) { std::cerr << "Error reading key_size for index " << i << std::endl; break; }

        if (key_size < 0 || key_size > 1000) {  // Assuming a max reasonable key size of 1000 characters
            std::cerr << "Invalid key_size at index " << i << ": " << key_size << std::endl;
            break;
        }

        std::string key(key_size, '\0');
        file.read(&key[0], key_size);
        if (!file) { std::cerr << "Error reading key for index " << i << std::endl; break; }

        int value;
        file.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!file) { std::cerr << "Error reading value for index " << i << std::endl; break; }

        char is_occupied;  // Change to char for better portability
        file.read(&is_occupied, sizeof(is_occupied));
        if (!file) { std::cerr << "Error reading occupied status for index " << i << std::endl; break; }

        occupied[i] = (is_occupied == 1);
        table[i] = {key, value};

        if (i % 1000 == 0) {
            std::cout << "Processed " << i << " entries." << std::endl;
        }
    }

    std::cout << "Hash table loaded with " << elements_count << " elements." << std::endl;

    file.close();
    return true;
}
