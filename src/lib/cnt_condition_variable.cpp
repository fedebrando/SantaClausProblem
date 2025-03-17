
#include "cnt_condition_variable.hpp"

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

unsigned int cnt_condition_variable::getCnt()
{
    return cnt;
}

bool cnt_condition_variable::any()
{
    return cnt > 0;
}
