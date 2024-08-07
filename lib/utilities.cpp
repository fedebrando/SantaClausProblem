#include "utilities.h"

// Returns one sampled value from the exponential distribution with avg as average
double exp_dis(double avg)
{
    static random_device rd;
    static mt19937 gen(rd());
    exponential_distribution<> ed(1.0/avg);

    return ed(gen);
}

// Prints the passed string and sleeps for a sampled millisecond value of time from the exponential distribution with non-zero avg_ms as average
// If avg_ms is 0, there won't be sleep
void log(string s, unsigned int avg_ms)
{
    ostringstream oss;

    if (VERBOSE)
    {
        oss << s << endl;
        cout << oss.str();
    }
    if (avg_ms)
        usleep(exp_dis(avg_ms)*1000);
}