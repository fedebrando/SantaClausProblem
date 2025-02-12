#include "santa_v3.h"

using namespace std;

SantaClaus::SantaClaus() : SantaClaus(9, 9, 10, 3, 1)
{}

SantaClaus::SantaClaus(unsigned int n_reindeer, unsigned int min_reindeer, unsigned int n_elves, unsigned int min_elves, unsigned int n_santa)
    : mtx(), TOT(_TOT_SERVICES), MIN(_TOT_SERVICES),
      id_santa_selected(NONE), n_santa(n_santa), await_someone(n_santa), wait_all_passed(n_santa), wait_greetings(n_santa), 
      wait_service(_TOT_SERVICES), turnstile(_TOT_SERVICES, 0), wait_end_of_service(n_santa), end_of_service(n_santa, false)
{
    TOT[DELIVERY] = n_reindeer;
    TOT[CONSULT] = n_elves;
    MIN[DELIVERY] = min_reindeer;
    MIN[CONSULT] = min_elves;
}

SantaClaus::~SantaClaus()
{}

void SantaClaus::new_service(SERVICE s, unsigned int& id_santa)
{
    unique_lock<mutex> lock(mtx);

#ifdef WAIT_VERBOSE
    chrono::_V2::system_clock::time_point start;
    chrono::_V2::system_clock::time_point stop;
    chrono::duration<double, milli> elapsed; 
#endif

    if (id_santa_selected != NONE)
        if (await_someone[id_santa_selected].any()) // selected Santa is free TODO (c'è bisogno di questo if? vedi 77-78)
            await_someone[id_santa_selected].notify_one();
#ifdef WAIT_VERBOSE
    start = chrono::high_resolution_clock::now();
#endif
    while (turnstile[s] == 0)
        wait_service[s].wait(lock);
#ifdef WAIT_VERBOSE
    stop = chrono::high_resolution_clock::now();
    elapsed = stop - start;
    cout << to_string(elapsed.count()) << endl;
#endif
    
    id_santa = id_santa_selected;
    turnstile[s]--;
    if (turnstile[s] > 0)
        wait_service[s].notify_one();
    else
        wait_all_passed[id_santa].notify_one();
    while (!end_of_service[id_santa])
        wait_end_of_service[id_santa].wait(lock);
    
    if (wait_end_of_service[id_santa].any())
        wait_end_of_service[id_santa].notify_one();
    else
        wait_greetings[id_santa].notify_one();
}

void SantaClaus::start_service(SERVICE& s, unsigned int id)
{
    unique_lock<mutex> lock(mtx);

    if (id_santa_selected == NONE)
        id_santa_selected = id;
    while (!((wait_service[DELIVERY].getCnt() >= MIN[DELIVERY] || wait_service[CONSULT].getCnt() >= MIN[CONSULT]) && id_santa_selected == (int) id))
        await_someone[id].wait(lock);
    
    if (wait_service[DELIVERY].getCnt() >= MIN[DELIVERY]) // serving the reindeer 
        s = DELIVERY;
    else // serving the elves
        s = CONSULT;
    turnstile[s] = MIN[s];
    end_of_service[id] = false;
    wait_service[s].notify_one(); // first reindeer/elf awakening
    while (turnstile[s] > 0)
        wait_all_passed[id].wait(lock);

    id_santa_selected = NONE;
    for (unsigned int i = 0; i < n_santa; i++) // selecting new santa
        if (await_someone[i].any())
        {
            id_santa_selected = i; // TODO perché non c'è await_someone[i].notify_one()?
            break;
        }
}

void SantaClaus::end_service(unsigned int id)
{
    unique_lock<mutex> lock(mtx);

    end_of_service[id] = true;
    wait_end_of_service[id].notify_one();
    while (wait_end_of_service[id].any())
        wait_greetings[id].wait(lock);
}
