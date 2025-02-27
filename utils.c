#include "shell.h"

void read_command(char *buffer)
{
	int n = read(STDIN_FILENO, buffer, MAX_COMMAND_LENGTH - 1);
	if (n == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	else if (n == 0)
		exit(EXIT_SUCCESS);

	buffer[n] = '\0';
	if (buffer[n - 1] == '\n')
		buffer[n - 1] = '\0';
}
