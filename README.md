# MiniShell

A simple UNIX-style command-line shell implemented in C.

## Overview

MiniShell (`MyShell`) is an interactive program that:

- Displays a prompt (`MyShell$ `)  
- Reads a line of user input  
- Splits the input into a command and arguments  
- Searches the user’s `PATH` for the executable  
- Forks a child process to run the command via `execv()`  
- Waits for the child to finish, then loops back to the prompt  
- Supports the built-in `exit` command to quit the shell  

_Code based on the Lab 2.1 “A simple shell” exercise

## Prerequisites

- Linux or UNIX‐compatible OS  
- GCC (or compatible C compiler)  
- Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `unistd.h`, `sys/wait.h`, etc.)

## Building

```bash
gcc -o MyShell command.c
