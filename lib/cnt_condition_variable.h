/*
 * Class cnt_condition_variable HAS-A condition_variable
 * A condition_variable with encapsulated counter
*/

#ifndef CNT_CONDITION_VARIABLE
#define CNT_CONDITION_VARIABLE

#include <condition_variable>
#include <mutex>

using namespace std;

// A condition variable with encapsulated counter
class cnt_condition_variable
{
    private:
        condition_variable cv;
        unsigned int cnt;
        
    public:
        cnt_condition_variable();
        ~cnt_condition_variable();

        void wait(unique_lock<mutex>& lock);
        void notify_one();
        void notify_all();

        // Returns the number of threads which are waiting on this condition variable
        unsigned int getCnt();

        // Returns True if there's at least one thread which waits on this condition variable, False otherwise
        bool any();
};

#endif
