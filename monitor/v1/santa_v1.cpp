
#include "santa_v1.hpp"

using namespace std;

SantaClaus::SantaClaus() : SantaClaus(9, 10, 3)
{}

SantaClaus::SantaClaus(unsigned int n_reindeer, unsigned int n_elves, unsigned int min_elves)
    : mtx(), TOT(_TOT_SERVICES), MIN_ELVES(min_elves),
      await_someone(), wait_all_passed(), wait_greetings(), 
      wait_service(_TOT_SERVICES), turnstile(_TOT_SERVICES, 0), wait_end_of_service(), end_of_service(false)
{
    TOT[DELIVERY] = n_reindeer;
    TOT[CONSULT] = n_elves;
}

SantaClaus::~SantaClaus()
{}

void SantaClaus::new_service(SERVICE s)
{
    unique_lock<mutex> lock(mtx);

    if (await_someone.any()) // Santa is free
        await_someone.notify_one();
    
#ifdef V1_PROB_VERBOSE
    if (s == DELIVERY && wait_service[s].getCnt() == TOT[DELIVERY] - 1)
    {
        if (await_someone.any())
            cout << "Santa is free!" << endl;
        else
            cout << "Santa is busy!" << endl;
    }
#endif

    while (turnstile[s] == 0)
        wait_service[s].wait(lock);
    
    turnstile[s]--;
    if (turnstile[s] > 0)
        wait_service[s].notify_one();
    else
        wait_all_passed.notify_one();
    while (!end_of_service)
        wait_end_of_service.wait(lock);
    
    if (wait_end_of_service.any())
        wait_end_of_service.notify_one();
    else
        wait_greetings.notify_one();
}

void SantaClaus::start_service(SERVICE& s)
{
    unique_lock<mutex> lock(mtx);

    while (!(wait_service[DELIVERY].getCnt() == TOT[DELIVERY] || wait_service[CONSULT].getCnt() >= MIN_ELVES))
        await_someone.wait(lock);
    
    if (wait_service[DELIVERY].getCnt() == TOT[DELIVERY])
        s = DELIVERY; // serving the reindeer 
    else
        s = CONSULT; // serving the elves
    turnstile[s] = (s == DELIVERY ? TOT[s] : MIN_ELVES);
    end_of_service = false;
    wait_service[s].notify_one(); // first reindeer/elf awakening
    while (turnstile[s] > 0)
        wait_all_passed.wait(lock);
}

void SantaClaus::end_service()
{
    unique_lock<mutex> lock(mtx);

    end_of_service = true;
    wait_end_of_service.notify_one();
    while (wait_end_of_service.any())
        wait_greetings.wait(lock);
}
