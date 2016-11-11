#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* thread_function(void)
{
    int *ret = malloc(sizeof(int));
    *ret = 5;
    pthread_exit((void*)ret);
}
int main()
{
    pthread_t thread;
    int *y;

    int err1 = pthread_create (&thread, NULL, (void *)&thread_function, NULL);

    int err2 = pthread_join(thread,(void**)&y);

    if (err1 != 0 || err2 != 0) {
        printf("Error!");
        return -1;
    }

    printf("y is %i\n", *y);
    free(y);

    return 0;
}
