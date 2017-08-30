// sem_op.c
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Source for sem_op.h module

#include "sem_op.h"

// Initializes arr_size semaphores in passed array
// Returns arr_size of successfully created semaphores
// Returns -1 if bad arugments are passed
int sem_creat(semaphore *sem_arr, int arr_size)
{
    if(sem_arr == NULL || arr_size < 0)
    {
        fprintf(stderr, "Bad arguments detected!\n");
        return -1;
    }

    int count;
    for(count = 0; count < arr_size; count++)
    {
        // Create semaphore
        sem_arr[count].semptr = sem_open(sem_arr[count].name, O_CREAT | O_EXCL, 0644, 0);
        if(sem_arr[count].semptr == SEM_FAILED)
        {
            perror("Opening semaphore error");
            break;
        }
    }
    return count;
}

// Closes all semaphores in array
// Returns sum of successfully closed semaphores
void sem_close_all(semaphore *sem_arr, int arr_size)
{
    for(int i = 0; i < arr_size; i++)
        sem_close(sem_arr[i].semptr);
}

// Unlinks all semaphores in array
// Returns sum of successfully unlinked semaphores
void sem_unlink_all(semaphore *sem_arr, int arr_size)
{
    for(int i = 0; i < arr_size; i++)
        sem_unlink(sem_arr[i].name);
}
