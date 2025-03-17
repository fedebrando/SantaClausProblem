
#ifndef STATISTICS
#define STATISTICS

#include <iostream>
#include <chrono>
#include <algorithm>
#include <mutex>

using namespace std;

/**
 * Represents modes for calling `_start_stop_multi` function
 */
enum ACTION
{
    /**
     * Chronometer start mode
     */
    START,

    /**
     * Chronometer stop mode
     */
    STOP
};

/**
 * @brief Measures and prints the time between two consecutive calls
 */
void _start_stop();

/**
 * @brief Measures and prints the time between two instants in a concurrent enviroment (unprotected)
 * 
 * This function measures time between two instants, starting from
 * `n_start_min`-th start call and stop at the `n_stop_min`-th stop call
 * 
 * @param act Call mode
 * @param n_start_min Number of start calls to start chronometer
 * @param n_stop_min Number of stop calls to stop chronometer
 * @param comma `true` to print values with comma instead dot, `false` otherwise (default: `false`)
 * 
 * @warning This function must be called only under mutually exlusive block
 */
void _start_stop_multi(ACTION act, unsigned int n_start_min, unsigned int n_stop_min, bool comma = true);

/**
 * @brief Measures and prints the time between two instants in a concurrent enviroment (protected)
 * 
 * This function measures time between two instants, starting from
 * `n_start_min`-th start call and stop at the `n_stop_min`-th stop call
 * 
 * @param act Call mode
 * @param n_start_min Number of start calls to start chronometer
 * @param n_stop_min Number of stop calls to stop chronometer
 * @param comma `true` to print values with comma instead dot, `false` otherwise (default: `false`)
 */
void _ex_start_stop_multi(ACTION act, unsigned int n_start_min, unsigned int n_stop_min, bool comma = true);

/**
 * @brief Replaces dots with commas
 * 
 * @param s The string to modify
 * @return The string with dots replaced by commas
 */
string dot2comma(string s);

#endif