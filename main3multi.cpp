
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>  
#include <list>
#include <cstdio>
#include <cmath>
#include "statistics.hpp"

#define ELVES_MIN 3
#define ELVES_MAX 15
#define N_REINDEER 9
#define N_EQ_AVG 5          // minimum number of average not-significantly variation to compute results for a single (n_elves, n_santa) couple
#define MAIN "main3"        // name of program to launch
#define EXEC_MAIN "./main3" // command-line program
#define EXEC_TIME 13        // MAIN program execution time (seconds)
#define SPACE "\t"
#define PRECISION 0.1       // not-significantly variation threshold
#define BATCH_SIZE 1000     // number of time values for each iteration

using namespace std;

// Updates received values with new BATCH_SIZE time values sampled from several execution of MAIN
void update(double& sum, double& sum_of_squared, int& num_values, int n_elves, int n_santa);

// Inserts in a list new time values received from an execution of MAIN process
void get_times(unsigned int n_elves, unsigned int n_santa, list<double>& values);

// Reads MAIN process output file to get time values
void read_output(list<double>& values, const char* outfile);

int main(void)
{
    double sum, sum_of_squared, avg, std_dev, old_avg = 0;
    int num_values;
    int cnt_n_cng; // number of average not-significantly variation
    
    cout << "n_reind" << SPACE << "n_elves" << SPACE << "n_santa" << SPACE << "average" << SPACE << "std_dev" << endl;
    for (unsigned int n_elves = ELVES_MIN; n_elves <= ELVES_MAX; n_elves++) // n_elves varying
    {
        for (unsigned int n_santa = 1; n_santa <= (1 + n_elves/ELVES_MIN); n_santa++) // n_santa varying
        {
            if (n_santa == 1 && n_elves == ELVES_MIN) // TODO DA TOGLIERE
                continue;
            
            sum = sum_of_squared = 0;
            num_values = 0;
            cnt_n_cng = 0;
            old_avg = 0;

            cout << to_string(N_REINDEER) << SPACE << to_string(n_elves) << SPACE << to_string(n_santa) << endl;

            while (cnt_n_cng < N_EQ_AVG) // (n_elves, n_santa) couple iterations
            {
                // Get new BATCH_SIZE time values
                update(sum, sum_of_squared, num_values, n_elves, n_santa);

                // Compute average
                avg = sum / num_values;

                // Average not-significantly variantion checking
                if (old_avg) // satisfied for all iterations but first one
                {
                    if (abs(old_avg - avg) <= PRECISION)
                        cnt_n_cng++;
                    else
                        cnt_n_cng = 0;
                }
                old_avg = avg; // update old average
            }

            // Compute standard deviation as a variance squared root
            std_dev = sqrt(sum_of_squared / num_values - avg * avg); // variance is the the difference between the avg of squares and the square of avg

            // Print results for this couple
            cout << SPACE << dot2comma(to_string(avg)) << SPACE << dot2comma(to_string(std_dev)) << endl;
        }
    }    
}

void update(double& sum, double& sum_of_squared, int& num_values, int n_elves, int n_santa)
{
    static list<double> values;
    double curr_val, curr_sum, curr_sum_of_squared;

    // Read times until they are at least BATCH_SIZE
    while (values.size() < BATCH_SIZE)
        get_times(n_elves, n_santa, values);

    // Compute sum and sum of squared of the BATCH_SIZE values
    curr_sum = curr_sum_of_squared = 0;
    for (int i = 0; i < BATCH_SIZE; i++)
    {
        curr_val = values.front();

        curr_sum += curr_val;
        curr_sum_of_squared += curr_val * curr_val;

        values.pop_front();
    }

    // Update
    sum += curr_sum;
    sum_of_squared += curr_sum_of_squared;
    num_values += BATCH_SIZE;
}

void get_times(unsigned int n_elves, unsigned int n_santa, list<double>& values)
{
    const char* outfile = "output.txt";
    int fd;
    pid_t pid = fork();

    if (pid < 0) 
    {
        cerr << "Error during fork" << endl;
        exit(1);
    }

    if (pid == 0) // child
    {
        // Open output file
        fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd < 0) 
        {
            cerr << "Error in opening file" << endl;
            exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) 
        {
            cerr << "Error in dup2" << endl;
            close(fd);
            exit(1);
        }
        close(fd);

        // Change process execution to EXEC_MAIN
        execlp(EXEC_MAIN, MAIN, to_string(N_REINDEER).c_str(), to_string(n_elves).c_str(), to_string(n_santa).c_str(), (char *)NULL);
        cerr << "Error in executing execlp" << endl;
        exit(1);
    } 
    else // parent
    {
        // Sleep during child execution for 10 seconds
        sleep(EXEC_TIME);

        // Kill child process
        if (kill(pid, SIGTERM)) 
            cerr << "Error in terminating child process" << endl;
        waitpid(pid, NULL, 0);

        // Read child output
        read_output(values, outfile);
    }
}

void read_output(list<double>& values, const char* outfile)
{
    double value;
    ifstream file(outfile);

    if (!file) 
    {
        cerr << "Error in opening file " << outfile << endl;
        exit(1);
    }

    while (file >> value) 
        values.push_back(value);
    file.close();
    if (file.bad()) 
    {
        cerr << "Error in file reading" << endl;
        exit(1);
    }
    if (remove(outfile)) 
    {
        cerr << "Error in deleting file " << outfile << endl;
        exit(1);
    }
}
