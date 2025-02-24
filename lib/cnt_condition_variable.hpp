
#ifndef CNT_CONDITION_VARIABLE
#define CNT_CONDITION_VARIABLE

#include <condition_variable>
#include <mutex>

using namespace std;

/**
 * @brief A condition variable with encapsulated counter
 * @note This class HAS-A condition_variable
 */
class cnt_condition_variable
{
    private:
        condition_variable cv;
        unsigned int cnt;
        
    public:
        /**
         * @brief Instantiates a new condition variable with encapsulated counter
         */
        cnt_condition_variable();

        ~cnt_condition_variable();

        /**
         * @brief Set the the caller thread to wait on this condition variable
         *
         * This function releases the associated mutex held by the unique_lock
         * and blocks the current thread
         *
         * @param lock Reference to a unique_lock managing the associated mutex
         */
        void wait(unique_lock<mutex>& lock);

        /**
         * @brief Notifies a thread which waits on this condition variable
         *
         * Wakes up a thread which waits on this condition variable,
         * if there is one
         */
        void notify_one();

        /**
         * @brief Notifies all threads which wait on this condition variable
         *
         * Wakes up all thread which wait on this condition variable
         */
        void notify_all();

        /**
         * @brief Retrieves the number of threads which are waiting on this condition variable
         * @return the number of threads which are waiting on this condition variable
         */ 
        unsigned int getCnt();

        /**
         * @brief Checks if there's a thread in waiting on this condition variable
         * @return True if there's at least one thread which waits on this condition variable, False otherwise
         */
        bool any();
};

#endif
