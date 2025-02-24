
#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <random>

#define VERBOSE true

using namespace std;

/**
 * @brief Samples an exponential distribution with received average
 * 
 * @param avg The average of the exponential distribution
 * @return A value sampled from the exponential distribution
 */
double exp_dis(double avg);

/**
 * @brief Prints a log and sleeps for some time
 * 
 * This function print the received log string and waits for a time sampled
 * from an exponential distribution with received positive average (milliseconds)
 * 
 * @param s The log string to print
 * @param avg_ms The positive average of the time exponential distribution (in milliseconds)
 * 
 * @warning If the received average is 0, this function prints a log but doesn't sleep
 */
void log(string s, unsigned int avg_ms = 0);

#endif
