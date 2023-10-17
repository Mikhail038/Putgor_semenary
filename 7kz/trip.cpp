#include <climits>
#include <cstddef>
#include <cstdio>
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
    bridge_free         = 0,
    bridge_occupied     = 1,
    boat_free           = 2,
    boat_occupied       = 3,
    tickets_free        = 4,
    tickets_occupied    = 5,
    land_occupied       = 6,
    boat_on_trip        = 7,
    boat_ready_for_next_trip = 8
};

#define N_SEMS 6

const uint64_t ROUNDS = 3;

const uint64_t TIME_PER_TRIP    = 1000000;
const uint64_t TIME_PER_BRIDGE  = 1000000;

const int passengers_amnt  = 5;
const int bridge_max_amnt  = 2;
const int boat_max_amnt    = 4;

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
    id_t sem_id = semget (IPC_PRIVATE, 9, S_IRWXU | S_IRWXG | S_IRWXO);// | IPC_CREAT);
    semaphore_init(sem_id);

    for (uint64_t cnt = 0; cnt != passengers_amnt; ++cnt)
    {
        passenger(cnt, sem_id);
    }

    boat (sem_id);

    semctl (sem_id, 0, IPC_RMID);

    for (uint64_t cnt = 0; cnt != passengers_amnt; ++cnt)
    {
        wait (NULL);
    }
}

void semaphore_init (id_t sem_id)
{
    semctl(sem_id, tickets_free,        SETVAL, boat_max_amnt);
    semctl(sem_id, tickets_occupied,    SETVAL, 0);
    semctl(sem_id, bridge_free,         SETVAL, bridge_max_amnt);
    semctl(sem_id, bridge_occupied,     SETVAL, 0);
    semctl(sem_id, boat_free,           SETVAL, boat_max_amnt);
    semctl(sem_id, boat_occupied,       SETVAL, 0);
    semctl(sem_id, boat_on_trip,       SETVAL,  1);
    semctl(sem_id, boat_ready_for_next_trip,       SETVAL, 0);
}

void passenger (uint64_t cnt, id_t sem_id)
{
    if (fork() == 0)
    {
        while (true)
        {
            SemOperation buy_ticket[] =
            {
                {boat_ready_for_next_trip, 0, 0},
                {tickets_free,      -1, 0},
                {tickets_occupied,  1,0},
            };
            printf ("P[%ld]: Try to buy a ticket    >0>0>0>0\n", cnt);
            if (semop(sem_id, buy_ticket, 3) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: I've bought ticket!    +0+0+0+0\n", cnt);

            SemOperation get_on_bridge[] =
            {
                {bridge_free,       -1,0},
                {bridge_occupied,   1,0}
            };
            printf ("P[%ld]: Try to get on a bridge >1>1>1>1\n", cnt);
            if (semop(sem_id, get_on_bridge, 2) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: I've entered a bridge! +1+1+1+1\n", cnt);

            usleep (TIME_PER_BRIDGE);

            SemOperation get_on_boat[] =
            {
                {bridge_free,       1,0},
                {bridge_occupied,   -1,0},
                {boat_free,         -1,0},
                {boat_occupied,     1,0}
            };
            printf ("P[%ld]: Try to get on a boat   >2>2>2>2\n", cnt);
            if (semop(sem_id, get_on_boat, 4) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: I've entered a boat    2+2+2+2\n", cnt);


            SemOperation wait_trip[] =
            {
                {boat_on_trip, 0, 0},
            };
            printf ("P[%ld]: Wait trip start             =3=3=3=3\n", cnt);
            if (semop(sem_id,  wait_trip, 1) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: Wait trip finished          =3=3=3=3\n", cnt);


            printf ("P[%ld]: Try to get on a bridge <4<4<4<4\n", cnt);
            if (semop(sem_id, get_on_bridge, 2) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: I've entered a bridge! -4-4-4-4\n", cnt);

            SemOperation get_on_land[] =
            {
                {bridge_free,       1,0},
                {bridge_occupied,   -1,0},
                {boat_occupied,  -1,0},
                {boat_free,  1,0},
                {tickets_free, 1, 0},
                {tickets_occupied, -1, 0}
            };
            printf ("P[%ld]: Try to get on land     <5<5<5<5\n", cnt);
            if (semop(sem_id, get_on_land, 6) != 0)
            {
                // perror ("");
                exit (0);
            }
            printf ("P[%ld]: I've got on land!      -5-5-5-5\n", cnt);

            // SemOperation wait_next[] =
            // {
            // };
            // printf ("P[%ld]: Wait start             =6=6=6=6\n", cnt);
            // semop(sem_id,  wait_next, 1);
            // printf ("P[%ld]: Wait finished          =6=6=6=6\n", cnt);
        }

        exit (0);
    }
}

void boat (id_t sem_id)
{
    uint64_t cnt = 0;
    while (cnt < ROUNDS)
    {
        printf ("B: Boat is waiting passengers\n");
        SemOperation trip_start[] =
        {
            {bridge_occupied, 0, 0},
            {tickets_free, 0, 0},
            {boat_free, 0, 0},
            {boat_ready_for_next_trip, 1, 0},
            {boat_on_trip, -1, 0}
        };
        semop(sem_id, trip_start, 5);
        printf ("B: Trip started\n");

        usleep (TIME_PER_TRIP);

        SemOperation trip_end[] =
        {
            {boat_on_trip, 1, 0},
        };
        semop(sem_id, trip_end, 1);
        printf ("B: Trip ended\n");

        SemOperation wait_landing[] =
        {
            {bridge_occupied, 0,0},
            {tickets_occupied, 0,0},
            {boat_occupied, 0, 0},
            {boat_ready_for_next_trip, -1, 0}
        };
        semop (sem_id, wait_landing, 4);
        printf ("Next tickets selling opens\n");

        ++cnt;
    }
}
