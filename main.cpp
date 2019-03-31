#include <iostream>
#include "read_conf.h"
#include "measure_time.h"
#include "integration.h"

int main(int argc, char **argv) {
    std::string filename("./../conf.txt");
    if (argc == 2)
        filename = argv[1];
    if (argc > 2) {
        std::cerr << "Too many arguments. Usage: \n"
                     "<program>\n"
                     "or\n"
                     "<program> <config-filename>\n" << std::endl;
        return 1;
    }
    std::ifstream config_stream(filename);
    if (!config_stream.is_open()) {
        std::cerr << "Failed to open configuration file " << filename << std::endl;
        return 2;
    }
    configuration_t config;
    try {
        config = read_conf(config_stream);
    } catch (std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 3;
    }
    auto before = get_current_time_fenced();
    size_t steps = config.initial_steps;
    double cur_res = integrate(config.x1, config.x2, config.y1, config.y2, steps, config.thread_num);
    std::cout << "The cur_res is: " << cur_res << std::endl;
    exit(66);
    double prev_res = cur_res;
    bool to_continue = true;
    double abs_err = -1; // Just guard value
    double rel_err = -1; // Just guard value
// #define PRINT_INTERMEDIATE_STEPS
    while (to_continue) {
#ifdef PRINT_INTERMEDIATE_STEPS
        cout << cur_res << " : " << steps << " steps" << endl;
#endif
        prev_res = cur_res;
        steps *= 2;
        cur_res = integrate(config.x1, config.x2, config.y1, config.y2, steps, config.thread_num);
        abs_err = std::fabs(cur_res - prev_res);
        rel_err = std::fabs((cur_res - prev_res) / cur_res);
#ifdef PRINT_INTERMEDIATE_STEPS
        cout << '\t' << "Abs err : rel err " << abs_err << " : " << rel_err << endl;
#endif
        to_continue = (abs_err > config.abs_err);
        to_continue = to_continue && (rel_err > config.rel_err);
        to_continue = to_continue && (steps < config.max_steps);
    }
    auto time_to_calculate = get_current_time_fenced() - before;
    std::cout.precision(10);
    std::cout << "Result: " << cur_res << std::endl;
    std::cout << "Abs err : rel err " << abs_err << " : " << rel_err << std::endl;
    std::cout << "Time: " << to_us(time_to_calculate) << std::endl;
    return 0;
}