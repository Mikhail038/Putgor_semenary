#include <stdio.h>
#include <unistd.h>

int main ()
{
    char* null_line = NULL;

    printf ("Hello");
    fflush(stdout);

    write (1, " world", 6);

    printf ("goodbye");
    // fflush(stdout);

    write (1, " see  ya", 8);

}
