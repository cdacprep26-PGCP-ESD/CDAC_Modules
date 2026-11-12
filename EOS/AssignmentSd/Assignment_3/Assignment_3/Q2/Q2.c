#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

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

            // Step 2: Launch different applications
            switch (i)
            {
                case 0:
                {
                    printf("Child 1: Launching PDF Viewer\n");

                    char *args[] = {
                        "xdg-open", "sample.pdf", NULL
                    };

                    execvp(args[0], args);
                    break;
                }

                case 1:
                {
                    printf("Child 2: Launching KWrite\n");

                    char *args[] = {
                        "kwrite", "notes.txt", NULL
                    };

                    execvp(args[0], args);
                    break;
                }

                case 2:
                {
                    printf("Child 3: Launching GCC compiler\n");

                    char *args[] = {
                        "gcc", "-c", "while1.c",
                        "-o", "while1.o", NULL
                    };

                    execvp(args[0], args);
                    break;
                }

                case 3:
                {
                    printf("Child 4: Launching while1 application\n");

                    char *args[] = {
                        "./while1.out", NULL
                    };

                    execvp(args[0], args);
                    break;
                }

                case 4:
                {
                    printf("Child 5: Launching false command\n");

                    char *args[] = {
                        "false", NULL
                    };

                    execvp(args[0], args);
                    break;
                }
            }

            // Executes only if execvp() fails
            perror("exec failed");
            _exit(127);
        }
    }

    // Step 3: Parent completes its own work
    printf("\nParent: All five children created.\n");

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        printf("Child %d PID = %d\n", i + 1, pid[i]);
    }

    printf("\nParent is doing its own work...\n");

    sleep(5);

    // Step 4: Terminate Child 4 abnormally
    printf("\nParent: Sending SIGKILL to Child 4...\n");

    if (kill(pid[3], SIGKILL) == 0)
    {
        printf("SIGKILL sent successfully to Child 4.\n");
    }
    else
    {
        perror("kill failed");
    }

    // Step 5: Collect termination status of all children
    printf("\nParent: Collecting child statuses...\n\n");

    for (int i = 0; i < NUM_CHILDREN; i++)
    {
        pid_t child_pid;

        do
        {
            child_pid = waitpid(pid[i], &status, 0);
        }
        while (child_pid == -1 && errno == EINTR);

        if (child_pid == -1)
        {
            perror("waitpid failed");
            continue;
        }

        printf("Collected Child PID: %d\n", child_pid);

        if (WIFEXITED(status))
        {
            int exit_code = WEXITSTATUS(status);

            if (exit_code == 0)
            {
                printf("Normal successful termination\n");
            }
            else
            {
                printf("Normal unsuccessful termination\n");
                printf("Exit status: %d\n", exit_code);
            }
        }
        else if (WIFSIGNALED(status))
        {
            printf("Abnormal termination\n");
            printf("Signal number: %d\n",
                   WTERMSIG(status));
        }

        printf("-----------------------------\n");
    }

    printf("\nParent: All five children collected.\n");
    printf("Parent process exiting.\n");

    return 0;
}
