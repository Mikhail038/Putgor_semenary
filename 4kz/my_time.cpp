#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main (int argc, char** argv)
{
    const int MICROSECS_IN_SEC = 1000000;

    if (argc <= 2)
    {
        return 0;
    }

    struct timeval time;
    suseconds_t start_time = 0;
    suseconds_t end_time = 0;


    if (fork() == 0)
    {
        execvp (argv[1], &(argv[1]));
        perror ("Exec failed");
        return 0;
    }
    gettimeofday(&time, NULL);
    start_time = time.tv_usec + time.tv_sec * MICROSECS_IN_SEC;
    wait (NULL);
    gettimeofday(&time, NULL);
    end_time = time.tv_usec + time.tv_sec * MICROSECS_IN_SEC;

    std::cout << "elapsed time:" << std::endl << (double)(((double) end_time - (double) start_time) / MICROSECS_IN_SEC) << "s" << std::endl;
    std::cout << end_time - start_time << "u" << std::endl;
}
