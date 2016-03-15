#include <cstdio>
#include <iostream>
#include <memory>
#include <sstream>
#include <unistd.h>

#include "threadinfo.hpp"

int main(int argc, char **argv)
{
    init(&argc, &argv);

    auto num_threads = get_max_threads();
    auto num_procs   = get_num_procs();

    auto threads = std::unique_ptr<thread_info[]>(
        new thread_info[num_procs*num_threads]);

    PARALLEL_REGION
    {
        int threadid = get_thread_num();
        int rank     = get_rank();
        int pos      = rank*num_threads + threadid;
        threads[pos].reinit(rank, threadid);
    }

    gather_thread_info(threads, num_procs, num_threads);
    if (get_rank() == 0) {
        for (int i = 0; i < num_procs; ++i) {
            for (int j = 0; j < num_threads; ++j) {
                std::cout << threads[i*num_threads + j] << "\n";
            }
        }
    }

    finalize();
    return 0;
}
