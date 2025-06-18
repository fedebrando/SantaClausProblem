
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

void set_affinity_to_core0(pthread_t nh)
{
    cpu_set_t cpuset;

    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);

    if (pthread_setaffinity_np(nh, sizeof(cpu_set_t), &cpuset))
        std::cerr << "Error on affinity setting";
}
