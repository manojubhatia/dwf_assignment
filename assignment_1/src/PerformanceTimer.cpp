#include "PerformanceTimer.h"

/**
 * @brief Starts the performance timer.
 * 
 * Records the current time as the start time for measuring the duration.
 */
void PerformanceTimer::start() {
    startTime = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Stops the performance timer and calculates the elapsed time.
 * 
 * @return The elapsed time in milliseconds since the last call to `start()`.
 */
double PerformanceTimer::stop() {
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    return duration.count();
}
