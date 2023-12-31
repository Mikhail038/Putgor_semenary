#pragma once
//===================================================================================================================

#include "../colors.h"
#include "../MYassert.h"

//===================================================================================================================

#include <cstddef>
#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


//===================================================================================================================

ssize_t my_write (int fd, char* buffer, size_t count);
void clean_buffer (char* buffer, size_t count);
int output_file (const char* source_path, int destination_fd);
int read_plus_write (int source_fd, int destination_fd);

//===================================================================================================================

typedef struct
{
    bool force_flag = false;
    bool verbose_flag = false;
    bool interactive_flag = false;
    bool help_flag = false;

    bool many_files;
} Flags;

int copy_file (const char* source_path, const char* destination_path, Flags* flags);

void print_help_cp ();


//===================================================================================================================


