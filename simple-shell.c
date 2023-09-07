#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

typedef struct History{
    char command[1000000];
    pid_t child_pid;
    time_t time;
    double exec_time;
} History;

History history[100];
volatile sig_atomic_t ctrl_c = 0;
int hist_indx = 0;

enum {
    INVALID_CMD = 0,
    DOT_SLASH_CMD,
    CD_CMD,
    MKDIR_CMD,
    RMDIR_CMD,
    RM_CMD,
    MV_CMD,
    CP_CMD,
    PWD_CMD,
    UNIQ_CMD,
    SORT_CMD,
    GREP_CMD,
    WC_CMD,
    ECHO_CMD,
    LS_CMD,
    HISTORY_CMD
};

void sigint_handler(int signum);
int is_valid_cmd(char* input);
char** return_args(char* input);
void ls(char* input);
void echo(char* input);
void wc(char* input);
void hist();

int main() {
    char input[1000000];
    time_t input_time;
    clock_t start,end;

    signal(SIGINT, sigint_handler); //registering the ctrl+c signal
    while(!ctrl_c){
        int cmd = 0;
        while (!cmd){
            printf("simple-shell> ");
            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("\n");
                break;
            }   
            //Taking command as input
            
            time(&input_time);
            //Noting time of the input 

            input[strcspn(input, "\n")] = '\0';
            // Check if the input is empty
            
            if (strlen(input) == 0) {
                continue;
            }
            // Prompt for input again

            if(strchr(input, '|') == NULL) cmd = is_valid_cmd(input); //Pipeline check
            else cmd = 16;
            
            if(cmd == 0) printf("Invalid command. Please try again.\n"); //Invalid Command
        }

        start = clock();
        switch(cmd){
            case 15:
                hist();
                break;
            case 14:
                ls(input);
                break;
            case 13:
                echo(input);
                break;
            case 12:
                wc(input);
                break;

        }
        end = clock();
        
        history[hist_indx].exec_time = ((double) (end - start))/CLOCKS_PER_SEC;
        history[hist_indx].time = input_time;
        strcpy(history[hist_indx].command,input);

        hist_indx++;
    }
    return 0;
}

// Signal handler function to catch Ctrl+C
void sigint_handler(int signum) {
    (void)signum;  // Suppress unused variable warning
    ctrl_c = 1;
    printf("\n");
    for (int i=0; i<hist_indx; i++){
        printf("Command: %s\n", history[i].command);
        printf("Child PID: %d\n", history[i].child_pid);
        printf("Time: %s", ctime(&history[i].time));
        printf("Execution Time: %lf\n", history[i].exec_time);
    }
    printf("\n");
    exit(0);  // Terminate the program immediately
}

int is_valid_cmd(char* input){
    // Create an array of valid command strings
    const char* valid_cmds[] = {
        "",
        "./",
        "cd",
        "mkdir",
        "rmdir",
        "rm",
        "mv",
        "cp",
        "pwd",
        "uniq",
        "sort",
        "grep",
        "wc",
        "echo",
        "ls",
        "history"
    };

    // Tokenize the input to get the first word (command)
    char* input_copy = strdup(input);
    char* cmd_header = strtok(input_copy, " ");
    // Iterate through the valid command strings and check for a match
    for (int i = 1; i <= HISTORY_CMD; i++) {
        if (strcmp(cmd_header, valid_cmds[i]) == 0) {
            free(input_copy); // Free the allocated memory
            return i;
        }
    }
    // Free the allocated memory and return 0 for an invalid command
    free(input_copy);
    return INVALID_CMD;
}

char** return_args(char* input) {
    char** args = NULL;
    int count = 0;
    char* input_copy = strdup(input);

    if (input_copy == NULL) {
        perror("strdup");
        return NULL;
    }

    char* token = strtok(input_copy, " ");
    
    while (token != NULL) {
        args = realloc(args, (count + 2) * sizeof(char*)); // +2 for the new argument and NULL
        if (args == NULL) {
            perror("reallocate");
            free(input_copy);
            return NULL;
        }

        args[count] = strdup(token);

        if (args[count] == NULL) {
            perror("strdup");
            free(input_copy);
            for (int i = 0; i < count; i++) {
                free(args[i]);
            }
            free(args);
            return NULL;
        }

        count++;
        args[count] = NULL; // Ensure the array is NULL-terminated
        token = strtok(NULL, " ");
    }

    free(input_copy);
    return args;
}

void ls(char* input) {
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
            perror("Waitpid");
            return;
        }
       if (WIFEXITED(status)) {
            history[hist_indx].child_pid = child_pid;
            //printf("Child process exited with status %d\n", WEXITSTATUS(status));
       }
    }
}

void echo(char* input) {
    // Tokenize the input by space
    char* input_copy = strdup(input);

    if (input_copy == NULL) {
        perror("strdup");
    }

    char* command = strtok(input_copy, " \t\n");
    char* argument = strtok(NULL, "\n");

    // Create an array for the command and its argument
    char* args[] = {command, argument, NULL};

    pid_t child_pid = fork();
    
    if (child_pid == -1) {
        perror("fork");
        return;
    }

    if (child_pid == 0) { // Child process
        // Execute the command with arguments
        execvp(command, args);
        
        // If execvp returns, there was an error
        perror("execvp");
        exit(EXIT_FAILURE);
    } 
    else {
        int status;
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("waitpid");
            return;
        }
        if (WIFEXITED(status)) {
            history[hist_indx].child_pid = child_pid;
            // printf("Child process exited with status %d\n", WEXITSTATUS(status));
        }
    }
}

void hist(){
    pid_t child_pid = fork();

    if (child_pid == -1) {
        perror("fork");
        return;
    }

    if (child_pid == 0) {
        for (int i=0; i<hist_indx; i++){
            printf("%s\n", history[i].command);
        }
        exit(0);
    } 
    else {
        int status;

        waitpid(child_pid, &status, 0);
        history[hist_indx].child_pid = child_pid;
        // printf("Child PID: %d\n", child_pid);
    }
}

void wc(char* input) {

}