#include <climits>
#include <cstddef>
#include <sys/types.h>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
#include <stdio.h>
#include <cstdint>

enum Semaphores
{
    bridge_free = 0,
    bridge_occupied = 1,
    boat_free = 3,
    boat_occupied = 4,
    tickets_free = 5,
    tickets_occupied = 6
};

#define N_SEMS 6

const uint64_t ROUNDS = 100;

const uint64_t TIME_PER_TRIP    = 1000;
const uint64_t TIME_PER_bridge  = 100;

const uint64_t passengers_amnt  = 20;
const uint64_t bridge_max_amnt  = 3;
const uint64_t boat_max_amnt    = 3;

void semaphore_init (id_t sem_id);
void passenger (uint64_t cnt, id_t sem_id);
void boat (id_t sem_id);

using SemOperation = sembuf;
// {
//     unsigned short sem_num; //индекс семафора в массиве
//     short sem_op; //код операции над семафором
//     short sem_flg; //флаг операции IPC_NOWAIT, SEM_UNDO
// } SemOperation;

int main ()
{
    id_t sem_id = semget (IPC_PRIVATE, 6, S_IRWXU | S_IRWXG | S_IRWXO);
    semaphore_init(sem_id);

    for (uint64_t cnt = 1; cnt != passengers_amnt + 1; ++cnt)
    {
        passenger(cnt, sem_id);
    }

    boat (sem_id);

    semctl (sem_id, IPC_RMID, 0);
}

void semaphore_init (id_t sem_id)
{
    semctl(sem_id, tickets_free,        SETVAL, boat_max_amnt);
    semctl(sem_id, tickets_occupied,    SETVAL, 0);
    semctl(sem_id, bridge_free,         SETVAL, bridge_max_amnt);
    semctl(sem_id, bridge_occupied,     SETVAL, 0);
    semctl(sem_id, boat_free,           SETVAL, boat_max_amnt);
    semctl(sem_id, boat_occupied,       SETVAL, 0);
}

void passenger (uint64_t cnt, id_t sem_id)
{
    if (fork() == 0)
    {
        SemOperation get_on_board_sem[] =
        {
            {tickets_free,      -1, 0},
            {tickets_occupied,  1,0},
            {bridge_free,       -1,0},
            {bridge_occupied,   1,0},
            {boat_free,         -1,0},
            {boat_occupied,     1,0}
        };

        semop(sem_id, get_on_board_sem, 6);
        printf ("R%ld: I've bought ticket!\n", cnt);

        exit (0);
    }
}

void boat (id_t sem_id)
{
    // printf ("J: I came\n");

//     for (uint64_t cnt = 1; cnt != amnt_runners + 1; ++cnt)
//     {
//         printf("J: Where is runner?\n");
//         if (msgrcv(id, &buf, MSG_SIZE, 0, 0) < 0)
//         {
//             perror ("judge name rcv");
//         }
//         printf("J: Ok %ld/%ld\n", cnt, amnt_runners);
//
//     }
//
//
//     printf("J: Start!\n");
//
//     buf.type = 1;
//     msgsnd(id, &buf, MSG_SIZE, 0);
//
//     if (msgrcv(id, &buf, MSG_SIZE, amnt_runners + 1, 0) < 0)
//     {
//         perror ("last rcv");
//     }
//     printf ("J: FINISH\n");
}
