#pragma once

#include "cpuset.hpp"

#ifndef HOST_NAME_MAX
#   define HOST_NAME_MAX 255
#endif

class thread_info
{
public:
    thread_info()
        : rank_(0)
        , threadid_(0)
    {
        hostname_[0] = 0;
    }

    thread_info(int rank, int tid)
    {
        reinit(rank, tid);
    }

    void reinit(int rank, int tid)
    {
        rank_ = rank;
        threadid_ = tid;
        get_hostname();
        affinity_ = get_cpu_affinity();
    }

    int rank() const
    {
        return rank_;
    }

    int threadid() const
    {
        return threadid_;
    }

    std::string hostname() const
    {
        return std::string(hostname_);
    }

    friend bool operator==(const thread_info &lhs, const thread_info &rhs);
    friend bool operator!=(const thread_info &lhs, const thread_info &rhs);
    friend std::ostream &operator<<(std::ostream &os, const thread_info &tinfo);

private:
    void get_hostname()
    {
        if (gethostname(hostname_, HOST_NAME_MAX) < 0) {
            throw std::system_error(errno, std::system_category());
        }
    }

    int rank_;
    int threadid_;
    char hostname_[HOST_NAME_MAX];
    cpuset affinity_;
};

bool operator==(const thread_info &lhs, const thread_info &rhs)
{
    return lhs.rank_ == rhs.rank_ &&
        lhs.threadid_ == rhs.threadid_ &&
        std::string(lhs.hostname_) == std::string(rhs.hostname_) &&
        lhs.affinity_ == rhs.affinity_;
}

bool operator!=(const thread_info &lhs, const thread_info &rhs)
{
    return !(lhs == rhs);
}

std::ostream &operator<<(std::ostream &os, const thread_info &tinfo)
{
    os << "Hostname: " << tinfo.hostname_ << ", Rank: " << tinfo.rank_
       << ", Thread: " << tinfo.threadid_ << "\n"
       << "    CPU affinity: " << tinfo.affinity_;
    return os;
}

template<bool HasOpenMP>
struct openmp_info
{
    static int get_max_threads()
    {
        return 1;
    }

    static int get_thread_num()
    {
        return 0;
    }
};

template<bool HasMPI>
struct mpi_info
{
    static int get_num_procs()
    {
        return 1;
    }

    static int get_rank()
    {
        return 0;
    }

    static void init(int *argc, char ***argv)
    { }

    static void finalize()
    { }

    static void gather_thread_info(
        const std::unique_ptr<thread_info[]> &threads,
        std::size_t num_procs, std::size_t num_threads)
    {
        return;
    }
};
