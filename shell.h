#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_COMMAND_LENGTH 1024

extern char **environ;

char *find_command_in_path(const char *command);
void execute_command(char *command);
void read_command(char *buffer);
void print_env(void);

#endif

