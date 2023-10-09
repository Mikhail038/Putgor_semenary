# ID function
## Process
every process has its __context__ and __PID__

## Users and Groups
data structure with its number == __UID__ \
plus another info \
Users can be gathered into __groups__ \
group has its number == __GID__ \
\
Every programm being started bu some __user__ from some __group__ \
This information is inside __programm context strucute__

## Also
/etc/passwd \
*FUNCS:*  __getpw...__\
..uid (uid_t) \
..nam (char*)
=> struct passwd*

/etc/group \
*FUNCS:*  __getgr...__\
..gid (gid_t) \
..nam (char*) \

## errno
strerror \
perror
