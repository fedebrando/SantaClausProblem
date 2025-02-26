
#include "statistics.hpp"

void _start_stop()
{
    static bool first_time = true;
    static chrono::_V2::system_clock::time_point start;
    chrono::_V2::system_clock::time_point stop;
    chrono::duration<double, milli> elapsed;
    string to_print;

    if (first_time)
    {
        start = chrono::high_resolution_clock::now();
        first_time = false;
    }
    else
    {
        stop = chrono::high_resolution_clock::now();
        elapsed = stop - start;
        cout << dot2comma(to_string(elapsed.count())) << endl;

        // reset value
        first_time = true;
    }
}

void _start_stop_multi(ACTION act, unsigned int n_start_min, unsigned int n_stop_min, bool comma)
{
    static mutex mtx;
    unique_lock<mutex> lock(mtx);
    
    static unsigned int n_start = 0;
    static unsigned int n_stop = 0;
    static chrono::_V2::system_clock::time_point start;
    chrono::_V2::system_clock::time_point stop;
    chrono::duration<double, milli> elapsed;
    string to_print;

    if (act == START)
    {
        n_start++;
        if (n_start == n_start_min)
            start = chrono::high_resolution_clock::now();
    }
    else
    {
        n_stop++;
        if (n_stop == n_stop_min)
        {
            stop = chrono::high_resolution_clock::now();
            elapsed = stop - start;
            if (comma)
                cout << dot2comma(to_string(elapsed.count())) << endl;
            else
                cout << to_string(elapsed.count()) << endl;

            // reset values
            n_start = 0;
            n_stop = 0;
        }
    }
}

string dot2comma(string s)
{
    replace(s.begin(), s.end(), '.', ',');
    return s;
}
