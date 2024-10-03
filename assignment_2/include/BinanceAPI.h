#ifndef BINANCE_API_H
#define BINANCE_API_H

#include <string>
#include <stdexcept>

/**
 * @class BinanceAPI
 * @brief A class that provides methods to interact with the Binance USD(S)-M Futures API.
 * 
 * This class allows you to retrieve aggregate trades data from the Binance API. It includes
 * functionality for sending GET requests and handling errors.
 */
class BinanceAPI {
public:
    /**
     * @brief Constructs a new BinanceAPI object.
     * 
     * @param baseURL The base URL for the Binance API.
     */
    BinanceAPI(const std::string& baseURL);

    /**
     * @brief Retrieves aggregate trades data from Binance for a given symbol.
     * 
     * @param symbol The trading pair symbol (e.g., "BTCUSDT").
     * @param limit The number of trades to retrieve (default is 5).
     * @return A JSON string containing aggregate trades data.
     * @throw APIException if there is an error while retrieving the data.
     */
    std::string getAggregateTrades(const std::string& symbol, int limit = 5);

    /**
     * @class APIException
     * @brief A custom exception class for handling Binance API errors.
     * 
     * This exception is thrown when there is an error with the API request.
     */
    class APIException : public std::runtime_error {
    public:
        /**
         * @brief Constructs a new APIException object with an error message.
         * 
         * @param message The error message to be associated with the exception.
         */
        explicit APIException(const std::string& message) : std::runtime_error(message) {}
    };

private:
    /**
     * @brief Sends a GET request to the Binance API and returns the response.
     * 
     * @param endpoint The API endpoint to send the request to.
     * @return The response from the API as a string.
     * @throw APIException if the request fails or the API returns an error.
     */
    std::string sendGETRequest(const std::string& endpoint);

    std::string baseURL; ///< The base URL for the Binance API
};

#endif
