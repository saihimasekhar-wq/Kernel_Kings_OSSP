#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 100
#define INITIAL_HISTORY 10

int main() {

    char *buffer = NULL;
    size_t buffer_size = 0;

    char **history = NULL;
    int history_count = 0;
    int history_capacity = INITIAL_HISTORY;

    history = malloc(history_capacity * sizeof(char *));

    if (history == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    while (1) {

        printf("myshell> ");
        fflush(stdout);

        ssize_t characters = getline(&buffer, &buffer_size, stdin);

        if (characters == -1) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting shell...\n");
            break;
        }

        if (strlen(buffer) == 0) {
            continue;
        }

        // Resize history if necessary
        if (history_count >= history_capacity) {

            history_capacity *= 2;

            char **temp = realloc(
                history,
                history_capacity * sizeof(char *)
            );

            if (temp == NULL) {
                printf("Memory allocation failed\n");
                break;
            }

            history = temp;
        }

        // Store command
        history[history_count] = malloc(strlen(buffer) + 1);

        if (history[history_count] == NULL) {
            printf("Memory allocation failed\n");
            break;
        }

        strcpy(history[history_count], buffer);

        history_count++;

        printf("You entered: %s\n", buffer);
    }

    // Free history
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }

    free(history);
    free(buffer);

    return 0;
}
