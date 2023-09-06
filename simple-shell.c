#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t ctrl_c = 0;

// Signal handler function to catch Ctrl+C
void sigint_handler(int signum) {
    (void)signum;  // Suppress unused variable warning
    ctrl_c = 1;
    exit(EXIT_FAILURE);  // Terminate the program immediately
}

int is_valid_cmd();

int main() {
    char input[1000000];
    signal(SIGINT, sigint_handler); //registering the ctrl+c signal
    while(!ctrl_c) {
        int cmd_valid = 0;
        while (!cmd_valid){
            printf("simple-shell> ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("\n");
                break;
            }
            input[strcspn(input, "\n")] = '\0';
            // Check if the input is empty (just Enter was pressed)
            if (strlen(input) == 0) {
                continue;  // Prompt for input again
            }
            char* token = strtok(input, "|");
            if (token != NULL) {
                // '|' exists in the input
                printf("Pipeline detected.\n");
                cmd_valid = 1;
                // Now you can process the individual commands in the pipeline
                // For example, you can further tokenize each part of the pipeline
                while (token != NULL) {
                    // Process the token as a command
                    printf("Command: %s\n", token);

                    // Get the next token
                    token = strtok(NULL, "|");
                }
            } else {
                // '|' does not exist in the input
                printf("No pipeline detected.\n");
                cmd_valid = 1;
                // You can handle the case where '|' is not present, and this is a single command
            }
        }
    }
    return 0;
}