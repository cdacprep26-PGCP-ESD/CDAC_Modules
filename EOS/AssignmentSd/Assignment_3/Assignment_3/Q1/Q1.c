#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define NUM_CHILDREN 5

int main()
{
    pid_t pid[NUM_CHILDREN];
    int status;

    setbuf(stdout, NULL);

    printf("Parent Process Started\n");
    printf("Parent PID: %d\n\n", getpid());

    // Step 1: Create five child processes
    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid[i] = fork();

        if (pid[i] < 0)
        {
            perror("fork failed");
            return 1;
        }

        if (pid[i] == 0)
        {
            printf("Child %d created. PID = %d, PPID = %d\n",
                   i + 1, getpid(), getppid());

            switch (i)
            {
                case 0:
                    printf("Child 1: Successful termination\n");
                    _exit(-1);

                case 1:
                    printf("Child 2: Unsuccessful termination\n");
                    _exit(5);

                case 2:
                    printf("Child 3: Running infinite loop\n");

                    while (1)
                    {
                        sleep(1);
                    }

                case 3:
                    printf("Child 4: Unsuccessful termination\n");
                    _exit(99);

                case 4:
                    printf("Child 5: Successful termination\n");
                    _exit(0);
            }

            _exit(1);
        }
    }

    // Step 2: Parent completes its own work
    printf("\nParent: All five children created.\n");

    printf("\nChild PIDs:\n");

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        printf("Child %d PID = %d\n", i + 1, pid[i]);
    }

    printf("\nParent is doing its own work...\n");

    sleep(5);

    printf("\nParent: My work is completed.\n");

    // Step 3: Send SIGKILL to Child 3
    printf("\nParent: Sending SIGKILL to Child 3...\n");

    if (kill(pid[2], SIGKILL) == 0)
    {
        printf("Parent: SIGKILL sent successfully to Child 3.\n");
    }
    else
    {
        perror("kill failed");
    }

    // Step 4: Collect all five children
    printf("\nParent: Collecting child termination statuses.\n\n");

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t child_pid = waitpid(-1, &status, 0);

        if (child_pid == -1)
        {
            perror("waitpid failed");
            return 1;
        }

        printf("Collected Child PID: %d\n", child_pid);

        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);

            if (exit_code == 0)
            {
                printf("Status: Normal successful termination\n");
            }
            else
            {
                printf("Status: Normal unsuccessful termination\n");
                printf("Exit code: %d\n", exit_code);
            }
        }
        else if (WIFSIGNALED(status))
        {
            printf("Status: Abnormal termination\n");
            printf("Terminating signal: %d\n",
                   WTERMSIG(status));
        }

        printf("-----------------------------\n");
    }

    printf("\nParent: All five children collected.\n");
    printf("Parent process exiting.\n");

    return 0;
}

