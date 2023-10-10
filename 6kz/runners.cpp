#include <cstddef>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <cstdint>

typedef struct
{
    long type;
    char data[0];
} Message;

const uint64_t TIME_PER_RUNNER = 10;

const long MSG_SIZE = 0;
const long delta = 10;

void runner (uint64_t cnt_runner, uint64_t amnt_runners, int key);
void judge_and_runners (uint64_t amnt_runners, int key);

int main ()
{
    const uint64_t amnt_runners = 10;

    msgget (IPC_PRIVATE, S_IRWXU | S_IRWXG | S_IRWXO);

    judge_and_runners (amnt_runners + delta, IPC_PRIVATE);

    for (uint64_t cnt = 0; cnt != amnt_runners; ++cnt)
    {
        wait (NULL);
    }

    msgctl (IPC_PRIVATE, IPC_RMID, NULL);
}

void runner (uint64_t cnt_runner, uint64_t amnt_runners, int key)
{
    if (fork() == 0)
    {
        Message msg;
        msg.type = cnt_runner;

        printf ("R%ld: I came!\n", cnt_runner);
        msgsnd(key, (const void *) &msg, MSG_SIZE, 0);

        Message buf;
        msgrcv(key, &buf, MSG_SIZE, cnt_runner - 1, 0);

        printf ("R%ld: Started!\n", cnt_runner);

        usleep (TIME_PER_RUNNER);

        printf ("R%ld: Finished!\n", cnt_runner);
        msgsnd(key, (const void *) &msg, MSG_SIZE, 0);

        exit (0);;
    }

    if (cnt_runner != amnt_runners - 1)
    {
        runner (cnt_runner + 1, amnt_runners, key);
    }
}

void judge_and_runners (uint64_t amnt_runners, int key)
{
    if (fork() == 0)
    {
        runner(delta , amnt_runners, IPC_PRIVATE);

        exit (0);
    }

    printf ("J: I came\n");

    Message buf;
    for (uint64_t cnt = delta; cnt != amnt_runners; ++cnt)
    {
        printf("J: Where is [%ld/%ld]?\n", cnt, amnt_runners);
        msgrcv(key, &buf, MSG_SIZE, cnt, 0);
        printf("J: Ok %ld\n", buf.type);
    }


    printf("J: Start!\n");

    buf.type = delta - 1;
    msgsnd(key, &buf, MSG_SIZE, 0);

}
