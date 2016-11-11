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
    if(pid<0)
    {
        fprintf(stderr,"Error\n");
        exit(1);
    }
    else if(pid==0)
    {
        my_value = 18951;
        fprintf(stderr, "Hello I am the child process\n"
                        "\tMy pid is %d\n"
                        "\tmy_value is %i\n",
                        getpid(), my_value);
        usleep(500000);
        exit(0);
    }
    else
    {
        wait(&status);
        fprintf(stderr, "Hello I am the parent process\n"
                        "\tMy pid is %d\n"
                        "\tmy_value is %i\n",
                        getpid(), my_value);
        exit(0);
    }
}
