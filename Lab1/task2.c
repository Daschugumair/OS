#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>


void* thread_function(void)
{
    fprintf(stderr, "Hello I am a thread\n"
                    "My pid is %d\n",
                    getpid());
    //my_value = 18951;
    //fprintf(stderr, "my_value is %i\n", my_value);
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

    int err1 = pthread_create(&thread, NULL, (void *)&thread_function, NULL);
    usleep(150000);

    fprintf(stderr, "Hello I am the parent process\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

    int err2 = pthread_join(thread,(void**)&ret);

    if (err1 != 0 || err2 != 0) {
        printf("Error!");
        return -1;
    }

    //printf("ret is %i\n", *ret);
    //free(y);

    return 0;
}
