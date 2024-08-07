#include <thread>
#include <chrono>
#include <csignal>
#include "cnt_condition_variable.h"
#include "utilities.h"
#include "statistics.h"
#include "santa_v2.h"

#define N_REINDEER 9
#define N_ELVES 10
#define MIN_ELVES 3

//#define STATS_VERBOSE_V2

using namespace std;

mutex mtx;
cnt_condition_variable wait_christmas;
bool is_christmas(false);

void christmas_handler(int signum) 
{
    unique_lock<mutex> lock(mtx);
    is_christmas = true;
    wait_christmas.notify_one();
}

void christmas_signal()
{
    chrono::_V2::system_clock::time_point t0(chrono::high_resolution_clock::now());
    chrono::milliseconds dt(10000);
    chrono::_V2::system_clock::time_point t_stop(t0);

    while (true)
    {
        t_stop += dt;
        this_thread::sleep_until(t_stop);

        raise(SIGUSR1);
    }
}

void reindeer(SantaClaus& sc, int id)
{
    while (true)
    {
        log("Reindeer " + to_string(id) + ": on vacation");
        {
            unique_lock<mutex> lock(mtx);
            while (!is_christmas)
                wait_christmas.wait(lock);

            if (!wait_christmas.any())
                is_christmas = false;    
            else
                wait_christmas.notify_one();
        }
        log("Reindeer " + to_string(id) + ": head back to the North Pole", 100);
        log("Reindeer " + to_string(id) + ": ready to deliver");
        sc.new_delivery();
#ifdef STATS_VERBOSE_V2
        _start_stop_v2(START, 1, N_REINDEER + 1);
#endif
        log("Reindeer " + to_string(id) + ": delivering toys", 100);
#ifdef STATS_VERBOSE_V2
        _start_stop_v2(STOP, 1, N_REINDEER + 1);
#endif
        sc.end_delivery();
        log("Reindeer " + to_string(id) + ": head back to the Pacific Islands", 100);
    }
}

void elf(SantaClaus& sc, int id)
{
    while (true)
    {
        log("Elf " + to_string(id) + ": making new toys", 400);
        log("Elf " + to_string(id) + ": ready to talk");
        sc.new_consult();
    }
}

void santa(SantaClaus& sc)
{
    SERVICE s;

    while (true)
    {
        log("Santa: waiting for reindeer/elves");
        sc.start_service(s);
        if(s == DELIVERY)
        {
#ifdef STATS_VERBOSE_V2
            _start_stop_v2(START, 1, N_REINDEER + 1);
#endif
            log("Santa: delivering toys", 100);
#ifdef STATS_VERBOSE_V2
            _start_stop_v2(STOP, 1, N_REINDEER + 1);
#endif
        }
        else
            log("Santa: arguing with a bunch of stupid elves", 10);
        sc.end_service(s);
        log("Santa: end of service");
    }
}

int main(void)
{
    SantaClaus sc(N_REINDEER, N_ELVES, MIN_ELVES);
    vector<thread> th_reindeer;
    vector<thread> th_elves;
    thread th_santa(santa, ref(sc));
    
    for(int i = 0; i < N_ELVES; i++)
    {
        thread th(elf, ref(sc), i);
        th_elves.push_back(move(th));
    }
    for(int i = 0; i < N_REINDEER; i++)
    {
        thread th(reindeer, ref(sc), i);
        th_reindeer.push_back(move(th));
    }

    signal(SIGUSR1, christmas_handler);
    thread th_signal(christmas_signal);
    
    th_signal.join();
    for(int i = 0; i < N_ELVES; i++)
        th_elves[i].join();
    for(int i = 0; i < N_REINDEER; i++)
        th_reindeer[i].join();
    th_santa.join();

    return 0;
}