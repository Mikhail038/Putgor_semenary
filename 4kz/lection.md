# Process in UNIX/POSIX

## Process examples

[1] __init__ (made by core) => __daemons__

### daemons:

__login__/__getty__ \
session_kde
terminal -> shell -> cmd

## __fork()__
>original (PID) \
>child (PID_child)

fork copies all original process context, \
__but:__
1. PID is differenet
2. Fork returns PID_child from parent process and 0 from child process (<0 in case of error).

original and child have same past but different future.


## strace
-e trace=clone,wait,write,...

## exec ( .. )
__destroying__ old context and creating __new__ context
(parh, argv, argc, env[n] "envirement")
but __PID__ stays __same__ and process still being run

>execl (path, argv[0..n], NULL) \
>execlp (path_or_name, argv[0..n], NULL) \
>execv (path, argv _{char** / char* []}_) \
>execv (path, argv) \
>execvp (path_or_name, argv) \
>execvpe (path_or_name, argv, env _{char**}_)
