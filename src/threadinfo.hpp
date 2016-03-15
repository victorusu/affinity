#pragma once

#include "threadinfo_base.hpp"

#ifdef _OPENMP
#   define PARALLEL_REGION  _Pragma("omp parallel")
#   define HAVE_OPENMP 1
#   include "threadinfo_omp.hpp"
#else
#   define PARALLEL_REGION
#   define HAVE_OPENMP 0
#endif

#ifdef USE_MPI
#   define HAVE_MPI 1
#   include "threadinfo_mpi.hpp"
#else
#   define HAVE_MPI 0
#endif

#include "threadinfo_api.hpp"
