#include <iostream>
#include <string.h>

int main (int argc, char** argv)
{
    int number_of_words = argc - 1;
    if (number_of_words == 0)
    {
        std::cout << std::endl;
        return 0;
    }


    int cnt = 1;

    bool newline_mark = true;

    if (strncmp ("-n", argv[1], 3) == 0)
    {
        newline_mark = false;
        ++cnt;
    }

    if ((strncmp ("--help", argv[1], 7) == 0) && (number_of_words == 1))
    {
        std::cout << "my_echo " "\x1b[4m" "LONG_OPTION"  "\x1B[0m"  << std::endl;
        std::cout << "my_echo [SHORT_OPTION] [" "\x1b[4m" "word1 word2" "\x1B[0m" " ...]"  << std::endl << std::endl;
        std::cout << "\x1b[32m" "-n"  "\x1B[0m" "        " "no endline symbol"  << std::endl;
        std::cout << "\x1b[31m" "--help" "\x1B[0m" "    " "manual information" << std::endl;

        return 0;
    }

    bool not_first_word = false;
    for (; cnt != number_of_words+1; ++cnt)
    {
        if (not_first_word)
        {
            std::cout << " ";
        }

        std::cout << argv[cnt];
        not_first_word = true;
    }

    if (newline_mark)
    {
        std::cout << std::endl;
    }
}
