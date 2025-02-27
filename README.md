# Simple Shell

## Description
This is a simple UNIX command line interpreter (shell) written in C. It replicates the functionality of `/bin/sh`, allowing users to execute commands interactively and non-interactively.

## Features
- Executes commands from the PATH.
- Supports built-in commands: `exit`, `env`.
- Interactive mode (displays a prompt) and non-interactive mode.
- Error handling and proper memory management.

## Compilation
```sh
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
