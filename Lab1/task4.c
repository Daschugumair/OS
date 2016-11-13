// vim: set cc=100 ts=4 tw=99 sts=4 et:

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <errno.h>

// message queue
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int my_value = 42;

char* mq_name = "/DEEDS_lab1_mq";

int msg_size = 200;


void parent_close_fail(mqd_t mq_des)
{
    // Close message queue if required
    if (mq_des != 0)
    {
        int err = mq_close(mq_des);
        if (err == -1)
        {
            fprintf(stderr, "Closing message queue on parent side failed with %d\n", errno);
        }
    }

    // Unlink message queue
    int err = mq_unlink(mq_name);
    if (err == -1)
    {
        fprintf(stderr, "Unlinking message queue failed with %d\n", errno);
    }

    exit(1);
}


void parent_unlink_fail()
{
    parent_close_fail(0);
}


void child_close_fail(mqd_t mq_des)
{
    // Close message queue
    if (mq_des != 0)
    {
        int err = mq_close(mq_des);
        if (err == -1)
        {
            fprintf(stderr, "Closing message queue on child side failed with %d\n", errno);
        }
    }

    exit(1);

}


void parent(int child_pid)
{
    // Initial 150ms sleep
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Parent sleeping after fork failed with %d\n", errno);
        parent_unlink_fail();
    }

    fprintf(stderr, "Child Process with pid %i created\n", child_pid);

    // Construct message string
    char message[msg_size];
    sprintf(message, "Hi, I am your parent. My PID=%d and my_value=%d",
            getpid(), my_value);

    // Get message queue descriptor
    mqd_t mqd = mq_open(mq_name, O_WRONLY);
    if (mqd == -1)
    {
        fprintf(stderr, "Opening a message queue for writing failed with %d\n", errno);
        parent_unlink_fail();
    }

    // Write message to queue
    int mq_err = mq_send(mqd, message, msg_size, 0);
    if (mq_err == -1)
    {
        fprintf(stderr, "Writing to message queue failed with %d\n", errno);
        parent_close_fail(mqd);
    }

    // Wait for the child and terminate
    int status;
    pid_t wait_err = wait(&status);
    if (wait_err <= 0)
    {
        fprintf(stderr, "Waiting for child failed with %d\n", errno);
        parent_close_fail(mqd);
    }
    fprintf(stderr, "Child Process with pid %i terminated\n", child_pid);

    // Close message queue
    mq_err = mq_close(mqd);
    if (mq_err == -1)
    {
        fprintf(stderr, "Closing message queue on parent side failed with %d\n", errno);
        parent_unlink_fail();
    }

    // Unlink message queue
    mq_err = mq_unlink(mq_name);
    if (mq_err == -1)
    {
        fprintf(stderr, "Unlinking message queue failed with %d\n", errno);
        exit(1);
    }
}


void child()
{
    // Initial 150ms sleep
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Child sleeping after fork failed with %d\n", errno);
        exit(1);
    }

    my_value = 18951;
    fprintf(stderr, "Hello I am the child process\n"
                    "\tMy pid is %d\n"
                    "\tmy_value is %i\n",
                    getpid(), my_value);

    // Additional 500ms sleep
    sleep_err = usleep(500000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Child sleeping failed with %d", errno);
        exit(1);
    }

    // Open message queue descriptor
    mqd_t mqd = mq_open(mq_name, O_RDONLY);
    if (mqd == -1)
    {
        fprintf(stderr, "Opening message queue for reading failed with %d\n", errno);
        exit(1);
    }

    // Get message size from queue and check return
    struct mq_attr attributes;
    int mq_err = mq_getattr(mqd, &attributes);
    if (mq_err == -1)
    {
        fprintf(stderr, "Getting message queue attributes failed with %d\n", errno);
        child_close_fail(mqd);
    }


    // Read message from queue
    int buf_size = attributes.mq_msgsize;
    char msg_buffer[buf_size];
    int msg_size = mq_receive(mqd, msg_buffer, buf_size, NULL);
    if (msg_size == -1)
    {
        fprintf(stderr, "Message Queue reading failed with error %d\n", errno);
        child_close_fail(mqd);
    }

    fprintf(stderr, "Child received:\n%s\n", msg_buffer);

    // Close message queue
    int err = mq_close(mqd);
    if (err == -1)
    {
        fprintf(stderr, "Closing message queue on child side failed with %d\n", errno);
        exit(1);
    }
}


int main()
{
    int pid;

    // Create message queue
    mqd_t mqd = mq_open(mq_name, O_CREAT, 0777, NULL);
    if (mqd == -1)
    {
        fprintf(stderr, "Creating message queue failed with %d\n", errno);
        exit(1);
    }

    // Fork, check for error and choose work
    pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "Fork failed with %d\n", errno);
        parent_unlink_fail();
    }
    else if (pid == 0)
    {
        child();
    }
    else
    {
        parent(pid);
    }

    return 0;
}
