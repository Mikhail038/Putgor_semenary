#include <iostream>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>

pid_t getpid(void);
pid_t getppid(void);

int main (int argc, char** argv)
{
    std::cout << "YA PID" "\x1b[31m" << getpid() << "\x1B[0m""\n" "MOI PAPA PID" "\x1b[33m" << getppid() << "\x1B[0m" "\n";

    return 0;
}
