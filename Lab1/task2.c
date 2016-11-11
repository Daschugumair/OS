// vim: set cc=100 ts=4 tw=99 sts=4 et:
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int my_value = 42;

void* thread_function()
{
    // Sleep for 150ms
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Childs sleeping after creation failed with %i", sleep_err);
        exit(1);
    }

    // Change value and print
    my_value = 18951;
    fprintf(stderr, "Hello I am the sub thread\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

    // Cleep for additional 500ms
    sleep_err = usleep(500000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Child sleeping failed with %i", sleep_err);
        exit(1);
    }

    pthread_exit((void*)NULL);
}

int main()
{
    pthread_t thread;
    int *ret;

    // Create child thread
    int err = pthread_create(&thread, NULL, (void *)&thread_function, &my_value);
    if (err != 0)
    {
        printf("Error!");
        return -1;
    }
    fprintf(stderr, "sub thread created\n");

    // Sleep for 150ms
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Parents sleeping after child creation failed with %i", sleep_err);
        exit(1);
    }

    fprintf(stderr, "Hello I am the main thread\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

    // Wait for child thread and exit
    err = pthread_join(thread,(void**)&ret);
    if (err != 0)
    {
        printf("Error!");
        return -1;
    }
    fprintf(stderr, "sub thread terminated\n");

    return 0;
}
