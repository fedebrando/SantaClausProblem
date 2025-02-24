/*
 * Santa Claus Problem v3
*/

#ifndef SANTA_V3
#define SANTA_V3

#include "cnt_condition_variable.hpp"
#include <mutex>
#include <vector>

#define WAIT_VERBOSE

#ifdef WAIT_VERBOSE
#include <iostream>
#include "statistics.hpp"
#endif

#define NONE (-1)

enum SERVICE
{
    DELIVERY,
    CONSULT,
    _TOT_SERVICES
};

/**
 * @brief Monitor for Santa Claus Problem (version 3)
 * 
 * This is a generalization of version 1, with arbitrary
 * number of Santa Claus threads
 */
class SantaClaus
{
    private:
        mutex mtx;

        vector<unsigned int> TOT;
        unsigned int MIN_ELVES;

        int id_santa_selected;
        unsigned int n_santa;
        vector<cnt_condition_variable> await_someone;
        vector<cnt_condition_variable> wait_all_passed;
        vector<cnt_condition_variable> wait_greetings;

        vector<cnt_condition_variable> wait_service;
        vector<unsigned int> turnstile;
        vector<cnt_condition_variable> wait_end_of_service;
        vector<bool> end_of_service;
        
    public:
        /**
         * @brief Instantiates a new SCP monitor for the classical case
         * 
         * Instantiates a new SCP monitor for 9 reindeer, 10 elves,
         * with 3 elves in a consulting group and 1 Santa Claus
         */
        SantaClaus();

        /**
         * @brief Instantiates a new SCP monitor with specified numbers of reindeer, elves,
         * elves in a consulting group and Santa Claus entities
         * 
         * @param n_reindeer Number of reindeer
         * @param n_elves Number of elves
         * @param min_elves Number of elves in a consulting group
         * @param n_santa Number of Santa Claus entities
         */
        SantaClaus(unsigned int n_reindeer, unsigned int n_elves, unsigned int min_elves, unsigned int n_santa);

        ~SantaClaus();

        /**
         * @brief A rein or an elf require service to a Santa Claus as a RPC
         * 
         * @param s The required service
         * @param id_santa Reference to get the index of the Santa Claus server
         */
        void new_service(SERVICE s, unsigned int& id_santa);

        /**
         * @brief Santa Claus makes himself available to provide a service
         * @param s Reference to obtain the required type of service
         * @param id The Santa Claus identifier
         */
        void start_service(SERVICE& s, unsigned int id);

        /**
         * @brief Santa Claus stop current service provisioning
         * @param id The Santa Claus identifier
         */
        void end_service(unsigned int id);
};

#endif
