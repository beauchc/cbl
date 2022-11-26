#include <cbl/adt/priority_queue.h>

#include <gtest/gtest.h>

//------------------------------------------------------------------------------
//
TEST(priority_queue, simple) {
    cbl::PriorityQueue<unsigned> queue;

    auto idx5 = queue.push(5);
    ASSERT_EQ(0, idx5);
    ASSERT_EQ(5, queue[idx5]);
    ASSERT_EQ(1, queue.size());
    ASSERT_TRUE(queue.valid(idx5));
    ASSERT_EQ(5, queue.top());

    auto idx4 = queue.push(4);
    ASSERT_EQ(1, idx4);
    ASSERT_EQ(4, queue[idx4]);
    ASSERT_EQ(2, queue.size());
    ASSERT_TRUE(queue.valid(idx4));
    ASSERT_TRUE(queue.valid(idx5));
    ASSERT_EQ(5, queue.top());

    auto idx6 = queue.push(6);
    ASSERT_EQ(2, idx6);
    ASSERT_EQ(3, queue.size());
    ASSERT_TRUE(queue.valid(idx4));
    ASSERT_TRUE(queue.valid(idx5));
    ASSERT_TRUE(queue.valid(idx6));
    ASSERT_EQ(6, queue.top());

    queue.pop();
    ASSERT_EQ(2, queue.size());
    ASSERT_EQ(5, queue.top());
    ASSERT_TRUE(queue.valid(idx4));
    ASSERT_TRUE(queue.valid(idx5));
    ASSERT_FALSE(queue.valid(idx6));
}

//------------------------------------------------------------------------------
//
TEST(priority_queue, push_update_update_pop_twice) {
    constexpr unsigned N = 100;
    cbl::PriorityQueue<unsigned> queue;

    for (unsigned k = 0; k < 2; ++k) {
        for (unsigned i = 0; i < N; ++i) {
            ASSERT_EQ(i, queue.push(i));
            ASSERT_EQ(i + 1, queue.size());
            ASSERT_EQ(i, queue.top());

            for (unsigned j = 0; j <= i; ++j) {
                ASSERT_TRUE(queue.valid(j));
                ASSERT_EQ(j, queue[j]);
            }
            for (unsigned j = i + 1; j < N; ++j) {
                ASSERT_FALSE(queue.valid(j));
            }
        }
        for (unsigned i = 0; i < N; ++i) {
            queue.update(i, i + N);
            ASSERT_EQ(N, queue.size());
            ASSERT_EQ(i + N, queue.top());

            for (unsigned j = 0; j < N; ++j) {
                auto expected = j <= i ? j + N : j;
                ASSERT_TRUE(queue.valid(j));
                ASSERT_EQ(expected, queue[j]);
            }
        }
        for (unsigned i = 0; i < N; ++i) {
            auto expected_top = (i == N - 1) ? (N - 1) : (2 * N - 1);

            queue.update(i, i);
            ASSERT_EQ(N, queue.size());
            ASSERT_EQ(expected_top, queue.top()) << i;

            for (unsigned j = 0; j < N; ++j) {
                auto expected = j <= i ? j : j + N;
                ASSERT_TRUE(queue.valid(j));
                ASSERT_EQ(expected, queue[j]);
            }
        }
        for (unsigned i = 0; i < N; ++i) {
            auto n = N - i - 1;
            ASSERT_EQ(n, queue.top());
            queue.pop();

            ASSERT_EQ(n, queue.size());
            for (unsigned j = 0; j < n; ++j) {
                ASSERT_TRUE(queue.valid(j));
                ASSERT_EQ(j, queue[j]);
            }
            for (unsigned j = n; j < N; ++j) {
                ASSERT_FALSE(queue.valid(j));
            }
        }
    }
    ASSERT_TRUE(queue.empty());
}
