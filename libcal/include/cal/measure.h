//
// Created by petr on 3.1.18.
//

#ifndef LIBCAL_MEASURE_H
#define LIBCAL_MEASURE_H

#include <chrono>
#include <functional>

template<typename TimeUnit, typename Clock = std::chrono::system_clock>
long measure(const std::function<void(void)>& fun)
{
    using namespace std::chrono;

    auto start = Clock::now();
    fun();
    auto end = Clock::now();

    return duration_cast<TimeUnit>(end - start).count();
}

#endif //LIBCAL_MEASURE_H
