#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include "shell.h"

void prompt(void)
{
    printf("($) ");
}

void execute_command(char *command)
{
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char *args[] = {command, NULL};
        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        wait(NULL);
    }
}

int main(void)
{
    char buffer[BUFFER_SIZE];

    while (1) {
        prompt();
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (feof(stdin)) {
                break;
            } else {
                perror("fgets failed");
                continue;
            }
        }

        buffer[strcspn(buffer, "\n")] = 0;

        if (strlen(buffer) == 0) {
            continue;
        }

        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        execute_command(buffer);
    }

    return 0;
}
