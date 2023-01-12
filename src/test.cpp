#include "headers/new_array.hpp"
#include <iostream>
#include <vector>
#include <alloca.h>
#include <memory_resource>
#include <memory>
#include <cstring>
#include <algorithm>
#include <stdlib.h>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <chrono>
#include <bitset>
#include <thread>
#include <memory_resource>
#include <atomic>
// super fast and optimized array manipulation functions
// inspired by the incredible std::vector, but way faster
// and using a power of two instead of a generic size
std::chrono::milliseconds get_epoch()
{
    return (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()));
}
std::atomic<bool> sw = false;
std::atomic<bool> test_done = false;
std::atomic<bool> standard_done = false;
void test_my_array()
{
    auto start = get_epoch();
check:
    while (!sw.load())
        goto check;
    new_array<new_array<double>> test(INT32_MAX / 32);
    for (auto i = 0; i < INT32_MAX / 32; i++)
    {
        test[i] = {1.};
    }

    std::cout << test[0][0] << std::endl; // 0.001
    test[0] = {0.25};
    std::cout << test[0][0] << std::endl; // 0.002

    auto end = get_epoch() - start;
    std::cout << "finished my vec in: " << end << "ms" << std::endl;
    test_done.store(true);
}

void test_std_array()
{
    auto start = get_epoch();
check:
    while (!sw.load())
        goto check;

    std::vector<std::vector<double>> test{};
    for (auto i = 0ul; i < INT32_MAX / 32; i++)
    {
        test.push_back({1.});
    }

    std::cout << test[0][0] << std::endl; // 0.001
    test[0] = {0.25};
    std::cout << test[0][0] << std::endl; // 0.002

    auto end = get_epoch() - start;

    std::cout << "finished std vec in: " << end << "ms" << std::endl;
    standard_done.store(true);
}

int main()
{
    sw.store(true);
    std::thread{test_std_array}.detach(); // 0.012s on UINT64
    std::thread{test_my_array}.detach();  // 0.008s on UINT64
    // sw.store(true);

    while(!standard_done.load() || !test_done.load())
        std::this_thread::sleep_for(std::chrono::microseconds{1});
}
