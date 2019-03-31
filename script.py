#!/usr/bin/python3.7
import sys
import subprocess
import os

programs = ["main"]

if os.name == "nt":
    for i in range(len(programs)):
        programs[i] = programs[i] + ".exe."

for root, directories, file_list in os.walk('.'):
    if all(i in file_list for i in programs):
        for i in range(len(programs)):
            if os.name == "nt":
                programs[i] = root + '\\' + programs[i]
            else:
                programs[i] = root + '/' + programs[i]
        break

if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("####    script.py    ####\n"
              "How to use:\n"
              "$ ./script.py config_file num_iter rae\n"
              "num_iter - number of iterations for each version to perform\n"
              "rae - value of error allowed for different launches to have")
        exit(0)
    if len(sys.argv) > 4:
        raise ValueError("Wrong amount of args")
    config_file = sys.argv[1]
    number_of_iterations = int(sys.argv[2])
    error_allowed = float(sys.argv[3])
    min_par_time = float('inf')
    previous_result = None
    for experiment in range(number_of_iterations):
        res_parallel = subprocess.run([programs[0], config_file], shell=False, stdout=subprocess.PIPE)
        parallel_output = res_parallel.stdout.decode().strip().split("\n")
        par_result = float(parallel_output[0].split(" ")[1])
        par_time = int(parallel_output[2].split(" ")[1])
        if (previous_result == None):
            previous_result = par_result
        if abs(par_result - previous_result) > error_allowed:
            print("Error: passed allowed error margin:\n prev: {}; curr: {}".format(previous_result, par_result))
            exit(5)
        if par_time < min_par_time:
            min_par_time = par_time
    print("The minimal time of execution is {} microseconds.".format(min_par_time))