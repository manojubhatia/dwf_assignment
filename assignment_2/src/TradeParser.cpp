#include "TradeParser.h"
#include "json.hpp" // External library for JSON parsing
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

/**
 * @brief Parses a JSON response to extract trade data.
 * 
 * This function parses a JSON response string containing multiple trades and converts
 * it into a vector of Trade structures. It checks for necessary fields and throws an
 * exception if required fields are missing or if the JSON parsing fails.
 * 
 * @param jsonResponse A JSON string containing the trade data.
 * @return A vector of Trade objects representing the parsed trades.
 * @throw std::runtime_error if there is an issue parsing the JSON or if the required fields are missing.
 */
std::vector<Trade> TradeParser::parseTrades(const std::string& jsonResponse) {
    std::vector<Trade> trades;
    
    try {
        auto jsonData = json::parse(jsonResponse);

        for (const auto& trade : jsonData) {
            // Check for necessary fields and handle missing fields
            if (trade.contains("a") && trade.contains("p") && trade.contains("q") &&
                trade.contains("f") && trade.contains("l") && trade.contains("T") && trade.contains("m")) {
                trades.push_back({
                    trade["a"].get<long long>(),          // Aggregate tradeId
                    trade["p"].get<std::string>(),        // Price
                    trade["q"].get<std::string>(),        // Quantity
                    trade["f"].get<int>(),                // First tradeId
                    trade["l"].get<int>(),                // Last tradeId
                    trade["T"].get<long long>(),          // Timestamp
                    trade["m"].get<bool>()                // Buyer is the maker?
                });
            } else {
                // If any required field is missing, throw an exception
                throw std::runtime_error("Malformed JSON: Missing required fields in the trade data.");
            }
        }
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw std::runtime_error("Error parsing trade data: " + std::string(e.what()));
    }
    return trades;
}
