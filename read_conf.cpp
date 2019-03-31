//
// Created by kwh44 on 3/31/19.
//

#include "read_conf.h"
#include <exception>
#include <stdexcept>
#include <string>

configuration_t read_conf(std::istream &cf) {
    std::ios::fmtflags flags(cf.flags()); // Save stream state
    cf.exceptions(std::ifstream::failbit); // Enable exception on fail
    configuration_t res;
    std::string temp;
    try {
        cf >> res.rel_err;
        getline(cf, temp); // Відкидаємо комент.
        cf >> res.abs_err;
        getline(cf, temp);
        cf >> res.x1;
        getline(cf, temp);
        cf >> res.x2;
        getline(cf, temp);
        cf >> res.y1;
        getline(cf, temp);
        cf >> res.y2;
        getline(cf, temp);
        cf >> res.initial_steps;
        getline(cf, temp);
        cf >> res.max_steps;
        getline(cf, temp);
        cf >> res.thread_num;
    } catch (std::ios_base::failure &fail) // Life without RAII is hard...
    {
        cf.flags(flags);
        throw; // re-throw exception
    }
    cf.flags(flags);
    if (res.x2 <= res.x1) {
        throw std::runtime_error("x1 should be <= x2");
    }
    if (res.y2 <= res.y1) {
        throw std::runtime_error("y1 should be <= y2");
    }
    if (res.initial_steps < 10) {
        throw std::runtime_error("Too few initial_steps");
    }
    if (res.max_steps < 10) {
        throw std::runtime_error("Too small max_steps");
    }
    if (res.thread_num <= 0) {
        throw std::runtime_error("Number of threads to use is <= 0: Please, think!");
    }
    if (res.abs_err <= 0 || res.rel_err <= 0) {
        throw std::runtime_error("Errors should be positive");
    }
    return res;
}
