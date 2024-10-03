#ifndef TRADEPARSER_H
#define TRADEPARSER_H

#include <string>
#include <vector>

/**
 * @struct Trade
 * @brief A structure to represent an individual trade from the Binance API.
 * 
 * This structure holds the details of a trade, including trade IDs, price, quantity, timestamp,
 * and buyer/seller information.
 */
struct Trade {
    long long aggregateTradeId;  ///< The aggregate trade ID.
    std::string price;           ///< The price of the trade.
    std::string quantity;        ///< The quantity traded.
    int firstTradeId;            ///< The first trade ID.
    int lastTradeId;             ///< The last trade ID.
    long long timestamp;         ///< The timestamp of the trade.
    bool isBuyerMaker;           ///< True if the buyer is the maker, false otherwise.
};

/**
 * @class TradeParser
 * @brief A class to parse trade data from the Binance API.
 * 
 * This class provides a method to parse a JSON response containing trade data into a list of Trade structures.
 */
class TradeParser {
public:
    /**
     * @brief Parses a JSON response to extract trade data.
     * 
     * @param jsonResponse A JSON string containing the trade data.
     * @return A vector of Trade objects representing the parsed trades.
     * @throw std::runtime_error if the JSON parsing fails.
     */
    std::vector<Trade> parseTrades(const std::string& jsonResponse);
};

#endif
