#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

#define MAX_COMMAND_LENGTH 1024

extern char **environ;

char *find_command_in_path(const char *command) {
	char *path_env = getenv("PATH"), *path_copy, *path, *full_path;
	
	if (!path_env || *command == '/' || *command == '.') {
		return (access(command, X_OK) == 0) ? strdup(command) : NULL;
	}

	path_copy = strdup(path_env);
	if (!path_copy) {
		perror("strdup");
		return NULL;
	}

	path = strtok(path_copy, ":");
	while (path) {
		full_path = malloc(strlen(path) + strlen(command) + 2);
		if (!full_path) {
			free(path_copy);
			return NULL;
		}
		sprintf(full_path, "%s/%s", path, command);
		
		if (access(full_path, X_OK) == 0) {
			free(path_copy);
			return full_path;
		}
		free(full_path);
		path = strtok(NULL, ":");
	}

	free(path_copy);
	return NULL;
}

void execute_command(char *command) {
	char *args[MAX_COMMAND_LENGTH / 2], *command_path;
	int i = 0;
	char *token = strtok(command, " ");

	while (token) {
		args[i++] = token;
		token = strtok(NULL, " ");
	}
	args[i] = NULL;

	if (!args[0]) {
		return;
	}

	command_path = find_command_in_path(args[0]);
	if (!command_path) {
		write(STDERR_FILENO, args[0], strlen(args[0]));
		write(STDERR_FILENO, ": Command not found\n", 20);
		exit(127);
	}

	if (execve(command_path, args, environ) == -1) {
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void read_command(char *buffer) {
	int n = read(STDIN_FILENO, buffer, MAX_COMMAND_LENGTH - 1);
	
	if (n == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	} else if (n == 0) {
		exit(EXIT_SUCCESS);
	}

	buffer[n] = '\0';
	if (buffer[n - 1] == '\n') {
		buffer[n - 1] = '\0';
	}
}

void print_env(void) {
	char **env = environ;
	while (*env) {
		write(STDOUT_FILENO, *env, strlen(*env));
		write(STDOUT_FILENO, "\n", 1);
		env++;
	}
}

int main(void) {
	char command[MAX_COMMAND_LENGTH], *cmd, *command_path;
	pid_t pid;
	int status;

	while (1) {
		if (isatty(STDIN_FILENO)) {
			write(STDOUT_FILENO, "#cisfun$ ", 9);
		}

		read_command(command);
		if (strlen(command) == 0) {
			continue;
		}

		cmd = strtok(command, "\n");
		while (cmd) {
			while (*cmd == ' ') {
				cmd++;
			}
			if (strlen(cmd) == 0) {
				cmd = strtok(NULL, "\n");
				continue;
			}

			if (strcmp(cmd, "exit") == 0) {
				exit(EXIT_SUCCESS);
			}

			if (strcmp(cmd, "env") == 0) {
				print_env();
				cmd = strtok(NULL, "\n");
				continue;
			}

			command_path = find_command_in_path(cmd);
			if (!command_path) {
				write(STDERR_FILENO, cmd, strlen(cmd));
				write(STDERR_FILENO, ": Command not found\n", 20);
				cmd = strtok(NULL, "\n");
				continue;
			}

			pid = fork();
			if (pid == -1) {
				perror("fork");
				exit(EXIT_FAILURE);
			}

			if (pid == 0) {
				execute_command(cmd);
			} else {
				wait(&status);
			}

			cmd = strtok(NULL, "\n");
		}
	}

	return 0;
}
