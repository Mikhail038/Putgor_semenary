# Inter Process Communication (IPC)
## System V IPC
>msg (message) \
>sem (semaphore) \
>mem/shm (memory)

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
