
#include <thread>
#include <chrono>
#include <csignal>
#include "cnt_condition_variable.hpp"
#include "utilities.hpp"
#include "santa_v3.hpp"

#define N_SANTA 5
#define N_REINDEER 9
#define N_ELVES 10

#define MIN_ELVES 3 // Number of elves in a consulting group

using namespace std;

// Threads

void reindeer(SantaClaus& sc, int id);
void elf(SantaClaus& sc, int id);
void santa(SantaClaus& sc, int id);

// Announces Christmas arrival for reindeer
void christmas_handler(int signum);

// Signal Christmas arrival with SIGUSR1 periodically
void christmas_signal();

// Reads line-command params
void read_params(int argc, char** argv, int& n_reindeer, int& n_elves, int& n_santa);

mutex mtx;
cnt_condition_variable wait_christmas;
bool is_christmas(false);

int main(int argc, char** argv)
{
    int n_reindeer, n_elves, n_santa;

    read_params(argc, argv, n_reindeer, n_elves, n_santa);

    // Create monitor
    SantaClaus sc(n_reindeer, n_elves, MIN_ELVES, n_santa);

    // Create main threads
    vector<thread> th_reindeer;
    vector<thread> th_elves;
    vector<thread> th_santa;
    
    for (int i = 0; i < n_santa; i++)
    {
        thread th(santa, ref(sc), i);
        th_santa.push_back(move(th));
    }
    for (int i = 0; i < n_elves; i++)
    {
        thread th(elf, ref(sc), i);
        th_elves.push_back(move(th));
    }
    for (int i = 0; i < n_reindeer; i++)
    {
        thread th(reindeer, ref(sc), i);
        th_reindeer.push_back(move(th));
    }

    // Set SIGUSR1 handler
    signal(SIGUSR1, christmas_handler);

    // Christmas signal thread
    thread th_signal(christmas_signal);
    
    // Wait for thread terminations
    th_signal.join();
    for (int i = 0; i < n_elves; i++)
        th_elves[i].join();
    for (int i = 0; i < n_reindeer; i++)
        th_reindeer[i].join();
    for (int i = 0; i < n_santa; i++)
        th_santa[i].join();

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
    unsigned int id_santa;

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
        sc.new_service(DELIVERY, id_santa);
        log("Reindeer " + to_string(id) + ": head back to the Pacific Islands (toys delivery with Santa " + to_string(id_santa) + " finished)", 100);
    }
}

void elf(SantaClaus& sc, int id)
{
    unsigned int id_santa;

    while (true)
    {
        log("Elf " + to_string(id) + ": making new toys", 400);
        log("Elf " + to_string(id) + ": ready to talk");
        sc.new_service(CONSULT, id_santa);
        log("Elf " + to_string(id) + ": go back to work (talking with Santa " + to_string(id_santa) + " finished)");
    }
}

void santa(SantaClaus& sc, int id)
{
    SERVICE s;

    while (true)
    {
        log("Santa " + to_string(id) + ": waiting for reindeer/elves");
        sc.start_service(s, id);
        if (s == DELIVERY)
            log("Santa " + to_string(id) + ": delivering toys", 100);
        else
            log("Santa " + to_string(id) + ": answer all questions in session", 10);
        sc.end_service(id);
        log("Santa " + to_string(id) + ": end of service");
    }
}

void read_params(int argc, char** argv, int& n_reindeer, int& n_elves, int& n_santa)
{
    if (argc != 3 + 1)
    {
        cerr << "Usage: " << *argv << " n_reindeer n_elves n_santa" << endl;
        exit(1);
    }

    n_reindeer = atoi(argv[1]);
    n_elves = atoi(argv[2]);
    n_santa = atoi(argv[3]);

    if (n_reindeer <= 0 || n_elves <= 0 || n_santa <= 0)
    {
        cerr << "Error: params must be positive integer" << endl;;
        exit(2);
    }
}
