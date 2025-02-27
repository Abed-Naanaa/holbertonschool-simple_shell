#include "shell.h"

void execute_command(char *command)
{
	char *args[MAX_COMMAND_LENGTH / 2];
	char *command_path;
	int i = 0;
	char *token;

	token = strtok(command, " ");
	while (token)
	{
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;

	if (!args[0])
		return;

	if (args[0][0] == '/' || args[0][0] == '.')
		command_path = args[0];
	else
		command_path = find_command_in_path(args[0]);

	if (command_path == NULL)
	{
		write(STDERR_FILENO, "Command not found\n", 18);
		exit(127);
	}

	if (execve(command_path, args, environ) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}
