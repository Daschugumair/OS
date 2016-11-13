// vim: set cc=100 ts=4 tw=99 sts=4 et:
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int my_value = 42;

int main()
{
    int status;
    int pid;

    // Fork and check for error
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr,"Error\n");
        exit(1);
    }

    // Initial sleep of 150ms
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Sleeping after fork failed with %i", sleep_err);
        exit(1);
    }

    // Parent process announces child creation
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
        exit(0);
    }

    // Parent process prints values
    fprintf(stderr, "Hello I am the parent process\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

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
