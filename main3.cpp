
#include <thread>
#include <chrono>
#include "cnt_condition_variable.hpp"
#include "utilities.hpp"
#include "santa_v3.hpp"

#define N_SANTA 5
#define N_REINDEER 20
#define N_ELVES 10

#define MIN_REINDEER 9
#define MIN_ELVES 3

using namespace std;

// Threads

void reindeer(SantaClaus& sc, int id);
void elf(SantaClaus& sc, int id);
void santa(SantaClaus& sc, int id);

// Reads line-command params
void read_params(int argc, char** argv, int& n_reindeer, int& n_elves, int& n_santa);

int main(int argc, char** argv)
{
    int n_reindeer, n_elves, n_santa;

    read_params(argc, argv, n_reindeer, n_elves, n_santa);

    // Create monitor
    SantaClaus sc(n_reindeer, MIN_REINDEER, n_elves, MIN_ELVES, n_santa);

    // Create threads
    vector<thread> th_reindeer;
    vector<thread> th_elves;
    vector<thread> th_santa;
    
    for(int i = 0; i < n_elves; i++)
    {
        thread th(elf, ref(sc), i);
        th_elves.push_back(move(th));
    }
    for(int i = 0; i < n_reindeer; i++)
    {
        thread th(reindeer, ref(sc), i);
        th_reindeer.push_back(move(th));
    }
    for(int i = 0; i < n_santa; i++)
    {
        thread th(santa, ref(sc), i);
        th_santa.push_back(move(th));
    }

    // Wait thread terminations
    for(int i = 0; i < n_elves; i++)
        th_elves[i].join();
    for(int i = 0; i < n_reindeer; i++)
        th_reindeer[i].join();
    for(int i = 0; i < n_santa; i++)
        th_santa[i].join();

    return 0;
}

void reindeer(SantaClaus& sc, int id)
{
    unsigned int id_santa;

    while (true)
    {
        log("Reindeer " + to_string(id) + ": ready to deliver");
        sc.new_service(DELIVERY, id_santa);
        log("Reindeer " + to_string(id) + ": delivery finished with Santa " + to_string(id_santa));
    }
}

void elf(SantaClaus& sc, int id)
{
    unsigned int id_santa;

    while (true)
    {
        log("Elf " + to_string(id) + ": ready to talk");
        sc.new_service(CONSULT, id_santa);
        log("Elf " + to_string(id) + ": conversation finished with Santa " + to_string(id_santa));
    }
}

void santa(SantaClaus& sc, int id)
{
    SERVICE s;

    while (true)
    {
        log("Santa " + to_string(id) + ": waiting for reindeer/elves");
        sc.start_service(s, id);
        if(s == DELIVERY)
            log("Santa " + to_string(id) + ": delivering toys", 100);
        else
            log("Santa " + to_string(id) + ": arguing with a bunch of stupid elves", 10);
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
