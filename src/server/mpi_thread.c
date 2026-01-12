#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include "server_threads.h"
#include "mpi_protocol.h"

void* mpi_thread_fn(void *arg)
{
    mpi_thread_args_t *args = (mpi_thread_args_t *)arg;
    job_queue_t *queue = args->queue;
    shm_region_t *shm = args->shm;

    while (1) {
        job_packet_t job;
        queue_pop(queue, &job);

        shm->job = job;
        shm->job_ready = 1;

        MPI_Send(&job, sizeof(job), MPI_BYTE,
                 0, MPI_JOB_TAG, MPI_COMM_WORLD);

        MPI_Recv(&shm->result, sizeof(result_packet_t), MPI_BYTE,
                 0, MPI_RESULT_TAG,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        shm->result_ready = 1;
    }
    return NULL;
}
