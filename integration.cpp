//
// Created by kwh44 on 3/31/19.
//

#include <thread>
#include <vector>
#include <atomic>
#include "integration.h"


double function(const double &x1, const double &x2) {
    double sum_result = 0.002;
    for (int i = -2; i <= 2; ++i) {
        for (int j = -2; j <= 2; ++j) {
            sum_result += 1 / (5 * (i + 2) + j + 3 + std::pow(x1 - 16 * j, 6) + std::pow(x2 - 16 * i, 6));
        }
    }
    return 1 / sum_result;
}

void double_integral_compute(double a, const double &b, double c, const double &d, const size_t &steps,
                             std::atomic<double> &result) {
    double dx = (b - a) / steps;
    double dy = (d - c) / steps;
    double cur_result(0);
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < steps; ++j) {
            cur_result += function(a + dx / 2, c + dy / 2);
            c += dy;
        }
        a += dx;
    }
    result = result + cur_result * dx * dy;
}

double integrate(double a, const double &b, double c, const double &d, const size_t &steps, const size_t &thread_num) {
    std::atomic<double> result(0);
    std::vector<std::thread> threads_list;
    threads_list.reserve(thread_num);
    double x_range_step = (b - a) / thread_num;
    size_t steps_part = steps / thread_num;
    for (size_t i = 0; i < thread_num; ++i) {
        threads_list.emplace_back(
                double_integral_compute, a + x_range_step * i,
                a + x_range_step * (i + 1), c, d, steps_part, std::ref(result));
    }
    for (auto &v: threads_list) v.join();
    return result;
}