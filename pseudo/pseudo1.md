```cpp
enum SERVICE
{
    DELIVERY,
    CONSULT,
    _TOT_SERVICES
};

class SantaClausV1 = Monitor
{
    int TOT[_TOT_SERVICES];
    int MIN_ELVES;

    // For Santa Claus

    condition_variable await_someone;
    condition_variable wait_all_passed;
    condition_variable wait_greetings;

    // For clients

    condition_variable wait_service[_TOT_SERVICES];
    int turnstile[_TOT_SERVICES];
    condition_variable wait_end_of_service;
    bool end_of_service;

    SantaClausV1(int n_reindeer, int n_elves, int min_elves)
    {
        TOT[DELIVERY] = n_reindeer;
        TOT[CONSULT] = n_elves;
        MIN_ELVES = min_elves;
        turnstile = {0}; // all zero
        end_of_service = false;
    }

    entry void new_service(SERVICE s)
    {
        if (await_someone.any()) // Santa is free
            await_someone.notify_one();
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

    entry void start_service(SERVICE& s)
    {
        while (!(wait_service[DELIVERY].getCnt() == TOT[DELIVERY] || wait_service[CONSULT].getCnt() >= MIN_ELVES))
            await_someone.wait(lock);
        
        if (wait_service[DELIVERY].getCnt() == TOT[DELIVERY]) // serving the reindeer
            s = DELIVERY;
        else // serving the elves
            s = CONSULT;
        turnstile[s] = (s == DELIVERY ? TOT[s] : MIN_ELVES);
        end_of_service = false;
        wait_service[s].notify_one(); // first reindeer/elf awakening
        while (turnstile[s] > 0)
            wait_all_passed.wait(lock);
    }

    entry void end_service()
    {
        end_of_service = true;
        wait_end_of_service.notify_one();
        while (wait_end_of_service.any())
            wait_greetings.wait(lock);
    }
};

SantaClausV1 sc = SantaClausV1(9, 10, 3);

void reindeer(SantaClausV1& sc)
{
    while (true)
    {
        <on vacation and wait Christmas>
        <head back to the North Pole>
        sc.new_service(DELIVERY);
        <head back to the Pacific Islands>
    }
}

void elf(SantaClausV1& sc, int id)
{
    while (true)
    {
        <make toys>
        sc.new_service(CONSULT);
    }
}

void santa(SantaClausV1& sc)
{
    SERVICE s;

    while (true)
    {
        sc.start_service(s);
        if (s == DELIVERY)
            <delivering toys>
        else
            <answer all questions in session>
        sc.end_service();
    }
}
```
