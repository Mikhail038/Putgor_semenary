
#include "read_write_funcs.hpp"

int main (int argc, char** argv)
{
    int stdin_fd = 0;
    int stdout_fd = 1;

    bool no_arguments_mark = true;

    if (argc > 1)
    {
        no_arguments_mark = false;
    }


    if (no_arguments_mark)
    {
        MY_COOLER_ASSERT(read_plus_write (stdin_fd, stdout_fd) >= 0, 0);

        return 0;
    }
    //else
    for (int files_cnt = 1; files_cnt != argc; ++files_cnt)
    {
        // printf("%d [%s]\n", files_cnt, argv[files_cnt]);
        MY_COOLER_ASSERT(output_file (argv[files_cnt], stdout_fd) >= 0, 0);
    }
}
