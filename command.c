// MiniShell: Simple UNIX-like shell implementation
// COSC 4302 - Group Project
// Author: Bryce 

#include <stdio.h>      // for printf, perror, fgets
#include <stdlib.h>     // for malloc, exit
#include <string.h>     // for strtok, strlen, strdup
#include <unistd.h>     // for fork, execv, access
#include <sys/types.h>  // for pid_t
#include <sys/wait.h>   // for wait
#include <fcntl.h>

#define BUFFER_LIMIT 1024  // max length of a command input
#define MAX_TOKENS 64      // max number of arguments
#define DELIM " \t\r\n"     // delimiters for splitting command input

// Prints the shell prompt to the screen
void showPrompt() {
    printf("MyShell$ ");
    fflush(stdout); // makes sure it shows up right away
}

// Reads a full line from the user
char* grabUserInput() {
    char* inputLine = malloc(BUFFER_LIMIT); // dynamically allocate space for input
    if (inputLine == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    if (fgets(inputLine, BUFFER_LIMIT, stdin) == NULL) {
        free(inputLine); // if input fails or EOF, free memory and return null
        return NULL;
    }

    // Remove newline at the end (if there is one)
    size_t len = strlen(inputLine);
    if (len > 0 && inputLine[len - 1] == '\n') {
        inputLine[len - 1] = '\0';
    }

    return inputLine; // return the cleaned-up input string
}

// Splits the command line input into tokens (command + arguments)
char** breakIntoArgs(char* inputLine) {
    char** tokens = malloc(MAX_TOKENS * sizeof(char*)); // allocate memory for argument list
    if (tokens == NULL) {
        perror("Token allocation failed");
        exit(EXIT_FAILURE);
    }

    int pos = 0;
    char* part = strtok(inputLine, DELIM); // split based on space and tab
    while (part != NULL && pos < MAX_TOKENS - 1) {
        tokens[pos++] = part;
        part = strtok(NULL, DELIM);
    }
    tokens[pos] = NULL; // null-terminate the list for execv()

    return tokens;
}

// Searches the PATH directories to find the executable file
char* findExecutable(char* programName) {
    // If it's already a full path and executable, return it
    if (access(programName, X_OK) == 0) {
        return strdup(programName);
    }

    // Get PATH env variable, which contains directories to search
    char* pathEnv = getenv("PATH");
    if (pathEnv == NULL) return NULL;

    char* pathDup = strdup(pathEnv); // duplicate string to safely modify it
    char* directory = strtok(pathDup, ":");
    while (directory != NULL) {
        char testPath[BUFFER_LIMIT];
        snprintf(testPath, sizeof(testPath), "%s/%s", directory, programName);
        if (access(testPath, X_OK) == 0) {
            free(pathDup);
            return strdup(testPath); // found it, return full path
        }
        directory = strtok(NULL, ":");
    }
    free(pathDup);
    return NULL; // executable not found
}

// Main loop of the shell
int main() {
    while (1) {
        showPrompt(); // Step 1: Display prompt

        char* userCommand = grabUserInput(); // Step 2: Read input
        if (userCommand == NULL) continue;

        // Handle built-in 'exit' command to quit the shell
        if (strcmp(userCommand, "exit") == 0) {
            free(userCommand);
            break;
        }

        char** arguments = breakIntoArgs(userCommand); // Step 3: Parse arguments
        if (arguments[0] == NULL) {
            free(userCommand);
            free(arguments);
            continue; // skip if user hit enter with no command
        }

        // Step 4: Locate the executable file
        char* executablePath = findExecutable(arguments[0]);
        if (executablePath == NULL) {
            fprintf(stderr, "%s: command not found\n", arguments[0]);
            free(userCommand);
            free(arguments);
            continue;
        }

        // Step 5: Fork a child process to run the command
        pid_t childPID = fork();
        if (childPID == 0) {
            // This is the child process
            execv(executablePath, arguments); // execute the program
            perror("Execution failed"); // only prints if execv fails
            exit(EXIT_FAILURE);
        } else if (childPID > 0) {
            // This is the parent process
            wait(NULL); // wait for child to finish
        } else {
            perror("Fork failed");
        }

        // Step 6: Free memory used
        free(userCommand);
        free(arguments);
        free(executablePath);
    }

    return 0;
}
