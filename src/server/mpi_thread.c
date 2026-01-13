#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>

#include "server_threads.h"
#include "mpi_protocol.h"
#include "common.h"

void* mpi_thread_fn(void *arg)
{
    mpi_thread_args_t *args = (mpi_thread_args_t *)arg;
    job_queue_t *queue = args->queue;
    shm_region_t *shm = args->shm;

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* This thread must run ONLY on rank 0 */
    if (rank != 0) {
        return NULL;
    }

    while (1) {
        job_packet_t job;
        queue_pop(queue, &job);

        /* Broadcast job to workers */
        for (int r = 1; r < size; r++) {
            MPI_Send(&job, sizeof(job), MPI_BYTE,
                     r, MPI_JOB_TAG, MPI_COMM_WORLD);
        }

        /* Collect partial results */
        double sum = 0.0;
        for (int r = 1; r < size; r++) {
            double partial;
            MPI_Recv(&partial, 1, MPI_DOUBLE,
                     r, MPI_RESULT_TAG,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += partial;
        }

        /* Write result into shared memory */
        shm->result.job_id = job.job_id;
        shm->result.result = sum;
        shm->result_ready = 1;
    }

    return NULL;
}
