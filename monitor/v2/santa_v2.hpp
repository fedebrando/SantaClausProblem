/*
 * Santa Claus Problem v2
*/

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
        SantaClaus();
        SantaClaus(unsigned int n_reindeer, unsigned int n_elves, unsigned int min_elves);
        ~SantaClaus();
        void new_consult();
        void new_delivery();
        void end_delivery();
        void start_service(SERVICE& s);
        void end_service(SERVICE s);
};

#endif
