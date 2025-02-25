#include "shell.h"
#include <string.h>
#include <ctype.h>

void execute_command(char *command)
{
    pid_t pid;
    int status;
    char *argv[2];
    char *end;

    while (isspace((unsigned char)*command)) command++;
    end = command + strlen(command) - 1;
    while (end > command && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    if (strlen(command) == 0)
    {
        printf("Error: Empty command\n");
        return;
    }

    argv[0] = command;
    argv[1] = NULL;

    pid = fork();
    if (pid == -1)
    {
        perror("Error: fork failed");
        return;
    }

    if (pid == 0)
    {
        if (execve(command, argv, environ) == -1)
        {
            perror("Error: execve failed");
            exit(127);
        }
    }
    else
    {
        if (waitpid(pid, &status, 0) == -1)
        {
            perror("Error: waitpid failed");
            return;
        }
    }
}

int main(void)
{
    char *input = NULL;
    size_t len = 0;
    ssize_t read;

    while (1)
    {
        if (isatty(STDIN_FILENO))
        {
            printf("#cisfun$ ");
            fflush(stdout);
        }

        read = getline(&input, &len, stdin);
        if (read == -1)
        {
            if (isatty(STDIN_FILENO))
                printf("\n");
            break;
        }

        input[strcspn(input, "\n")] = '\0';
        execute_command(input);
    }

    free(input);
    return (0);
}
