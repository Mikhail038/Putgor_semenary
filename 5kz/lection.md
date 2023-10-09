# Inter Process Communication (IPC)
## PIPE
Literature: W.Stevens (IPC) \
(simplified) Robachevskiy (OS UNIX)

>PROCESS _->fork->_  CHILD + PARENT

### fd table

## int pipe (int fds[])
opens FIFO pipe (char)
>[0] - read \
>[1] - write
other array elements are useless


one fd to write in pipe \
another to read from pipe

_read_ __blocked__ until data \
it ma cause double waiting problemms \
so usually half of fd's are __blocked__

In need of two-way communication \
usually create __another pipe__

__PIPE_BUF__ is constant for pipe buffer size \
[4k...8k]

_write_ can be __blocked__ too (in case of buf overflow)

If data_size <= PIPE_BUF \
_write_ will wait untill in pipe buffer will be enough place \
(__atomic__ operation)

_read_ returns 0 in case:
> no processes have access to pipe write channel \
> __AND__ pipe buffer is empty

_read_ returns 0 in case:
> no processes have access to pipe read channel \

## communication with 3rd party process
0, 1, 2 std in/out/err

__$__ cmd1 | cmd2 \
stdout ->pipe-> stdin

## Syscalss
> dup \
> dup3 \
> dup3

~~~C
{
    close(1);
    dup(fds[1]);
} //dup2

close(fds[1]);
~~~

## Hometask
## my_sh**l

promt __$__ echo hello | cat | wc
