
// --force
// --verbose
// --interactive
// --help
#include "read_write_funcs.hpp"
#include <bits/getopt_core.h>
#include <bits/getopt_ext.h>
#include <cstddef>
        //    struct option {
        //        const char *name;
        //        int         has_arg;
        //        int        *flag;
        //        int         val;
        //    };

const struct option options_array[] =
{
    {"force",       no_argument, NULL, 'f'},
    {"verbose",     no_argument, NULL, 'v'},
    {"interative",  no_argument, NULL, 'i'},
    {"help",        no_argument, NULL, 'h'}
};

int main (int argc, char* const* argv)
{
    Flags flags;

    int getopt_return_value = 0;

    do
    {
        getopt_return_value = getopt_long(argc, argv, "fvih", options_array, NULL);
        // printf ("[%c] = %d\n", getopt_return_value, getopt_return_value);

        switch (getopt_return_value)
        {
            case ('f'):
                flags.force_flag = true;
                break;

            case ('v'):
                flags.verbose_flag = true;
                break;

            case ('i'):
                flags.interactive_flag = true;
                break;

            case ('h'):
                flags.help_flag = true;
                break;

            case (-1):
                break;

            default:
                MY_COOLER_ASSERT(0, 0);
        }
    } while (getopt_return_value != -1);

    if (flags.help_flag)
    {
        print_help_cp ();
        return 0;
    }

    int current_line = optind;

    // printf ("%d %d\n", argc, current_line);

    MY_COOLER_ASSERT(argc - current_line >= 2, 0);

    if (argc - current_line > 1)
    {
        flags.many_files = true;
    }

    // for (; current_line != argc; ++current_line)
    // {
        copy_file(argv[current_line], argv[current_line + 1], &flags);
    // }

    current_line += 2;
}
