// vim: set cc=100 ts=4 tw=99 sts=4 et:

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include <errno.h>

// shared memory
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

int my_value = 42;

const char* shm_name = "/DEEDS_lab1_shm";

size_t msg_size = 200;


void parent_unlink_fail()
{
    int unlink_err = shm_unlink(shm_name);
    if(unlink_err == -1)
    {
        fprintf(stderr, "[pid:%d] error unlinking the Shared Memory Object.",
                        getpid());
    }
    exit(1);
}

void parent_unmap_fail(void *buffer)
{
    int shm_err = munmap(buffer, msg_size);
    if (shm_err == -1)
    {
        fprintf(stderr, "[pid:%d] unmapping the Shared Memory Object failed"
                        " with %d\n", getpid(), errno);
    }
    parent_unlink_fail();
}


void parent(int child_pid)
{
    fprintf(stderr, "Child Process with pid %i created\n", child_pid);

    // Create Shared Memory Object
    int shmd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shmd == -1)
    {
        fprintf(stderr, "Creating SMO failed with %d\n", errno);
        exit(1);
    }

    // Allocate SMO size
    int shm_err = ftruncate(shmd, msg_size);
    if (shm_err == -1)
    {
        fprintf(stderr, "Cannot set SMO size to %ld, error %d\n", msg_size,
                        errno);
        parent_unlink_fail();
    }

    // Map SMO to a variable
    char *message = mmap(NULL, msg_size, PROT_WRITE, MAP_SHARED, shmd, 0);
    if (message == MAP_FAILED)
    {
        fprintf(stderr, "Cannot truncate SMO, error %d\n", errno);
        parent_unlink_fail();
    }

    // Construct message string
    sprintf(message, "Hi, I am your parent. My PID=%d and my_value=%d",
            getpid(), my_value);

    // 150ms sleep
    int sleep_err = usleep(150000);
    if (sleep_err == -1)
    {
        fprintf(stderr, "Parent sleepin failed with %d\n", errno);
        parent_unlink_fail();
    }

    // Wait for the child and terminate
    int status;
    pid_t wait_err = wait(&status);
    if (wait_err <= 0)
    {
        fprintf(stderr, "Waiting for child failed with %d\n", errno);
        parent_unmap_fail(message);
    }
    fprintf(stderr, "Child Process with pid %i terminated\n", child_pid);

    // Unmap Shared Memory Object
    shm_err = munmap(message, msg_size);
    if (shm_err == -1)
    {
        fprintf(stderr, "Unmapping Shared Memory Object on parent side failed with %d\n", errno);
        parent_unlink_fail();
    }

    // Unlink Shared Memory Object
    shm_err = shm_unlink(shm_name);
    if(shm_err == -1)
    {
      fprintf(stderr, "Error unlinking the Shared Memory Object from parent on exit\n");
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

    // Open SMO descriptor for reading
    int shmd = shm_open(shm_name, O_RDONLY, 0666);
    if (shmd == -1)
    {
        fprintf(stderr, "Opening SMO for reading failed with %d\n", errno);
        exit(1);
    }

    // Map SMO to a variable
    char *message = mmap(NULL, msg_size, PROT_READ, MAP_SHARED, shmd, 0);
    if (message == MAP_FAILED)
    {
        fprintf(stderr, "Cannot truncate SMO, error %d\n", errno);
        exit(1);
    }

    // Output the message from the shared memory
    fprintf(stderr, "Child received:\n%s\n", message);

    // Unmap Shared Memory Object
    int shm_err = munmap(message, msg_size);
    if (shm_err == -1)
    {
        fprintf(stderr, "Unmapping shared memory on parent side failed with %d\n", errno);
        exit(1);
    }
}


int main()
{
    int pid;

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
