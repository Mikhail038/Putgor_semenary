#include <cmath>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

int main (int argc, char** argv)
{
    pid_t parent_pid = getpid();

    const int SLEEPY_CONST = 1000;

    for (int cnt = 1; cnt != argc; ++cnt)
    {
        fork();

        if  (getpid() != parent_pid)
        {
            // std::cout << "[" << getpid() << "]" << std::endl;

            int value = atoi(argv[cnt]);

            usleep (SLEEPY_CONST * value);
            std::cout << value << std::endl;

            return 0;
        }
    }

    for (int cnt = 1; cnt != argc; ++cnt)
    {
        wait (NULL);
    }

    return 0;
}
