/*
 * Utilities
*/

#ifndef UTILITIES
#define UTILITIES

#include <iostream>
#include <sstream>
#include <unistd.h>
#include <random>

#define VERBOSE false

using namespace std;

// Returns one sampled value from the exponential distribution with avg as average
double exp_dis(double avg);

// Prints the passed string and sleeps for a sampled millisecond value of time from the exponential distribution with non-zero avg_ms as average
// (If avg_ms is 0, there won't be sleep)
void log(string s, unsigned int avg_ms = 0);

#endif
