#include <cstring>
#include <iostream>
#include <new>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <vector>

#include "sys_funcs.hpp"

const std::uint64_t LINE_BUF_SIZE = 128;

int main ()
{
    char* line = new char [LINE_BUF_SIZE];

    user_loop(line);

    delete[] line;
}
