
#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <random>
#include <pthread.h>
#include <sched.h>

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

/**
 * @brief Constraints the execution of a thread on a single core
 * 
 * This procedure constraints the execution of the thread with the
 * received native handle on the core with index 0
 * 
 * @param nh The native handle of the thread to bind
 */
void set_affinity_to_core0(pthread_t nh);

#endif
