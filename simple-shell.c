#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

volatile sig_atomic_t ctrl_c = 0;
char history[100][1000000];

void sigint_handler(int signum);
int is_valid_cmd(char* input);
void ls(char* input);
char** return_args(char* input);

int main() {
    int num = 0;
    char input[1000000];
    signal(SIGINT, sigint_handler); //registering the ctrl+c signal
    while(!ctrl_c) {
        int cmd = 0;
        while (!cmd){
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
            strcpy(history[num++],input);
            if(strchr(input, '|') == NULL) cmd = is_valid_cmd(input);
            else cmd = 15; // cmd is 15 when pipelining is detected
            if(cmd == 0) printf("Invalid command. Please try again.\n");
        }
        switch(cmd){
            case 16:
                for (int j = 0; j < num; ++j) printf("%s\n",history[j]);
                break;
            case 14:
                ls(input);
                break;
        }
    }
    return 0;
}

// Signal handler function to catch Ctrl+C
void sigint_handler(int signum) {
    (void)signum;  // Suppress unused variable warning
    ctrl_c = 1;
    printf("\n");
    exit(0);  // Terminate the program immediately
}

int is_valid_cmd(char* input){
    char* token = strtok(input, " ");
    if(input[0] == '.' && input[1] == '/') return 1; // returns 1 for './' cmd
    else if(strcmp(token,"cd") == 0) return 2; //returns 2 for 'cd' cmd
    else if(strcmp(token, "mkdir") == 0) return 3; // returns 3 for 'mkdir' cmd
    else if(strcmp(token,"rmdir") == 0) return 4; // returns 4 for 'rmdir' cmd
    else if(strcmp(token,"rm") == 0) return 5; // returns 5 for 'rm' cmd
    else if(strcmp(token,"mv") == 0) return 6; // returns 6 for 'mv' cmd
    else if(strcmp(token,"cp") == 0) return 7; // returns 7 for 'cp' cmd
    else if(strcmp(token,"pwd") == 0) return 8; // returns 8 for 'pwd' cmd
    else if(strcmp(token,"uniq") == 0) return 9; // returns 9 for 'uniq' cmd
    else if(strcmp(token,"sort") == 0) return 10; // returns 10 for 'sort' cmd
    else if(strcmp(token,"grep") == 0) return 11; // returns 11 for 'grep' cmd
    else if(strcmp(token,"wc") == 0) return 12; // returns 12 for 'wc' cmd
    else if(strcmp(token,"echo") == 0) return 13; // returns 13 for 'echo' cmd
    else if(strcmp(token,"ls") == 0) return 14; // returns 14 for 'ls' cmd
    else if(strcmp(token,"history") == 0) return 16; // returns 16 for 'history' cmd
    else return 0; // invalid cmd
}

void ls(char* input){
    pid_t child_pid;
    child_pid = fork();
    if(child_pid == -1){
        perror("fork");
        return;
    }
    if(child_pid == 0){
        char** args = return_args(input);
        args[0] = "bin/ls";
        int l = execv("/bin/ls",args);
        if(l == -1){
            perror("execv");
            return;
        }
    }
    else{
        int status;
        if(waitpid(child_pid,&status, 0) == -1){
            perror("waitpid");
            return;
        }
        if (WIFEXITED(status)) {
            printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }
    }
}

char** return_args(char* input){
    char** args = NULL;
    int count = 0;
    char* input_copy = strdup(input);
    char* argument = strtok(input_copy," ");
    while(argument != NULL){
        args = realloc(args, (count + 1) * sizeof(char*));
        if (args == NULL) {
            perror("reallocate");
            return (char **) "error";
        }
        args[count] = strdup(argument);
        printf("args[%d] = %s\n", count, args[count]); // Debug print
        count++;
        argument = strtok(NULL, " ");
    }
    args = realloc(args, (count + 1) * sizeof(char*));
    if (args == NULL) {
        perror("reallocate");
        return (char **) "error";
    }
    args[count] = NULL;
    printf("Contents of args array:\n");
    for (int i = 0; args[i] != NULL; i++) {
        printf("args[%d] = %s\n", i, args[i]);
    }
    return args;
}