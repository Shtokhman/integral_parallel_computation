//
// Created by kwh44 on 3/31/19.
//

#ifndef INTEGRAL_PARALLEL_COMPUTATION_READ_CONF_H
#define INTEGRAL_PARALLEL_COMPUTATION_READ_CONF_H

#include <stddef.h>
#include <fstream>
#include <iostream>

struct configuration_t {
    double rel_err, abs_err;
    double x1, x2;
    double y1, y2;
    size_t initial_steps;
    size_t max_iters;
    size_t thread_num;
};

configuration_t read_conf(std::istream &cf);

#endif //INTEGRAL_PARALLEL_COMPUTATION_READ_CONF_H
