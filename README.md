# affinity
A small C++ wrapper for managing Linux CPU sets and CPU affinity

## The wrapper

The CPU set wrapper resides in [cpuset.hpp](include/cpuset.hpp) and basically wraps the functionality of Linux's [CPU_SET(3)](http://man7.org/linux/man-pages/man3/CPU_SET.3.html). You can use and manipulate cpu sets in a natural way through the bitwise oeprators and use them for setting the affinity of the calling process as in the following example:

```
cpuset cpuset1, cpuset2;
cpuset1.insert(1);
cpuset1.insert(2);
cpuset2.insert(2);
cpuset2.insert(3);

cpuset1 &= cpuset2;
set_cpu_affinity(cpuset1);
```

You can then easily either print out the CPU affinity of the current process
```
std::cout << get_cpu_affinity() << "\n";
```

or programmatically query its properties
```
cpuset cpuset = get_cpu_affinity();
std::cout << cpuset.size()  << "\n";
std::cout << cpuset.find(1) << "\n";
```

## The tool

The ``affinity`` tool simply queries and prints out the affinity of the calling thread. This is useful for testing and experimenting with different CPU affinity setting options (e.g., ``GOMP_CPU_AFFINITY``, ``KMP_AFFINITY`` etc.) or for thread placement policies of different job schedulers in HPC systems. The following is the output of running the tool with two threads on two nodes of a distributed memory system:

```
Hostname: nid00012, Rank: 0, Thread: 0
    CPU affinity: [   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15]
Hostname: nid00012, Rank: 0, Thread: 1
    CPU affinity: [   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15]
Hostname: nid00013, Rank: 1, Thread: 0
    CPU affinity: [   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15]
Hostname: nid00013, Rank: 1, Thread: 1
    CPU affinity: [   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15]
```

### Compilation
You can compile three versions of the tool:

1. Serial
  * ``make OPENMP=0``
2. OpenMP only
  * ``make OPENMP=1``
3. Hybrid (MPI+OpenMP)
  * ``make CXX=mpiCC OPENMP=1 MPI=1``
  * For the hybrid version, make sure to set ``CXX`` to a valid MPI C++ compiler.

To enable debug compilation pass ``DEBUG=0`` to ``make``. Additionally, you can control compilation by setting ``CPPFLAGS``, ``CXXFLAGS`` and ``LDFLAGS`` as usual.

The default compilation is equivalent to ``make OPENMP=1 DEBUG=0``.

### Running the tool

After successful compilation an executable ``affinity`` will be created in the top-level source directory. For example, to test the CPU affinity using of GNU's OpenMP implementation, invoke it as follows:

```
$ OMP_NUM_THREADS=4 GOMP_CPU_AFFINITY=0-4 ./affinity
Hostname: localhost, Rank: 0, Thread: 0
    CPU affinity: [   0]
Hostname: localhost, Rank: 0, Thread: 1
    CPU affinity: [   1]
Hostname: localhost, Rank: 0, Thread: 2
    CPU affinity: [   2]
Hostname: localhost, Rank: 0, Thread: 3
    CPU affinity: [   3]
```

### Running the unit tests for the cpuset wrapper

To run the unit tests you will need to have installed the [Google Test](https://github.com/google/googletest) framework and set the ``GTEST_ROOT`` variable accordingly:
```
make GTEST_ROOT=/path/to/googletest
```

If the variable is not set, the tests will not be compiled. After successful compilation the tests executable will be placed inside the ``tests`` directory. You can run the unit tests as follows:
```
$ ./tests/test_cpuset
```
