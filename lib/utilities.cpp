
#include "utilities.hpp"

double exp_dis(double avg)
{
    static random_device rd;
    static mt19937 gen(rd());
    exponential_distribution<> ed(1.0/avg);

    return ed(gen);
}

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
