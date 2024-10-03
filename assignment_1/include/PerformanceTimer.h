#ifndef PERFORMANCE_TIMER_H
#define PERFORMANCE_TIMER_H

#include <chrono>

/**
 * @class PerformanceTimer
 * @brief A class to measure the time taken by various operations.
 * 
 * This class uses the high-resolution clock to provide precise measurements of
 * elapsed time between the `start` and `stop` calls.
 */
class PerformanceTimer {
public:
    /**
     * @brief Starts the performance timer.
     * 
     * This method records the current time as the start time for timing.
     */
    void start();

    /**
     * @brief Stops the performance timer and calculates the elapsed time.
     * 
     * @return The elapsed time in milliseconds since the last call to `start()`.
     */
    double stop();  // Returns time in milliseconds

private:
    /**
     * @brief Stores the time point when the timer was started.
     */
    std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
};

#endif
