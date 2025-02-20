#include <main.h>

int main(int argc, char **argv)
{
        (void)argc, (void)argv;
        char *buf = NULL;
        size_t count = 0;
        ssize_t nread;
        pid_t child_pid;
        int status;

        while (1)
        {
                write(STDOUT_FILENO, "MyShell$ ", 9);

                nread = getline(&buf, &count, stdin);

                if (nread ==  -1)
                {
                        perror("Exiting shell");
                        exit(1);
                }

                child_pid = fork();

                if (child_pid == -1)
                {
                        perror("Failed to create.");
                        exit (41);
                }

                if (child_pid == 0)
                {
                        /* The creation was successful and we can execute the user input */
if (execve(array[0], array, NULL) == -1)
           {
                   perror("Couldn't execute");
                   exit(7);
           }


                        printf("The creation was successful\n");
                }
                else
                {
                        /* Wait for the child process to execute before terminating the parent process */
                        wait(&status);
                }
        }
        free(buf);
        return (0);
}
