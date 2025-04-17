#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Prompt display
void printPrompt() {
    printf("MyShell$ ");
    fflush(stdout);
}

// Read command line input
char* readCommand() {
    char* buffer = malloc(1024);
    if (buffer == NULL) {
        perror("malloc failed");
        exit(1);
    }

    if (fgets(buffer, 1024, stdin) == NULL) {
        free(buffer);
        return NULL;
    }

    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    return buffer;
}

// Parse input into command and arguments
char** parseCommand(char* input) {
    char** args = malloc(64 * sizeof(char*));
    if (args == NULL) {
        perror("malloc failed");
        exit(1);
    }

    int i = 0;
    char* token = strtok(input, " ");
    while (token != NULL && i < 63) {
        args[i] = token;
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    return args;
}

// Just a test loop to demonstrate
int main() {
    while (1) {
        printPrompt();

        char* input = readCommand();
        if (input == NULL || strcmp(input, "exit") == 0) {
            break;
        }

        char** args = parseCommand(input);

        // Debug: Print each parsed word
        int i = 0;
        while (args[i] != NULL) {
            printf("arg[%d]: %s\n", i, args[i]);
            i++;
        }

        free(input);
        free(args);
    }

    return 0;
}
