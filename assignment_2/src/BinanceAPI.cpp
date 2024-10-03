#include "BinanceAPI.h"
#include <iostream>
#include <curl/curl.h>

/**
 * @brief Callback function to handle cURL data.
 * 
 * This function is used by cURL to append data received from the server to the response string.
 * 
 * @param contents Pointer to the data received.
 * @param size Size of each data element.
 * @param nmemb Number of elements.
 * @param out Pointer to the string where the data will be appended.
 * @return The total size of the data handled.
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* out) {
    size_t totalSize = size * nmemb;
    out->append((char*)contents, totalSize);
    return totalSize;
}

/**
 * @brief Constructs a new BinanceAPI object with the specified base URL.
 * 
 * @param baseURL The base URL of the Binance API.
 */
BinanceAPI::BinanceAPI(const std::string& baseURL) : baseURL(baseURL) {}

/**
 * @brief Sends a GET request to the Binance API and retrieves the response.
 * 
 * This function sends a GET request to the specified API endpoint and handles various error
 * scenarios, including network issues and invalid HTTP response codes.
 * 
 * @param endpoint The API endpoint to send the GET request to.
 * @return A string containing the response from the API.
 * @throw APIException if there is a network error or if the API returns an error code.
 */
std::string BinanceAPI::sendGETRequest(const std::string& endpoint) {
    std::string url = baseURL + endpoint;
    CURL* curl;
    CURLcode res;
    std::string response;
    long http_code = 0;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Enable verbose output for debugging 
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        res = curl_easy_perform(curl);

        // Get HTTP response code
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (res != CURLE_OK) {
            std::string errorMessage = "Network error: " + std::string(curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            throw APIException(errorMessage);
        }

        // Check for HTTP error codes or no response (HTTP code 0)
        if (http_code == 0) {
            curl_easy_cleanup(curl);
            throw APIException("No response from server (HTTP code 0). Possible network or DNS issue.");
        } else if (http_code >= 200 && http_code < 300) {
            // Request was successful
            curl_easy_cleanup(curl);
            return response;
        } else {
            // Handle any other non-successful HTTP status codes
            curl_easy_cleanup(curl);
            throw APIException("API error: HTTP code " + std::to_string(http_code));
        }

        curl_easy_cleanup(curl);
    } else {
        throw APIException("Failed to initialize cURL.");
    }

    return response;
}

/**
 * @brief Retrieves aggregate trade data from the Binance API.
 * 
 * This function requests aggregate trade data for the specified trading pair symbol and limit from
 * the Binance USD(S)-M Futures API.
 * 
 * @param symbol The trading pair symbol (e.g., "BTCUSDT").
 * @param limit The number of trades to retrieve (default is 5).
 * @return A JSON string containing the aggregate trade data.
 * @throw APIException if there is a network error or if the API returns an error code.
 */
std::string BinanceAPI::getAggregateTrades(const std::string& symbol, int limit) {
    std::string endpoint = "/fapi/v1/aggTrades?symbol=" + symbol + "&limit=" + std::to_string(limit);
    return sendGETRequest(endpoint);
}
