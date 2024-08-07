#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>  
#include <list>
#include <cstdio>
#include <cmath>
#include "statistics.h"

#define ELVES_MIN 3
#define ELVES_MAX 15
#define N_REINDEER 9
#define MIN_ITERATIONS 5
#define N_EQ_AVG 2
#define MAIN "main3"
#define EXEC_MAIN "./main3"
#define SPACE "\t"
#define PRECISION 0.01

using namespace std;

void read_output(list<double>& values, const char* outfile);
void get_times(unsigned int n_reindeer, unsigned int n_elves, unsigned int n_santa, list<double>& values, const char* outfile);
double avg(const list<double>& values);
double std_dev(const list<double>& values, const double avg);

int main() {
    list<double> values;
    double average;
    double dev_std;
    double old_avg;
    int iter = 0;
    int cnt_n_cng = 0;
    const char* outfile = "output.txt";
    
    cout << "N_REINDEER" << SPACE << "N_ELVES" << SPACE << "N_SANTA" << SPACE << "AVERAGE" << SPACE << "STANDARD_DEVIATION" << endl;
    for (unsigned int n_elves = ELVES_MIN; n_elves <= ELVES_MAX; n_elves++)
    {
        for (unsigned int n_santa = 1; n_santa <= (1 + n_elves/ELVES_MIN); n_santa++)
        {
            values.clear();
            iter = 0;
            cnt_n_cng = 0;
            old_avg = 0;
            cout << to_string(N_REINDEER) << SPACE << to_string(n_elves) << SPACE << to_string(n_santa);
            while (iter < MIN_ITERATIONS || cnt_n_cng < N_EQ_AVG)
            {
                get_times(N_REINDEER, n_elves, n_santa, values, outfile);
                average = avg(values);
                dev_std = std_dev(values, average);        
                if (iter > 0)
                {
                    if (abs(old_avg - average) <= PRECISION)
                        cnt_n_cng++;
                    else
                        cnt_n_cng = 0;
                }
                iter++;
                old_avg = average;
            }
            cout << SPACE << dot2comma(to_string(average)) << SPACE << dot2comma(to_string(dev_std)) << endl;
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

void get_times(unsigned int n_reindeer, unsigned int n_elves, unsigned int n_santa, list<double>& values, const char* outfile)
{
    int fd;
    pid_t pid = fork();

    if (pid < 0) 
    {
        cerr << "Error during fork" << endl;
        exit(1);
    }

    if (pid == 0) // child
    {
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
        execlp(EXEC_MAIN, MAIN, to_string(n_reindeer).c_str(), to_string(n_elves).c_str(), to_string(n_santa).c_str(), (char *)NULL);
        cerr << "Error in executing execlp" << endl;
        exit(1);
    } 
    else // parent
    {
        sleep(5);
        if (kill(pid, SIGTERM)) 
            cerr << "Error in killing child process" << endl;
        waitpid(pid, NULL, 0);
        read_output(values, outfile);
    }
}

double avg(const list<double>& values)
{
    double sum = 0;

    for (const double& v : values)
        sum += v;
    
    return (sum/values.size());
}

double std_dev(const list<double>& values, const double avg)
{ 
    double sum_d = 0;

    for (const double& v : values)
        sum_d += (v - avg) * (v - avg);
    
    return sqrt(sum_d/values.size());
}