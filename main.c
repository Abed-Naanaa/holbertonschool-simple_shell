#include "shell.h"

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
 * trim_whitespace - Trims leading and trailing whitespace from a string.
 * @str: The string to trim.
 *
 * Return: A pointer to the trimmed string.
 */
char *trim_whitespace(char *str)
{
    char *end;

    /* Trim leading space */
    while (isspace((unsigned char)*str)) str++;

    if (*str == '\0') /* All spaces? */
        return str;

    /* Trim trailing space */
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    /* Write new null terminator */
    *(end + 1) = '\0';

    return str;
}

/**
 * execute_command - Executes a command using execve.
 * @command: The command to execute.
 */
void execute_command(char *command)
{
    pid_t pid;
    int status;
    char *argv[] = {command, NULL};

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0) /* Child process */
    {
        if (execve(command, argv, environ) == -1)
        {
            perror(command);
            exit(EXIT_FAILURE);
        }
    }
    else /* Parent process */
    {
        wait(&status);
    }
}

/**
 * main - Entry point of the shell program.
 *
 * Return: Always 0.
 */
int main(void)
{
    char *input = NULL;
    size_t input_size = 0;
    ssize_t read_size;
    char *command;

    while (1)
    {
        display_prompt();

        read_size = getline(&input, &input_size, stdin);
        if (read_size == -1)
        {
            /* Handle EOF (Ctrl+D) */
            printf("\n");
            free(input);
            break;
        }

        /* Remove the newline character from the input */
        input[strcspn(input, "\n")] = '\0';

        /* Split input into commands based on newlines */
        command = strtok(input, "\n");
        while (command != NULL)
        {
            /* Trim leading/trailing whitespace from the command */
            command = trim_whitespace(command);

            if (strlen(command) > 0)
            {
                /* Execute the command */
                execute_command(command);
            }

            /* Get the next command */
            command = strtok(NULL, "\n");
        }
    }

    free(input); /* Free the allocated memory for input */
    return (0);
}
