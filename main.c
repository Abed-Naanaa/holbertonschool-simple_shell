#include "shell.h"

void execute_command(char *command)
{
	pid_t pid;
	int status;
	char *argv[2];

	argv[0] = command;
	argv[1] = NULL;

	pid = fork();
	if (pid == -1)
	{
		perror("Error");
		return;
	}

	if (pid == 0)
	{
		if (execve(command, argv, environ) == -1)
		{
			perror("./shell");
			exit(127);
		}
	}
	else
		waitpid(pid, &status, 0);
}

int main(void)
{
	char *input = NULL;
	size_t len = 0;
	ssize_t read;

	while (1)
	{
		printf("#cisfun$ ");
		fflush(stdout);

		read = getline(&input, &len, stdin);
		if (read == -1)
		{
			printf("\n");
			break;
		}

		input[strcspn(input, "\n")] = '\0';
		execute_command(input);
	}

	free(input);
	return (0);
}
