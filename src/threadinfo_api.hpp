#pragma once

#include "threadinfo_base.hpp"

static int get_max_threads()
{
    return openmp_info<HAVE_OPENMP>::get_max_threads();
}

static int get_thread_num()
{
    return openmp_info<HAVE_OPENMP>::get_thread_num();
}

static int get_num_procs()
{
    return mpi_info<HAVE_MPI>::get_num_procs();
}

static int get_rank()
{
    return mpi_info<HAVE_MPI>::get_rank();
}

static void init(int *argc, char ***argv)
{
    return mpi_info<HAVE_MPI>::init(argc, argv);
}

static void finalize()
{
    return mpi_info<HAVE_MPI>::finalize();
}

static void gather_thread_info(const std::unique_ptr<thread_info[]> &threads,
                               std::size_t num_procs, std::size_t num_threads)
{
    return mpi_info<HAVE_MPI>::gather_thread_info(threads,
                                                  num_procs, num_threads);
}
