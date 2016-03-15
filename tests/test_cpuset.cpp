#include <gtest/gtest.h>

#include "cpuset.hpp"

TEST(cpuset, construction)
{
    {
        cpuset cpuset;
        EXPECT_EQ(0u, cpuset.size());
    }

    {
        cpuset cpuset(1);
        EXPECT_EQ(1u, cpuset.size());
        EXPECT_TRUE(cpuset.find(1));
    }

    {
        cpuset cpuset1;
        cpuset1.insert(1);

        cpuset cpuset2(cpuset1);
        EXPECT_EQ(cpuset1, cpuset2);
    }

    {
        cpuset cpuset1;
        cpuset1.insert(1);

        cpuset cpuset2 = cpuset1;
        EXPECT_EQ(cpuset1, cpuset2);
    }
}

TEST(cpuset, insertion)
{
    cpuset cpuset;

    // first try to add something above CPU_SETSIZE
    cpuset.insert(cpuset.max_cpus() + 1);
    EXPECT_EQ(0u, cpuset.size());

    cpuset.insert(1);
    cpuset.insert(2);
    cpuset.insert(3);
    EXPECT_EQ(3u, cpuset.size());
}

TEST(cpuset, find)
{
    cpuset cpuset;

    cpuset.insert(1);
    cpuset.insert(3);
    EXPECT_TRUE(cpuset.find(1));
    EXPECT_FALSE(cpuset.find(2));
    EXPECT_TRUE(cpuset.find(3));
}

TEST(cpuset, removal)
{
    cpuset cpuset;

    cpuset.insert(1);
    cpuset.insert(2);
    cpuset.insert(3);

    cpuset.remove(2);
    EXPECT_FALSE(cpuset.find(2));
}

TEST(cpuset, operators)
{
    {
        // Test AND
        cpuset cpuset1, cpuset2;

        cpuset1.insert(1);
        cpuset1.insert(2);
        cpuset2.insert(3);
        cpuset2.insert(4);

        cpuset cpuset3 = cpuset1 | cpuset2;
        cpuset1 |= cpuset2;

        EXPECT_EQ(4u, cpuset1.size());
        EXPECT_EQ(cpuset1, cpuset3);
    }

    {
        // Test OR
        cpuset cpuset1, cpuset2;

        cpuset1.insert(1);
        cpuset1.insert(2);
        cpuset2.insert(2);
        cpuset2.insert(3);

        cpuset cpuset3 = cpuset1 & cpuset2;
        cpuset1 &= cpuset2;

        EXPECT_EQ(1u, cpuset1.size());
        EXPECT_TRUE(cpuset1.find(2));
        EXPECT_EQ(cpuset1, cpuset3);
    }

    {
        // Test XOR
        cpuset cpuset1, cpuset2;

        cpuset1.insert(1);
        cpuset1.insert(2);
        cpuset2.insert(2);
        cpuset2.insert(3);

        cpuset cpuset3 = cpuset1 ^ cpuset2;
        cpuset1 ^= cpuset2;

        EXPECT_EQ(2u, cpuset1.size());
        EXPECT_FALSE(cpuset1.find(2));
        EXPECT_TRUE(cpuset1.find(1));
        EXPECT_TRUE(cpuset1.find(3));
        EXPECT_EQ(cpuset1, cpuset3);
    }
}

TEST(cpuset, set_affinity)
{
    cpuset cpuset_save = get_cpu_affinity();

    {
        // Test setting cpu affinity
        cpuset cpuset;

        cpuset.insert(1);
        EXPECT_NO_THROW(set_cpu_affinity(cpuset));
        EXPECT_EQ(1, get_current_cpu());
    }

    {
        // Test setting cpu affinity errors
        cpuset cpuset;

        cpuset.insert(150);
        try {
            set_cpu_affinity(cpuset);
            ADD_FAILURE() << "expected std::system_error\n";
        } catch (std::system_error &e) {
            EXPECT_EQ(EINVAL, e.code().value());
        }
    }

    set_cpu_affinity(cpuset_save);
}

TEST(cpuset, get_affinity)
{
    cpuset cpuset_save = get_cpu_affinity();

    {
        // Test affinity query
        cpuset cpuset_orig;

        cpuset_orig.insert(0);
        cpuset_orig.insert(1);
        EXPECT_NO_THROW(set_cpu_affinity(cpuset_orig));

        cpuset cpuset_actual = get_cpu_affinity();
        EXPECT_EQ(cpuset_orig, cpuset_actual);
    }

    set_cpu_affinity(cpuset_save);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
