
#ifndef SANTA_V2
#define SANTA_V2

#include "cnt_condition_variable.hpp"
#include <mutex>
#include <vector>

enum SERVICE
{
    DELIVERY,
    CONSULT,
    _TOT_SERVICES
};

/**
 * @brief Monitor for Santa Claus Problem (version 2)
 * 
 * In this problem version, Santa Claus and reindeer
 * deliver toys independently
 */
class SantaClaus
{
    private:
        mutex mtx;

        vector<unsigned int> TOT;
        unsigned int MIN_ELVES;

        cnt_condition_variable await_someone;
        cnt_condition_variable wait_all_passed;
        cnt_condition_variable wait_greetings;
        cnt_condition_variable wait_reindeer_return;

        vector<cnt_condition_variable> wait_service;
        vector<unsigned int> turnstile;
        cnt_condition_variable wait_end_of_service;
        bool end_of_service;
        
    public:
        /**
         * @brief Instantiates a new SCP monitor for the classical case
         * 
         * Instantiates a new SCP monitor for 9 reindeer, 10 elves
         * and with 3 elves in a consulting group
         */
        SantaClaus();

        /**
         * @brief Instantiates a new SCP monitor with specified numbers of reindeer, elves and
         * elves in a consulting group
         * 
         * @param n_reindeer Number of reindeer
         * @param n_elves Number of elves
         * @param min_elves Number of elves in a consulting group
         */
        SantaClaus(unsigned int n_reindeer, unsigned int n_elves, unsigned int min_elves);

        ~SantaClaus();

        /**
         * @brief An elf requires new consultation to Santa Claus as a RPC
         */
        void new_consult();

        /**
         * @brief A rein make itself available for delivering toys
         */
        void new_delivery();

        /**
         * @brief A rein terminates its delivery
         */
        void end_delivery();

        /**
         * @brief Santa Claus makes himself available to provide a service
         * @param s Reference to obtain the required type of service
         */
        void start_service(SERVICE& s);

        /**
         * @brief Santa Claus stop current service provisioning
         */
        void end_service(SERVICE s);
};

#endif
