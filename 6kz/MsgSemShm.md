# Inter Process Communication (IPC)
## System V IPC
>msg (message) \
>sem (semaphore) \
>mem/shm (memory)

# Messages
### open -> read/write -> close
get -> rcv/snd (msg) | ... | .. | -> ctl (control)

msgget|semget|shmget -> msgrcv\msgsnd e.t.c

## msgget
>Key {32bit} \
>flags | permissions

returns __id__

### Flags
IPC_CREAT, IPC_EXCL

## ftok (file to key)
>path \
>n [0..255]

IPC_PRIVATE = const key if key needed only in parent-child relations

## msgsnd
>id \
>buf. first long (8-byte) is __type__  of data in msg \
>len. extra data (if len == 0 type is one) \
> flags

## msgrcv
>id \
>buf. first long (8-byte) is __type__  of msg \
>len. amount of extra data (if len == 0 type is one) \
>type (== 0 - any type. < 0 all that lesser than abs(type). > 0 only that type ) \
>flags

## msgctl
>id \
>cmd \
>p. extra arg

cmd to remove fifo __IPC_RMID__

# Semaphores
## critical section
~~~C
begin
///

///
end
~~~
processes that come to barier __together__ must pass it __one-by-one__ \
__ENTER__ -> /// -> __LEAVE__ \
than another

## race condition
__atomic__ compare-and-set

## binary semaphore
>down/enter/P \
>1 -> 0 -> \
>0 -> 0 wait

>up/leave/V \
>0 -> 1 \
>1 -> undefined

## counted semaphore
int >= 0

__+__ \
__-__ \
__==0__

## semget (Key, n, flags)
need to by initialised

## semget (Key, n, flags)
## semop (id, arr, n)
## semctl (id, flag)
SETVAL
GETVAL
better not use
