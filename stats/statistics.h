/*
 * statistics :)
*/

#ifndef STATISTICS
#define STATISTICS

#include <iostream>
#include <chrono>
#include <algorithm>
#include <mutex>

using namespace std;

enum ACTION
{
    START,
    STOP
};

// Measures and prints the time between two consecutive calls
void _start_stop_v1();

// Measures and prints the time between the n_start_min-th time with act as START and the n_stop_min-th time with act as STOP
void _start_stop_v2(ACTION act, unsigned int n_start_min, unsigned int n_stop_min);

// Returns the string s with '.' substituted by ',' 
string dot2comma(string s);

#endif