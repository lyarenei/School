// proj2.h
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Header file for proj2 program

#include <stdio.h>
#include <stdlib.h>             // exit, rand, srand
#include <unistd.h>             // usleep, fork
#include <sys/wait.h>           // wait
#include <errno.h>              // errno, perror
#include <string.h>             // strlen, strcpy
#include <ctype.h>              // isdigit
#include <stdarg.h>             // va_start, va_end
#include <time.h>               // time
#include <signal.h>             // signal catching and handling

#include "shvar_op.h"           // Shared variable operations
#include "sem_op.h"             // Semaphore operations

// Prints help info to stderr
void print_help(void);

// Checks line for numbers and converts it to integer value (unsigned only)
// Returns converted value, or -1 if any error
int number_check(char *line);

// Checks argv for numbers and converts them
// Stores output to arg_arr
// On success - returns 0
// On error - returns 1
int check_nums(int *arg_arr, char **argv, int count);

// Checks number ranges in arg_arr
// On success - returns 0
// On error - returns 1
int check_num_range(int *arg_arr, int count);

// Prints line to output_file and increments line_number
void print_msg(FILE *output_file, int *line_number, const char *line, ...);

// Sleeps caller for random time between 0 and max_rand
void random_time_sleep(int max_rand);

// Spawns count processes from caller between 0 and max_spawn_time time
void spawn_processes(int count, int max_spawn_time);

// Spawner duty function - just wait for its child processes to finish
void gen_duty(semaphore *sem_arr, int gen_process_num);

// Adult duty function - synchronizes with children in the care centre
void adult_duty(int *arg_arr, shared_var *variables, semaphore *sem_arr);

// Child duty function - synchronizes with adults in the care centre
void child_duty(int *arg_arr, shared_var *variables, semaphore *sem_arr);

// Signal handler after creating shared variables
void sig_exit_shvar(int signal);

// Signal handler after creating semaphores
void sig_exit_sem(int signal);

// Signal handler for spawners
void sig_exit_spawner(int signal);

// Signal handler for adults and children
void sig_exit_duty(int signal);
