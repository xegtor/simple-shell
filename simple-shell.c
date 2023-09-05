#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

volitile sig_atomic_t ctrl_c = 0;

// Signal handler function to catch Ctrl+C
void sigint_handler(int signum) {
    (void)signum;  // Suppress unused variable warning
    ctrl_c = 1;
}

int is_valid_cmd()

        int main() {
    string input;
    signal(SIGINT, sigint_handler); //registering the ctrl+c signal
    while(!ctrl_c){
        int cmd_valid = 0;
        while(!cmd_valid)
            printf("simple-shell> ");
        if (fgets(input, sizeof(input), stdin) == NULL){
            printf("\n");
            break;
        }
    }
    return 0;
}