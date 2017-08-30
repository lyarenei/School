// proj2.c
// Author: Dominik Křivohlávek
// Login: xkrivo02
// Program uses modified child care problem to synchronize forked processes
// and writes output to proj2.out file

#include "proj2.h"

// Sum of semaphores
#define SEM_COUNT 6

// Sum of shared variables
#define SHARED_COUNT 9

// Array of shared variables
shared_var variables[SHARED_COUNT];

// Array of semaphores
semaphore sem_arr[SEM_COUNT];

// FILE pointer to output file
FILE *output;

// Prints help info to stderr
void print_help(void)
{
    fprintf(stderr,"\n./proj2 A C AGT CGT AWT CWT\n\nWhere:\n\
    A = number of adults to be generated (must be > 0)\n\
    C = number of children to be generated (must be > 0)\n\
    AGT = maximum time (in ms) for new adult process to be generated <0;5000>\n\
    CGT = maximum time (in ms) for new child process to be generated <0;5000>\n\
    AWT = maximum time (in ms) for adult process to simulate activity <0;5000>\n\
    CWT = maximum time (in ms) for child process to simulate activity <0;5000>\n");
}

// Checks line for numbers and converts it to integer value (unsigned only)
// Returns converted value, or -1 if any error
int number_check(char *line)
{
    int len = strlen(line);

    // Check passed line for numbers
    for (int i = 0; i < len; i++)
    {
        if (!isdigit(line[i]))
        {
            fprintf(stderr,"Line contains non integers!\n");
            return -1;
        }
    }

    // Convert the char numbers to int
    int value = 0;
    if(!sscanf(line, "%d", &value))
    {
        fprintf(stderr,"Failed to convert line to integer!\n");
        return -1;
    }
    return value;
}

// Checks argv for numbers and converts them
// Stores output to arg_arr
// On success - returns 0
// On error - returns 1
int check_nums(int *arg_arr, char **argv, int count)
{
    // Convert array of char numbers into interger values
    for(int i = 1; i < count; i++)
    {
        arg_arr[i-1] = number_check(argv[i]);
        if(arg_arr[i-1] < 0)
        {
            fprintf(stderr,"Bad argument range detected!\n");
            return 1;
        }    
    }
    return 0;
}

// Checks number ranges in arg_arr
// On success - returns 0
// On error - returns 1
int check_num_range(int *arg_arr, int count)
{
    // Adults and children must be > 0
    if(!arg_arr[0] || !arg_arr[1])
    {
        fprintf(stderr,"Bad argument range detected!\n");
        return 1;
    }

    // Times must be < 5001
    for(int i = 2; i < count-1; i++)
    {
        if(arg_arr[i] > 5000)
        {
            fprintf(stderr,"Bad argument range detected!\n");
            return 1;
        }    
    }
    return 0;
}

// Prints line to output_file and increments line_number
void print_msg(FILE *output_file, int *line_number, const char *line, ...)
{
    va_list args;
    va_start (args, line);
    fprintf(output_file, "%d\t: ",*line_number);
    vfprintf(output_file, line, args);
    va_end (args);
    *line_number += 1;
}

// Sleeps caller for random time between 0 and max_rand
void random_time_sleep(int max_rand)
{
    if(max_rand > 0)
        usleep(rand() % max_rand);
}

// Spawns count processes from caller between 0 and max_spawn_time time
void spawn_processes(int count, int max_spawn_time)
{
    for (int i = 0; i < count; i++)
        {
            // Sleep for random time
            random_time_sleep(max_spawn_time);

            pid_t pid = fork ();
            if(pid < 0)
            {
                perror("Error when forking children");
                kill(0, SIGUSR1);
                break;
            }
            // Break spawned process from loop, so it won't fork again
            else if (pid == 0)
                break;
        }
}

// Spawner duty function - just wait for its child processes to finish
void gen_duty(semaphore *sem_arr, int gen_process_num)
{
    // Sem overview
    sem_t *spawn_sem = sem_arr[2].semptr;
    sem_t *finish_sem = sem_arr[3].semptr;

    // As soon as it is unblocked by last process
    // it allows the processes to finish
    sem_wait(spawn_sem);
        for (int i = 0; i < gen_process_num; i++)
            sem_post(finish_sem);
    sem_post(spawn_sem);

    // Wait for all child processes to finish
    for(int i = 0; i < gen_process_num; i++)
        wait(NULL);
}

// Adult duty function - synchronizes with children in the care centre
void adult_duty(int *arg_arr, shared_var *variables, semaphore *sem_arr)
{
    // Number of adults and children from arguments
    int adult = arg_arr[0];
    int child = arg_arr[1];

    // Rand_max for activity simulation
    int awt = arg_arr[4];

    // Output line numbering
    int *line_num = variables[0].valueptr;

    // Child related shared variables
    int *child_sum = variables[1].valueptr;         // Sum of children in centre
    int *child_out = variables[2].valueptr;         // Sum of children which already left
    int *adult_id_pool = variables[7].valueptr;     // ID pool for child processes
    int *child_wait = variables[4].valueptr;        // Child waiting sum
    
    // Adult related shared variables
    int *adult_sum = variables[5].valueptr;         // Sum of adults in centre
    int *adult_out = variables[6].valueptr;         // Sum of adults which already left
    int *adult_wait = variables[8].valueptr;        // Adult waiting sum

    sem_t *stdout_sem = sem_arr[0].semptr;          // Controls exclusive access to output
    sem_t *traffic_sem = sem_arr[1].semptr;         // Controls traffic from/to centre
    sem_t *spawner_sem = sem_arr[2].semptr;       // Holds spawner processes before finishing 
    sem_t *finish_sem = sem_arr[3].semptr;          // Holds all processes from centre

    sem_t *c_wait_q_sem = sem_arr[4].semptr;        // Holds waiting child to enter
    sem_t *a_wait_q_sem = sem_arr[5].semptr;        // Holds waiting adult to leave

    // Adult start section
    sem_wait(traffic_sem);

        // Get adult ID
        int adult_id = *adult_id_pool;

        sem_wait(stdout_sem);
            print_msg(output, line_num,"A %d\t: started\n", adult_id);
        sem_post(stdout_sem);

        // Increment ID usage
        *adult_id_pool += 1;

    sem_post(traffic_sem);

    // Adult enter section
    sem_wait(traffic_sem);

    sem_wait(stdout_sem);
        print_msg(output, line_num,"A %d\t: enter\n", adult_id);
    sem_post(stdout_sem);
    
    *adult_sum += 1;

    // Signal children if they are waiting
    if((*child_wait) != 0)
    {
        int i = 0;
        for(i = 0; (i < (*child_wait)) && (i < 3); i++)
        {
            
            sem_post(c_wait_q_sem);
        }
        *child_sum += i;
        *child_wait -= i;
    }
    sem_post(traffic_sem);

    // Activity simulation
    random_time_sleep(awt);
    
    // Adult leaving section
    sem_wait(traffic_sem);

    sem_wait(stdout_sem);
        print_msg(output, line_num,"A %d\t: trying to leave\n", adult_id);

    // Check if leaving adult won't violate rules
    if((*child_sum) <= (((*adult_sum)-1)*30))
    {
        // Adult leaves
        print_msg(output, line_num,"A %d\t: leave\n", adult_id);
        *adult_sum -= 1;
        *adult_out += 1;
        sem_post(stdout_sem);
        // If this was last adult ever, unlock semaphore for remaining children
        if((*adult_out) == adult)
        {
            for(int i = 0; i < (child - ((*child_sum)+(*child_out))); i++)
            {
                sem_post(c_wait_q_sem);
                if((*child_wait) != 0)
                    *child_wait -= 1;

                *child_sum += 1;
            }
        }

        //Signal spawners all processes are ready to finish
        if(((*child_out) == child) && ((*adult_out) == adult))
        sem_post(spawner_sem);

        sem_post(traffic_sem);
    }
    else
    {
        // Adult must wait
        print_msg(output, line_num,"A %d\t: waiting: %d: %d\n", adult_id, *adult_sum, *child_sum);
        *adult_wait += 1;
        sem_post(stdout_sem);
        sem_post(traffic_sem);

        sem_wait(a_wait_q_sem);
        sem_wait(stdout_sem);
            print_msg(output, line_num,"A %d\t: leave\n", adult_id);
        sem_post(stdout_sem);

        // If this was last adult ever, unlock semaphore for remaining children
        if((*adult_out) == adult)
        {
            for(int i = 0; i < (child - ((*child_sum)+(*child_out))); i++)
            {
                sem_post(c_wait_q_sem);
                if((*child_wait) != 0)
                    *child_wait -= 1;
                
                *child_sum += 1;
            }
        }

        //Signal spawners all processes are ready to finish
        if(((*child_out) == child) && ((*adult_out) == adult))
            sem_post(spawner_sem);
    }

    // Wait for all to exit (signalled by spawners)
    sem_wait(finish_sem);
        sem_wait(stdout_sem);
            print_msg(output, line_num,"A %d\t: finished\n", adult_id);
        sem_post(stdout_sem);
    sem_post(finish_sem);
}

// Child duty function - synchronizes with adults in the care centre
void child_duty(int *arg_arr, shared_var *variables, semaphore *sem_arr)
{
    // Number of adults and children to generate
    int adult = arg_arr[0];
    int child = arg_arr[1];

    // Rand_max for activity simulation
    int cwt = arg_arr[5];

    // Output line numbering
    int *line_num = variables[0].valueptr;

    // Child related shared variables
    int *child_sum = variables[1].valueptr;         // Sum of children in centre
    int *child_out = variables[2].valueptr;         // Sum of children which already left
    int *child_id_pool = variables[3].valueptr;     // ID pool for child processes
    int *child_wait = variables[4].valueptr;        // Child waiting sum
    
    // Adult related shared variables
    int *adult_sum = variables[5].valueptr;         // Sum of adults in centre
    int *adult_out = variables[6].valueptr;         // Sum of adults which already left
    int *adult_wait = variables[8].valueptr;        // Adult waiting sum

    sem_t *stdout_sem = sem_arr[0].semptr;          // Controls exclusive access to output
    sem_t *traffic_sem = sem_arr[1].semptr;         // Controls traffic from/to centre
    sem_t *spawner_sem = sem_arr[2].semptr;         // Holds spawner processes before finishing 
    sem_t *finish_sem = sem_arr[3].semptr;          // Holds all processes from centre

    sem_t *c_wait_q_sem = sem_arr[4].semptr;        // Holds waiting child to enter
    sem_t *a_wait_q_sem = sem_arr[5].semptr;        // Holds waiting adult to leave

    // Child start section
    sem_wait(traffic_sem);

        // Get Child ID
        int child_id = *child_id_pool;
        
        sem_wait(stdout_sem);
            print_msg(output, line_num,"C %d\t: started\n", child_id);
        sem_post(stdout_sem);

        // Increment ID usage
        *child_id_pool +=1;

    sem_post(traffic_sem);
    
    // Child enter section
    sem_wait(traffic_sem);

    sem_wait(stdout_sem);

    // Check if entering child will violate centre rules
    if(((*child_sum) < ((*adult_sum)*3)) || (*adult_out) == adult)
    {
        // Child enters
        print_msg(output, line_num,"C %d\t: enter\n", child_id);
        *child_sum += 1;
        sem_post(stdout_sem);
        sem_post(traffic_sem);
    }
    else
    {
        // Child waits
        print_msg(output, line_num,"C %d\t: waiting: %d: %d\n",child_id, *adult_sum, *child_sum);
        *child_wait += 1;
        sem_post(stdout_sem);
        sem_post(traffic_sem);

        // Wait in queue to enter (signalled by adults)
        sem_wait(c_wait_q_sem);
        sem_wait(stdout_sem);
            print_msg(output, line_num,"C %d\t: enter\n", child_id);
        sem_post(stdout_sem);
    }

    // Simulate activity
    random_time_sleep(cwt);

    // Child leaving section
    sem_wait(traffic_sem);
    
    sem_wait(stdout_sem);
        print_msg(output, line_num,"C %d\t: trying to leave\n", child_id);
        print_msg(output, line_num,"C %d\t: leave\n", child_id);
    sem_post(stdout_sem);

    *child_sum -= 1;
    *child_out += 1;

    // Check if there are waiting children or adults, signal adult if possible
    if(((*adult_wait) != 0) && ((*child_sum) <= ((*adult_sum-1)*3)))
    {
        // Adult leaves
        *adult_wait -= 1;
        *adult_sum -= 1;
        *adult_out += 1;
        sem_post(a_wait_q_sem);
    }

    int signal = 0;
    //Signal spawners all processes are ready to finish
    if((*child_out == child) && (*adult_out == adult))
        signal = 1;

    sem_post(traffic_sem);

    if(signal)
        sem_post(spawner_sem);

    // Wait for all to exit (signalled by spawners)
    sem_wait(finish_sem);
        sem_wait(stdout_sem);
            print_msg(output, line_num,"C %d\t: finished\n", child_id);
        sem_post(stdout_sem);
    sem_post(finish_sem);
}

// Signal handler after creating shared variables
void sig_exit_shvar(int signal)
{
    shvar_detach_all(variables, SHARED_COUNT);
    shvar_delete_all(variables, SHARED_COUNT);

    if(signal == SIGINT)
        exit(130);
}

// Signal handler after creating semaphores
void sig_exit_sem(int signal)
{
    shvar_detach_all(variables, SHARED_COUNT);
    shvar_delete_all(variables, SHARED_COUNT);

    sem_close_all(sem_arr, SEM_COUNT);
    sem_unlink_all(sem_arr, SEM_COUNT);

    if(signal == SIGINT)
        exit(130);
}

// Signal handler for spawners
void sig_exit_spawner(int signal)
{
    while (wait(NULL) > 0);
    
    shvar_detach_all(variables, SHARED_COUNT);
    shvar_delete_all(variables, SHARED_COUNT);

    sem_close_all(sem_arr, SEM_COUNT);
    sem_unlink_all(sem_arr, SEM_COUNT);

    fclose(output);

    if(signal == SIGINT)
        exit(130);

    if(signal == SIGUSR1)
        exit(2);
}

// Signal handler for adults and children
void sig_exit_duty(int signal)
{
    shvar_detach_all(variables, SHARED_COUNT);
    sem_close_all(sem_arr, SEM_COUNT);
    fclose(output);

    if(signal == SIGINT)
        exit(130);

    if(signal == SIGUSR1)
        exit(2);
}

int main(int argc, char **argv)
{
    // Ignore SIGUSR1 (until specified otherwise)
    signal(SIGUSR1, SIG_IGN);

    // All arguments must be passed
    if(argc != 7)
    {
        if(argc > 7)
            fprintf(stderr,"Too much arguments detected!\n");
        else
            fprintf(stderr,"All arguments are required!\n");
        print_help();
        return EXIT_FAILURE;
    }
    
    // Array for converted passed args
    int arg_arr[6];

    // Check for numbers in argv
    if(check_nums(arg_arr, argv, argc))
    {
        print_help();
        return EXIT_FAILURE;
    }

    // Check for number range in arg_arr
    if(check_num_range(arg_arr, argc))
    {
        print_help();
        return EXIT_FAILURE;
    }

    // Number of adults and children to generate
    int adult = arg_arr[0];
    int child = arg_arr[1];

    // Maximum time for generating the adult and child processes
    int agt = arg_arr[2];
    int cgt = arg_arr[3];

    // Other non-shared variables
    pid_t child_gen_pid = -1;                       // Child spawner PID
    pid_t adult_gen_pid = -1;                       // Adult spawner PID

    // Ignore SIGINT
    signal(SIGINT, SIG_IGN);

    // Check if all shared variables were initialized
    if(shvar_init(variables, SHARED_COUNT) != SHARED_COUNT)
    {
        fprintf(stderr, "Error when initializing shared variables!\n");
        shvar_delete_all(variables, SHARED_COUNT);
        exit(2);
    }

    // Handle SIGINT
    signal(SIGINT, sig_exit_shvar);

    // Zero array for name and write names into array of semaphores

    // Exclusive access to output 
    memset(sem_arr[0].name, 0, 15);
    strcpy(sem_arr[0].name, "/xkr02_stdout");      
    
    // Traffic from/to centre
    memset(sem_arr[1].name, 0, 15);
    strcpy(sem_arr[1].name, "/xkr02_traff");
    
    // Holds spawn processes before finishing 
    memset(sem_arr[2].name, 0, 15);
    strcpy(sem_arr[2].name, "/xkr02_spawner");
    
    // Holds all processes from centre berfore finishing
    memset(sem_arr[3].name, 0, 15);
    strcpy(sem_arr[3].name, "/xkr02_finish");
    
    // Holds waiting child to enter
    memset(sem_arr[4].name, 0, 15);
    strcpy(sem_arr[4].name, "/xkr02_c_q_sem");
    
    // Holds waiting adult to leave
    memset(sem_arr[5].name, 0, 15);
    strcpy(sem_arr[5].name, "/xkr02_a_q_sem");

    // Ignore SIGINT
    signal(SIGINT, SIG_IGN);

    // Check if all semaphorese were successfully created
    if(sem_creat(sem_arr, SEM_COUNT) != SEM_COUNT)
    {
        fprintf(stderr, "An error occured when initializing semaphores!\n");
        shvar_delete_all(variables,SHARED_COUNT);
        sem_unlink_all(sem_arr, SEM_COUNT);
        exit(2);
    }

    // Handle SIGINT
    // Detach and delete all shared variables
    //  
    signal(SIGINT, sig_exit_sem);

    // Set needed semaphores to 1
    sem_post(sem_arr[0].semptr);
    sem_post(sem_arr[1].semptr);

    // Shared variable initialization

    // Output line numbering
    *variables[0].valueptr = 1;

    // Child related shared variables
    *variables[1].valueptr = 0;         // Sum of children in centre
    *variables[2].valueptr = 0;         // Sum of children which already left
    *variables[3].valueptr = 1;         // ID pool for child processes
    *variables[4].valueptr = 0;         // Child waiting sum
    
    // Adult related shared variables
    *variables[5].valueptr = 0;         // Sum of adults in centre
    *variables[6].valueptr = 0;         // Sum of adults which already left
    *variables[7].valueptr = 1;         // ID pool for adult processes
    *variables[8].valueptr = 0;         // Adult waiting sum

    // Seed the rand() function by zeroing the time
    srand(time(NULL));

    // Open file for output
    output = fopen("proj2.out", "w");

    // Check opening file
    if(output == NULL)
    {
        perror("Error opening file");
        
        shvar_detach_all(variables, SHARED_COUNT);
        shvar_delete_all(variables, SHARED_COUNT);

        sem_close_all(sem_arr, SEM_COUNT);
        sem_unlink_all(sem_arr, SEM_COUNT);

        exit(1);
    }
    else // Disable buffering
        setbuf(output, NULL);

    // Handle SIGINT
    signal(SIGINT, sig_exit_spawner);

    // Handle SIGINT
    signal(SIGUSR1, sig_exit_spawner);

    // Fork to get child spawner
    pid_t child_spawner = fork();
    if(child_spawner > 0)
    {
        // Fork to get adult spawner
        pid_t adult_spawner = fork();
        
        // Main process just waits for the children to finish
        if(adult_spawner > 0)
        {
            // Wait for all forked processes
            while (wait(NULL) > 0);

            // Detach and delete all shared variables
            shvar_detach_all(variables, SHARED_COUNT);
            shvar_delete_all(variables,SHARED_COUNT);
            
            // Close and unlink all semaphores
            sem_close_all(sem_arr, SEM_COUNT);
            sem_unlink_all(sem_arr, SEM_COUNT);

            // Close file...
            if(fclose(output) == EOF)
                exit(1);

            return EXIT_SUCCESS;
        }
        
        // If we are adult spawner
        else if(adult_spawner == 0)
        {
            // Store adult spawner PID
            adult_gen_pid = getpid();

            //Generating adults
            spawn_processes(adult, agt);

            // Only spawner process waits for children to finish
            if(getpid() == adult_gen_pid)
            {
                // Just wait until all child processes finish
                gen_duty(sem_arr, adult);
            }
            else
            {
                // Handle SIGINT differently
                signal(SIGINT, sig_exit_duty);

                // Handle SIGUSR1 differently
                signal(SIGUSR1, sig_exit_duty);

                adult_duty(arg_arr, variables, sem_arr);
            }
        }
        else
        {
            perror("Error forking adult_gen");
            kill(0, SIGUSR1);
        }
    }

    // If we are child spawner
    else if(child_spawner == 0)
    {
        // Store child spawner PID
        child_gen_pid = getpid();
        
        //Generating children
        spawn_processes(child, cgt);

        // Only spawner process waits for children to finish
        if(getpid() == child_gen_pid)
        {
            // Just wait until all child processes finish
            gen_duty(sem_arr, child);
        }
        else
        {
            // Handle SIGINT differently
            signal(SIGINT, sig_exit_duty);

            // Handle SIGUSR1 differently
            signal(SIGUSR1, sig_exit_duty);

            child_duty(arg_arr, variables, sem_arr);
        }
    }
    else
    {
        perror("Error when forking child_gen");
        kill(0, SIGUSR1);
    }

    // Detach all shared variables
    shvar_detach_all(variables, SHARED_COUNT);

    // Close all semaphores
    sem_close_all(sem_arr, SEM_COUNT);
    
    // Close file...
    if(fclose(output) == EOF)
        exit(1);

    return EXIT_SUCCESS;
}
