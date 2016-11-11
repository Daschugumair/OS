#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int main()
{
    int my_value = 42;
    int status;
    int pid;

    pid=fork();
    usleep(150000);

    // Something went wrong
    if(pid < 0)
    {
        fprintf(stderr,"Error\n");
        exit(1);
    }

    // Parent process announces the creation
    if (pid != 0) {
        fprintf(stderr, "Child Process with pid %i created\n", pid);
    }

    // Child process prints values
    if(pid == 0)
    {
        my_value = 18951;
        fprintf(stderr, "Hello I am the child process\n"
                        "\tMy pid is %d\n"
                        "\tmy_value is %i\n",
                        getpid(), my_value);
        usleep(500000);
        exit(0);
    }

    // Parent process prints values
    fprintf(stderr, "Hello I am the parent process\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);


    // wait for the child and terminate
    pid_t err = wait(&status);
    if (err <= 0)  {
        fprintf(stderr,"Error\n");
        exit(1);
    }
    fprintf(stderr, "Child Process with pid %i terminated\n", pid);

    /**
        QUESTION

        In the task it says:
        Both the parent and child have to print my_value along with the message that states their PID and the parent has to print it in the child termination message as well.

        I am not sure what is actually meant. Should I print everything again for the parent here?
    */

    return 0;
}
