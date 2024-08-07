/*
 * Santa Claus Problem v3
*/

#ifndef SANTA_V3
#define SANTA_V3

#include "cnt_condition_variable.h"
#include <mutex>
#include <vector>

#define WAIT_VERBOSE

#ifdef WAIT_VERBOSE
#include <iostream>
#include "statistics.h"
#endif

#define NONE (-1)

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
        vector<unsigned int> MIN;

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
        SantaClaus();
        SantaClaus(unsigned int n_reindeer, unsigned int min_reindeer, unsigned int n_elves, unsigned int min_elves, unsigned int n_santa);
        ~SantaClaus();
        void new_service(SERVICE s, unsigned int& id_santa);
        void start_service(SERVICE& s, unsigned int id);
        void end_service(unsigned int id);
};

#endif
