// shvar_op.h
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Header file for shvar_op.c module

#ifndef SHVAR_OP_H
#define SHVAR_OP_H

#include <stdio.h>
#include <sys/shm.h>            // all shm operations

// Shared variable struct for array,
// allows to call function on multiple shared variables
typedef struct shared_var
{
    key_t shmkey;               // Shared memory fragment key
    int shmid;                  // Shared memory ID
    int *valueptr;              // Pointer to value in shared variable
}shared_var;

// Initializes arr_size variables in passed array
// Returns number of successfully created variables
// Returns -1 if bad arugments are passed
int shvar_init(shared_var *variables, int arr_size);

// Detaches all shared variables in array
// Returns sum of successfully detached shared variables
void shvar_detach_all(shared_var *variables, int arr_size);

// Deletes shared variable from system
// Returns 1 if error, else 0
int shvar_delete(int shmid);

// Deletes all shared variables in array
// Returns sum of successfully deleted shared variables
int shvar_delete_all(shared_var *variables, int arr_size);

#endif /* SHVAR_OP_H */