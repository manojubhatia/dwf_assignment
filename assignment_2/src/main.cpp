#include <iostream>
#include "BinanceAPI.h"
#include "TradeParser.h"
#include "PerformanceTimer.h"

/**
 * @brief Main function to interact with the Binance API, retrieve trades, and measure the parsing performance.
 * 
 * This function initializes the BinanceAPI, retrieves a stream of trades for the "BTCUSDT" symbol, parses
 * the trades using the TradeParser class, and prints the parsed trades. It also measures the time taken 
 * to parse the trades using the PerformanceTimer class.
 * 
 * @return int Returns 0 on successful execution, or prints an error message if there is an API or runtime error.
 */
int main() {
    try {
        // Initialize the Binance API with base URL
        BinanceAPI binance("https://fapi.binance.com");

        // Get a stream of trades
        std::string jsonResponse = binance.getAggregateTrades("BTCUSDT", 10);

        // Parse the trades
        TradeParser parser;
        PerformanceTimer timer;
        timer.start();
        std::vector<Trade> trades = parser.parseTrades(jsonResponse);
        double timeTaken = timer.stop();

        // Print the parsed trades
        for (const auto& trade : trades) {
            std::cout << "{\n"
                    << "  \"a\": " << trade.aggregateTradeId << ",\n"
                    << "  \"p\": \"" << trade.price << "\",\n"
                    << "  \"q\": \"" << trade.quantity << "\",\n"
                    << "  \"f\": " << trade.firstTradeId << ",\n"
                    << "  \"l\": " << trade.lastTradeId << ",\n"
                    << "  \"T\": " << trade.timestamp << ",\n"
                    << "  \"m\": " << (trade.isBuyerMaker ? "true" : "false") << "\n"
                    << "}\n";
        }

        // Print the time taken to parse the trades
        std::cout << "Time taken to parse trades: " << timeTaken << " ms" << std::endl;
    } catch (const BinanceAPI::APIException& e) {
        std::cerr << "API Error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
