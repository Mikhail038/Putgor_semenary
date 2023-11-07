//=========================================================================

#include "monitor.hpp"

namespace Threads
{
    Monitor::Monitor()
    {
        pthread_mutex_init(&mutex_, NULL);

        pthread_cond_init(&son_wait_    , NULL);
        pthread_cond_init(&father_wait_ , NULL);
    }

    Monitor::~Monitor()
    {
        pthread_mutex_destroy(&mutex_);

        pthread_cond_destroy(&son_wait_);
        pthread_cond_destroy(&father_wait_);
    }
}
