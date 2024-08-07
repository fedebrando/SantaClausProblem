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

void _start_stop_v1();
void _start_stop_v2(ACTION act, unsigned int n_start_min, unsigned int n_stop_min);

string dot2comma(string s);

#endif