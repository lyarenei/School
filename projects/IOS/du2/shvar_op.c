// shvar_op.c
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Source for shvar_op.h module

#include "shvar_op.h"

// Initializes arr_size variables in passed array
// Returns number of successfully created variables
// Returns -1 if bad arugments are passed
int shvar_init(shared_var *variables, int arr_size)
{
    if(variables == NULL || arr_size < 0)
    {
       fprintf(stderr, "Bad arguments detected!\n");
        return -1;
    }

    int count;
    for(count = 0; count < arr_size; count++)
    {
        // Create key
        variables[count].shmkey = ftok("/dev/null", (count+1));
        if(variables[count].shmkey < 0)
        {
            perror("Creating ftok key");
            break;
        }
        
        // Alloc space
        variables[count].shmid = shmget(variables[count].shmkey, sizeof(int), 0644 | IPC_CREAT);
        if(variables[count].shmid < 0)
        {
            perror("Shmget allocating");
            break;
        }

        // Attach pointers to allocated space
        variables[count].valueptr = (int *)shmat(variables[count].shmid, NULL, 0);
        if(variables[count].valueptr == (int *)(-1))
        {
            perror("Shmat mapping");
            break;
        }
    }
    return count;
}

// Detaches all shared variables in array
void shvar_detach_all(shared_var *variables, int arr_size)
{
    for(int i = 0; i < arr_size; i++)
        shmdt(variables[i].valueptr);
}

// Deletes shared variable from system
// Returns 1 if error, else 0
int shvar_delete(int shmid)
{
    int error = 0;
    if(shmctl(shmid, IPC_RMID, 0) < 0)
    {
        perror("Shmctl deleting");
        error = 1;
    }
    return error;
}

// Deletes all shared variables in array
// Returns sum of successfully deleted shared variables
int shvar_delete_all(shared_var *variables, int arr_size)
{
    int count;
    for(count = 0; count < arr_size; count++)
        if(shvar_delete(variables[count].shmid) == 1)
            break;
    
    return count;
}
