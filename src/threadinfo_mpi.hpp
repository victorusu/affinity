#pragma once

#include <mpi.h>
#include "threadinfo.hpp"

// mpi_info specializations for MPI

template<>
struct mpi_info<true>
{
    static int get_num_procs()
    {
        int ret;
        MPI_Comm_size(MPI_COMM_WORLD, &ret);
        return ret;
    }

    static int get_rank()
    {
        int ret;
        MPI_Comm_rank(MPI_COMM_WORLD, &ret);
        return ret;
    }

    static void init(int *argc, char ***argv)
    {
        MPI_Init(argc, argv);
    }

    static void finalize()
    {
        MPI_Finalize();
    }

    static void gather_thread_info(
        const std::unique_ptr<thread_info[]> &threads,
        std::size_t num_procs, std::size_t num_threads)
    {
        auto offset     = get_rank()*num_threads;
        auto sendbuff   = static_cast<void *>(&threads[offset]);
        auto send_bytes = num_threads*sizeof(thread_info);
        if (get_rank() == 0) {
            MPI_Gather(MPI_IN_PLACE, send_bytes, MPI_CHAR,
                       threads.get(), send_bytes, MPI_CHAR, 0, MPI_COMM_WORLD);

        } else {
            MPI_Gather(sendbuff, send_bytes, MPI_CHAR,
                       threads.get(), send_bytes, MPI_CHAR, 0, MPI_COMM_WORLD);
        }
    }
};
