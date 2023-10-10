#include <bits/types/struct_timeval.h>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <climits>

#include <linux/limits.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

size_t count_words_in_buf (char* buffer, int size, bool* wait_space);
size_t count_lines_in_buf (char* buffer, int size);

int main (int argc, char** argv)
{
    if (argc <= 2)
    {
        return 0;
    }

    int fds[2]{};
    pipe(fds);

    if (fork() == 0)
    {
        dup2(fds[1], STDOUT_FILENO);
        close(fds[1]);

        execvp(argv[1], &(argv[1]));
        perror("Exec failed");

        return 0;
    }
    close (fds[1]);

    char* out_buffer = (char*) calloc(PIPE_BUF, sizeof(char));

    size_t count_bytes = 0;
    size_t count_words = 0;
    size_t count_lines = 0;

    int readed_bytes = 0;
    bool wait_space = false;

    while (true)
    {
        readed_bytes = read(fds[0], out_buffer, PIPE_BUF);
        if (readed_bytes <= 0)
        {
            break;
        }

        // std::cout << out_buffer << std::endl;

        count_bytes += readed_bytes;
        count_lines += count_lines_in_buf (out_buffer, readed_bytes);
        count_words += count_words_in_buf (out_buffer, readed_bytes, &wait_space);
    }

    wait (NULL);

    std::cout << count_lines << " " << count_words << " " << count_bytes << " " << std::endl;

    free (out_buffer);
}



size_t count_lines_in_buf (char* buffer, int size)
{
    size_t ret_value = 0;

    for (size_t cnt = 0; cnt != size; ++cnt)
    {
        if (buffer[cnt] == '\n')
        {
            ++ret_value;
        }
    }

    return ret_value;
}

size_t count_words_in_buf (char* buffer, int size, bool* wait_space)
{
    size_t ret_value = 0;

    for (size_t cnt = 0; cnt != size; ++cnt)
    {
        while (((isalpha(buffer[cnt]) != 0) || (isdigit(buffer[cnt]) != 0)) && (cnt != (size - 1)))
        {
            *wait_space = true;
            ++cnt;
            continue;
        }

        if (*wait_space)
        {
            ++ret_value;
            *wait_space = false;
        }
    }

    return ret_value;
}
