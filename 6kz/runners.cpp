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
    char data[1];
} Message;

const uint64_t TIME_PER_RUNNER = 10;

const long MSG_SIZE = 0;

void runner (uint64_t amnt_runner, id_t id);
void judge (uint64_t amnt_runners, id_t id);

int main ()
{
    const uint64_t amnt_runners = 10;

    id_t id = msgget (IPC_PRIVATE, S_IRWXU | S_IRWXG | S_IRWXO);

    if (fork () == 0)
    {
        for (uint64_t cnt = 1; cnt != amnt_runners + 1; ++cnt)
        {
            runner(cnt, id);
        }
        exit (0);
    }

    judge (amnt_runners, id);

    for (uint64_t cnt = 0; cnt != amnt_runners; ++cnt)
    {
        wait (NULL);
    }

    msgctl (id, IPC_RMID, NULL);
}

void runner (uint64_t cnt_runner, id_t id)
{
    if (fork() == 0)
    {
        Message msg;

        printf ("R%ld: I came!\n", cnt_runner);

        msg.type = cnt_runner;
        if (msgsnd(id, (const void *) &msg, MSG_SIZE, 0) < 0)
        {
            perror ("name snd");
        }

        Message buf;
        if (msgrcv(id, &buf, MSG_SIZE, cnt_runner, 0) < 0)
        {
            perror ("start rcv");
        }

        printf ("R%ld: Started!\n", cnt_runner);

        usleep (TIME_PER_RUNNER);

        printf ("R%ld: Finished!\n", cnt_runner);

        msg.type = cnt_runner + 1;
        if (msgsnd(id, (const void *) &msg, MSG_SIZE, 0) < 0)
        {
            perror ("finish snd");
        }

        exit (0);
    }

    // if (cnt_runner != amnt_runners - 1)
    // {
    //     runner (cnt_runner + 1, amnt_runners, id);
    // }
}

void judge (uint64_t amnt_runners, id_t id)
{
    printf ("J: I came\n");

    Message buf;
    for (uint64_t cnt = 1; cnt != amnt_runners + 1; ++cnt)
    {
        printf("J: Where is runner?\n");
        if (msgrcv(id, &buf, MSG_SIZE, 0, 0) < 0)
        {
            perror ("");
        }

        printf("J: Ok %ld/%ld\n", buf.type, amnt_runners);
    }


    printf("J: Start!\n");

    buf.type = 1;
    msgsnd(id, &buf, MSG_SIZE, 0);

}
