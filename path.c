#include "shell.h"

char *find_command_in_path(const char *command)
{
	char *path_env, *path_copy, *path, *full_path;

	if (access(command, X_OK) == 0)
		return (strdup(command));

	path_env = getenv("PATH");
	if (!path_env || strlen(path_env) == 0)
		return (NULL);

	path_copy = strdup(path_env);
	if (!path_copy)
	{
		perror("strdup");
		exit(EXIT_FAILURE);
	}

	path = strtok(path_copy, ":");
	while (path)
	{
		full_path = malloc(strlen(path) + strlen(command) + 2);
		if (!full_path)
		{
			perror("malloc");
			free(path_copy);
			exit(EXIT_FAILURE);
		}
		sprintf(full_path, "%s/%s", path, command);

		if (access(full_path, X_OK) == 0)
		{
			free(path_copy);
			return (full_path);
		}

		free(full_path);
		path = strtok(NULL, ":");
	}

	free(path_copy);
	return (NULL);
}
