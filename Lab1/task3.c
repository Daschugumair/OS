// vim: set cc=100 ts=4 tw=99 sts=4 et:
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <errno.h>

int my_value = 42;

int main()
{
    int status;
    int pid;

    // Initializing pipe. fd[0] is for reading, fd[1] is for writing 
    int fd[2];
    int pipe_err = pipe(fd);
    
    if (pipe_err == -1)
    {
        fprintf(stderr, "Initializing pipe failed with %i", pipe_err);
        exit(1);
    }
    
    

    pid = fork();

    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Sleeping after fork failed with %i", sleep_err);
        exit(1);
    }

    // Something went wrong
    if (pid < 0)
    {
        fprintf(stderr,"Error\n");
        exit(1);
    }

    // Parent process announces the creation
    if (pid != 0)
    {
        fprintf(stderr, "Child Process with pid %i created\n", pid);
    }

    // Child process prints values
    if (pid == 0)
    {
        my_value = 18951;
        fprintf(stderr, "Hello I am the child process\n"
                        "\tMy pid is %d\n"
                        "\tmy_value is %i\n",
                        getpid(), my_value);
        int sleep_err = usleep(500000);
        if (sleep_err == -1)
        {
            fprintf(stderr, "Child sleeping failed with %i", sleep_err);
            exit(1);
        }

        // Close writing on the Child
        close(fd[1]);
        char pipeBuffer[100];

        int read_err = read(fd[0], pipeBuffer , 100);

        if (read_err == -1)
        {
            fprintf(stderr, "Read failed with error code %i\n", errno);
            exit(1);
        }

        fprintf(stderr, "Buffer: %s\n", pipeBuffer); 


        exit(0);
    }

    char message[100];

    // Parent process prints values. sprintf stores into a variable message
    sprintf(message, "Hi, I am your parent. My PID=%d and my_value=%d",
            getpid(), my_value);

    // Close reading on Parent
    close(fd[0]);
    write(fd[1], message, 100);


    // wait for the child and terminate
    pid_t err = wait(&status);
    if (err <= 0)
    {
        fprintf(stderr,"Error\n");
        exit(1);
    }
    fprintf(stderr, "Child Process with pid %i terminated\n", pid);

    return 0;
}
