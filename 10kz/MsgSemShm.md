# Threads

process

>context\
>\
>user\
>kernel\
>cpu [0, 1, 2, ...]

__sheduler__ ^

## multithreading
one process can have __many__ cpu contexts

## POSIX THREADS (PTHREAD)

### pthread_create (&tid, &attr, &func, data
returns **0** in case of **succes**

### pthread_join (tid, &value)
waits for end of thread function and takes __ret_value__

## Mutex
~~~C
pthread_mutex_t __m__ = PTHREAD_MUTEX_INITIALIZER
~~~
or
~~~C
int pthread_mutex_init(&m, attr);
~~~

~~~C
__pthread_mutex_destroy(&m)__

__pthread_mutex_lock (&m)__

__pthread_mutex_unlock (&m)__
~~~

### condition

~~~C
cond_t condition c = PTHREAD_CONDITION_INITIALIZER;
//or
pthread_conditin_init (&c, addr);
..._signal (&c);
..._broadcast (&c);
~~~

## Monitor
~~~CXX
struct Monitor
{
    mutex_t m
    cond_t c1, c2, c3;

    Monitor ();
    ~Monitor ();

};
~~~
