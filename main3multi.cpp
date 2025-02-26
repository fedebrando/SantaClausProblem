
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>  
#include <list>
#include <cstdio>
#include <cmath>
#include <filesystem>
#include "statistics.hpp"

#define ELVES_MIN 3
#define ELVES_MAX 15
#define N_REINDEER 9

#define MAIN "main3"        // name of program to launch
#define EXEC_MAIN "./main3" // command-line program
#define SPACE "\t"
#define POLLING_PERIOD 5  // time period (seconds) after which there is file is checked
#define ROW_BYTES 9 // chars in a row (approximately)

#define N 10 // approximately number of data for each <n_elves, n_santa> pair

using namespace std;

// Reads MAIN process output file to get time values
void read_output(list<double>& values, const char* outfile);

// Retrieves average, standard deviation and number of values from MAIN execution
void get_stats(unsigned int n_elves, unsigned int n_santa, double& avg, double& std_dev, unsigned int& num_data);

int main(void)
{
    double avg, std_dev;
    unsigned int num_data;
    
    cout << "n_reind" << SPACE << "n_elves" << SPACE << "n_santa" << SPACE << "average" << SPACE << "std_dev" << SPACE << "num_data" << endl;
    for (unsigned int n_elves = ELVES_MIN; n_elves <= ELVES_MAX; n_elves++) // n_elves varying
    {
        for (unsigned int n_santa = 1; n_santa <= (1 + n_elves/ELVES_MIN); n_santa++) // n_santa varying
        {            
            cout << to_string(N_REINDEER) << SPACE << to_string(n_elves) << SPACE << to_string(n_santa);

            get_stats(n_elves, n_santa, avg, std_dev, num_data);

            // Print results for this couple
            cout << SPACE << dot2comma(to_string(avg)) << SPACE << dot2comma(to_string(std_dev)) << SPACE << num_data << endl;
        }
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

void get_stats(unsigned int n_elves, unsigned int n_santa, double& avg, double& std_dev, unsigned int& num_data)
{
    const char* outfile = "output.txt";
    uintmax_t file_bytes = 0;
    list<double> values;
    double sum, sum_of_squared;
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

    // parent
    do
    {
        sleep(POLLING_PERIOD);
        try
        {
            file_bytes = filesystem::file_size(outfile);
        }
        catch (const filesystem::filesystem_error& e)
        {
            cerr << "Errore: " << e.what() << endl;
        }
    } while (file_bytes / ROW_BYTES < N);
    
    // Kill child process
    if (kill(pid, SIGTERM)) 
        cerr << "Error in terminating child process" << endl;
    waitpid(pid, NULL, 0);

    // Read file
    read_output(values, outfile);

    // Compute stats
    sum = sum_of_squared = 0;
    for (double val : values)
    {
        sum += val;
        sum_of_squared += val * val;
    }

    // Set stats
    num_data = values.size();
    avg = sum / num_data;
    std_dev = sqrt(sum_of_squared / num_data - avg * avg);
}
