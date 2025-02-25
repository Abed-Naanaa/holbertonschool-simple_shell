#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

void execute_command(char *command) {
    pid_t pid = fork();
    
    if (pid == -1) {
        perror("Fork failed");
        exit(1);
    }

    if (pid == 0) {
        char *args[2];
        args[0] = command;
        args[1] = NULL;
        if (execve(command, args, NULL) == -1) {
            perror(command);
            exit(1);
        }
    } else {
        wait(NULL);
    }
}

int main(void) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while (1) {
        printf(PROMPT);
        read = getline(&line, &len, stdin);

        if (read == -1) {
            free(line);
            exit(0);
        }

        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }

        execute_command(line);
    }

    free(line);
    return 0;
}
