
#include <thread>
#include <chrono>
#include <csignal>
#include "cnt_condition_variable.hpp"
#include "utilities.hpp"
#include "statistics.hpp"
#include "santa_v2.hpp"

//#define MONO_CORE // Run the application on a single core

#define N_REINDEER 9
#define N_ELVES 10
#define MIN_ELVES 3 // Number of elves in a consulting group

//#define V2_DELIVERY_TIME_VERBOSE

using namespace std;

// Announces Christmas arrival for reindeer
void christmas_handler(int signum);

// Signal Christmas arrival with SIGUSR1 periodically
void christmas_signal();

// Main threads

void reindeer(SantaClaus& sc, int id);
void elf(SantaClaus& sc, int id);
void santa(SantaClaus& sc);

mutex mtx;
cnt_condition_variable wait_christmas;
bool is_christmas(false);

int main(void)
{
#ifdef MONO_CORE
    set_affinity_to_core0(pthread_self());
#endif

    // Create monitor
    SantaClaus sc(N_REINDEER, N_ELVES, MIN_ELVES);

    // Create main threads
    vector<thread> th_reindeer;
    vector<thread> th_elves;
    thread th_santa(santa, ref(sc));
#ifdef MONO_CORE
    set_affinity_to_core0(th_santa.native_handle());
#endif
    
    for (int i = 0; i < N_ELVES; i++)
    {
        thread th(elf, ref(sc), i);
#ifdef MONO_CORE
        set_affinity_to_core0(th.native_handle());
#endif
        th_elves.push_back(move(th));
    }
    for (int i = 0; i < N_REINDEER; i++)
    {
        thread th(reindeer, ref(sc), i);
#ifdef MONO_CORE
        set_affinity_to_core0(th.native_handle());
#endif
        th_reindeer.push_back(move(th));
    }

    // Set SIGUSR1 handler
    signal(SIGUSR1, christmas_handler);

    // Christmas signal thread
    thread th_signal(christmas_signal);
#ifdef MONO_CORE
    set_affinity_to_core0(th_signal.native_handle());
#endif
    
    // Wait for thread terminations
    th_signal.join();
    for (int i = 0; i < N_ELVES; i++)
        th_elves[i].join();
    for (int i = 0; i < N_REINDEER; i++)
        th_reindeer[i].join();
    th_santa.join();

    return 0;
}

void christmas_handler(int signum) 
{
    unique_lock<mutex> lock(mtx);

    is_christmas = true;
    wait_christmas.notify_one();
}

void christmas_signal()
{
    chrono::_V2::high_resolution_clock::time_point t0(chrono::high_resolution_clock::now());
    chrono::milliseconds dt(10000);
    chrono::_V2::high_resolution_clock::time_point t_stop(t0);

    while (true)
    {
        t_stop += dt;
        this_thread::sleep_until(t_stop); // to avoid error accumulation

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
        sc.new_service(DELIVERY);
#ifdef V2_DELIVERY_TIME_VERBOSE
        _ex_start_stop_multi(START, 1, N_REINDEER + 1);
#endif
        log("Reindeer " + to_string(id) + ": delivering toys", 100);
#ifdef V2_DELIVERY_TIME_VERBOSE
        _ex_start_stop_multi(STOP, 1, N_REINDEER + 1);
#endif
        sc.consumed_service(DELIVERY);
        log("Reindeer " + to_string(id) + ": head back to the Pacific Islands", 100);
    }
}

void elf(SantaClaus& sc, int id)
{
    while (true)
    {
        log("Elf " + to_string(id) + ": making new toys", 400);
        log("Elf " + to_string(id) + ": ready to talk");
        sc.new_service(CONSULT);
        log("Elf " + to_string(id) + ": talking with Santa", 10);
        sc.consumed_service(CONSULT);
    }
}

void santa(SantaClaus& sc)
{
    SERVICE s;

    while (true)
    {
        log("Santa: waiting for reindeer/elves");
        sc.start_service(s);
        if (s == DELIVERY)
        {
#ifdef V2_DELIVERY_TIME_VERBOSE
            _ex_start_stop_multi(START, 1, N_REINDEER + 1);
#endif
            log("Santa: delivering toys", 100);
#ifdef V2_DELIVERY_TIME_VERBOSE
            _ex_start_stop_multi(STOP, 1, N_REINDEER + 1);
#endif
        }
        else // s == CONSULT
            log("Santa: answer all questions in session", 10);
        sc.end_service(s);
        log("Santa: end of service");
    }
}
