# Files and streams

## stdio.h
buffer emptyfing
1. full
2. \n
3. fflush();
4. exit (); return from main

## FILE*
int fd - file descriptor

__fd table__ in __process context__
array of numerated cells \
elements of this table are __pointers__ to OS provided memory

0. stdin
1. stdout
2. stderr

related to one hardware - __tty__

## syscalls
int fd; \
char* buffer; \
size_t count;

### ssize_t read ( fd, buf, count )
> returnvalue == n > 0 -> __n <= count__ \
> returnvalue == n < 0 -> __error occurred__ \
> returnvalue == n == 0 -> __EOF__

### ssize_t write ( fd, buf, count)
> returnvalue == n >= 0 -> __n <= count__ \
> returnvalue == n < 0 -> __error occurred__ \

if n <= count \
call write one more time with lesser count (count -= n) \
and shifted buffer pointer

## cat
without arguments redas stdin and writes to stdout

## int open (const char* path, int flags, int mode)
path from __/(root)__ is __absolute__ \
path not from root is __relative__

> returnvalue == n >= 0 -> __n == fd__ \
> returnvalue == n < 0 -> __error occurred__ \

### flags:
O_RDONLY
O_WRONLY
O_RDWR

O_WRONLY | O_CREAT | O_TRUNC \
...........................^ create....^ delete all info from file

O_EXCL return error if file exists \
O_APPEND to append new text to end of file

### mode:
\0 7 \0 \0 is to __R W X__

## cp
-f --force \
-v --verbose \
-i --interactive \
-h --help

getopt \
getopt_long

__params:__ argc, argv, "fvih"
__params_long:__ argc, argv, {"--force", 'f'}

while __-1__ is not returned

these functions __reorganize argv__ so in __optin__ is number of first non-flag line
