#pragma once

#include <cstddef>
#include <pthread.h>

namespace Threads
{
    struct Monitor
    {
        pthread_mutex_t mutex_;

        pthread_cond_t son_wait_;
        pthread_cond_t father_wait_;

        //=======================================================================================================

        Monitor();
        ~Monitor();

        //=======================================================================================================

        void put_brick ();
    };
}
