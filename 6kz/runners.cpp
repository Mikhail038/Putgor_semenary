#include <climits>
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

const long JUDGE_TYPE = LONG_MAX;

void runner (uint64_t cnt_runner, id_t id);
void judge (uint64_t amnt_runners, id_t id);

int main ()
{
    const uint64_t amnt_runners = 5;

    id_t id = msgget (IPC_PRIVATE, S_IRWXU | S_IRWXG | S_IRWXO);

    for (uint64_t cnt = 1; cnt != amnt_runners + 1; ++cnt)
    {
        runner(cnt, id);
    }

    judge (amnt_runners, id);

    for (uint64_t cnt = 1; cnt != amnt_runners + 1; ++cnt)
    {
        wait (NULL);
    }

    msgctl (id, IPC_RMID, NULL);
}

void runner (uint64_t cnt_runner, id_t id)
{
    if (fork() == 0)
    {
        Message msg_to_snd;
        msg_to_snd.type = JUDGE_TYPE;
        printf ("R%ld: I came!\n", cnt_runner);
        if (msgsnd(id, (const void *) &msg_to_snd, MSG_SIZE, 0) < 0)
        {
            perror ("name snd");
        }

        Message msg_to_rcv;
        if (msgrcv(id, &msg_to_rcv, MSG_SIZE, cnt_runner, 0) < 0)
        {
            perror ("start rcv");
        }
        printf ("R%ld: Started!\n", cnt_runner);


        usleep (TIME_PER_RUNNER);

        msg_to_snd.type = cnt_runner + 1;
        printf ("R%ld: Finished!\n", cnt_runner);
        if (msgsnd(id, (const void *) &msg_to_snd, MSG_SIZE, 0) < 0)
        {
            perror ("finish snd");
        }

        exit (0);
    }
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
        printf("J: Ok %ld/%ld\n", cnt, amnt_runners);

    }


    printf("J: Start!\n");

    buf.type = 1;
    msgsnd(id, &buf, MSG_SIZE, 0);

    if (msgrcv(id, &buf, MSG_SIZE, amnt_runners + 1, 0) < 0)
    {
        perror ("");
    }
    printf ("J: FINISH\n");
}
