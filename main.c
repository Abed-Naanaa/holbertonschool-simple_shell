#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

/**
 * display_prompt - Displays the shell prompt.
 */
void display_prompt(void)
{
    printf("#cisfun$ ");
    fflush(stdout);
}

/**
 * execute_command - Executes a command using execve.
 * @command: The command to execute.
 *
 * Return: 0 on success, -1 on failure.
 */
int execute_command(char *command)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return (-1);
    }

    if (pid == 0) /* Child process */
    {
        if (execve(command, NULL, environ) == -1)
        {
            perror(command);
            exit(EXIT_FAILURE);
        }
    }
    else /* Parent process */
    {
        wait(&status);
    }

    return (0);
}

/**
 * main - Entry point of the shell program.
 *
 * Return: Always 0.
 */
int main(void)
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        display_prompt();

        if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL)
        {
            /* Handle EOF (Ctrl+D) */
            printf("\n");
            break;
        }

        /* Remove the newline character from the input */
        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            /* Empty input, just display the prompt again */
            continue;
        }

        /* Execute the command */
        if (execute_command(input) == -1)
        {
            /* Error message is already printed by execute_command */
        }
    }

    return (0);}
