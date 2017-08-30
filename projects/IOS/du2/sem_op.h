// sem_op.h
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Header file for sem_op.c module

#ifndef SEM_OP_H
#define SEM_OP_H

#include <stdio.h>
#include <semaphore.h>          // all semaphores operations
#include <fcntl.h>              // O_CREAT, O_EXEC

// Semaphore struct for array
// allows to call function on multiple semaphores
typedef struct semaphore
{
    sem_t *semptr;              // Pointer to semaphore
    char name[15];              // Semaphore name
}semaphore;

// Initializes arr_size semaphores in passed array
// Returns arr_size of successfully created semaphores
// Returns -1 if bad arugments are passed
int sem_creat(semaphore *sem_arr, int arr_size);

// Closes all semaphores in array
// Returns sum of successfully closed semaphores
void sem_close_all(semaphore *sem_arr, int arr_size);

// Unlinks all semaphores in array
// Returns sum of successfully unlinked semaphores
void sem_unlink_all(semaphore *sem_arr, int arr_size);

#endif /* SEM_OP_H */