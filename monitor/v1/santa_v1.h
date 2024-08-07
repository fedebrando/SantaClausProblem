/*
 * Santa Claus Problem v1
*/

#ifndef SANTA_V1
#define SANTA_V1

#include "cnt_condition_variable.h"
#include <mutex>
#include <vector>

#define PROB_VERBOSE

#ifdef PROB_VERBOSE
#include <iostream>
#endif

enum SERVICE
{
    DELIVERY,
    CONSULT,
    _TOT_SERVICES
};

class SantaClaus
{
    private:
        mutex mtx;

        vector<unsigned int> TOT;
        unsigned int MIN_ELVES;

        cnt_condition_variable await_someone;
        cnt_condition_variable wait_all_passed;
        cnt_condition_variable wait_greetings;

        vector<cnt_condition_variable> wait_service;
        vector<unsigned int> turnstile;
        cnt_condition_variable wait_end_of_service;
        bool end_of_service;
        
    public:
        SantaClaus();
        SantaClaus(unsigned int n_reindeer, unsigned int n_elves, unsigned int min_elves);
        ~SantaClaus();
        void new_service(SERVICE s);
        void start_service(SERVICE& s);
        void end_service();
};

#endif
