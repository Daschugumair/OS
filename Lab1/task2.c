#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

/**
    QUESTION

    In the task it says we should simply change the task1 to use pthread instead of fork.
    However it is not specified how the memory of my_value will be shared. I simply passed the pointer here.
    Obviously changing the value in the sub thread changes the value in the main thread. But it is not clear to me if this is what is intended.

*/

void* thread_function(int* my_value)
{
    *my_value = 18951;
    fprintf(stderr, "Hello I am the sub thread\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), *my_value);

    usleep(500000);

    //int *ret = malloc(sizeof(int));
    //*ret = 5;
    //pthread_exit((void*)ret);
    pthread_exit((void*)NULL);
}

int main()
{
    int my_value = 42;
    pthread_t thread;
    int *ret;

    int err1 = pthread_create(&thread, NULL, (void *)&thread_function, &my_value);
    if (err1 != 0) {
        printf("Error!");
        return -1;
    }

    fprintf(stderr, "sub thread created\n");

    usleep(150000);

    fprintf(stderr, "Hello I am the main thread\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

    int err2 = pthread_join(thread,(void**)&ret);

    if (err2 != 0) {
        printf("Error!");
        return -1;
    }

    fprintf(stderr, "sub thread terminated\n");

    //printf("ret is %i\n", *ret);
    //free(y);

    return 0;
}
