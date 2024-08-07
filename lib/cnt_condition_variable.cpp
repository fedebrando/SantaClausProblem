
#include "cnt_condition_variable.h"

cnt_condition_variable::cnt_condition_variable() : cv(), cnt(0)
{}

cnt_condition_variable::~cnt_condition_variable()
{}

void cnt_condition_variable::wait(unique_lock<mutex>& lock)
{
    cnt++;
    cv.wait(lock);
    cnt--;
}

void cnt_condition_variable::notify_one()
{
    cv.notify_one();
}

void cnt_condition_variable::notify_all()
{
    cv.notify_all();
}

// Returns the number of threads which are waiting on this condition variable
unsigned int cnt_condition_variable::getCnt()
{
    return cnt;
}

// Returns the truth value of the proposition "There is at least one thread which is waiting on this condition variable"
bool cnt_condition_variable::any()
{
    return cnt > 0;
}
