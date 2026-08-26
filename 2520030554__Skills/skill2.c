#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT 100

void processCommand(char *command) {
    if (strcmp(command, "hello") == 0) {
        printf("Hello! Welcome to MyShell.\n");
    }
    else if (strcmp(command, "help") == 0) {
        printf("\nAvailable commands:\n");
        printf("  hello - Display greeting\n");
        printf("  help  - Display available commands\n");
        printf("  exit  - Exit the shell\n");
    }
    else if (strlen(command) == 0) {
        // Do nothing for empty input
    }
    else {
        printf("Unknown command: %s\n", command);
    }
}

int main() {
    char buffer[MAX_INPUT];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        fgets(buffer, sizeof(buffer), stdin);

        // Remove newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Check exit condition
        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }
        else {
            processCommand(buffer);
            fflush(stdout);
        }
    }

    return 0;
}

/*
myshell> hello
Hello! Welcome to MyShell.
myshell> help

Available commands:
  hello - Display greeting
  help  - Display available commands
  exit  - Exit the shell
myshell> abc
Unknown command: abc
myshell> Enter
Unknown command: Enter
myshell> exit
Exiting shell...

myshell> hello
help
abc
exitHello! Welcome to MyShell.
myshell>
Available commands:
  hello - Display greeting
  help  - Display available commands
  exit  - Exit the shell
myshell> Unknown command: abc
myshell> exit
Exiting shell...
==5502==
==5502== HEAP SUMMARY:
==5502==     in use at exit: 0 bytes in 0 blocks
==5502==   total heap usage: 2 allocs, 2 frees, 2,048 bytes allocated
==5502==
==5502== All heap blocks were freed -- no leaks are possible
==5502==
==5502== For lists of detected and suppressed errors, rerun with: -s
==5502== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
*/
