#pragma once

#include <omp.h>
#include "threadinfo_base.hpp"

// parallel_impl specializations for OpenMP

template<>
struct openmp_info<true>
{
    static int get_max_threads()
    {
        return omp_get_max_threads();
    }

    static int get_thread_num()
    {
        return omp_get_thread_num();
    }
};
