#include "shell.h"

int main(void)
{
	char command[MAX_COMMAND_LENGTH];
	pid_t pid;
	char *cmd;
	int status;

	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "#cisfun$ ", 9);

		read_command(command);
		if (strlen(command) == 0)
			continue;

		cmd = strtok(command, "\n");
		while (cmd)
		{
			while (*cmd == ' ')
				cmd++;

			if (strlen(cmd) == 0)
			{
				cmd = strtok(NULL, "\n");
				continue;
			}

			if (strcmp(cmd, "exit") == 0)
				exit(EXIT_SUCCESS);

			if (strcmp(cmd, "env") == 0)
			{
				print_env();
				cmd = strtok(NULL, "\n");
				continue;
			}

			pid = fork();
			if (pid == -1)
			{
				perror("fork");
				exit(EXIT_FAILURE);
			}

			if (pid == 0)
				execute_command(cmd);
			else
			{
				wait(&status);
				if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
					exit(WEXITSTATUS(status));
			}

			cmd = strtok(NULL, "\n");
		}
	}
	return (0);
}
