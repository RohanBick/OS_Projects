/* Your code goes here */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define MAX_LINE_LENGTH 1024
int execute_command(char *command, char *arg1, char *arg2) {
    pid_t pid;
    int status;
    if ((pid = fork()) == -1) {
        perror("fork");
        return 1;
    }
    if (pid == 0) { 
        char *args[] = {command, arg1, arg2, NULL};
        execvp(command, args);
        perror("execvp");  // execvp returns only on error
        exit(EXIT_FAILURE);
    } else {  
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 0;
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <commands-file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("fopen");
        fprintf(stderr, "Error has occurred\n");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), fp)) {
        char command[10], arg1[256], arg2[256];
        if (sscanf(line, "%9s %255s %255s", command, arg1, arg2) != 3) {
            fprintf(stderr, "Error has occurred\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        if (strcmp(command, "cp") != 0 && strcmp(command, "mv") != 0) {
            fprintf(stderr, "Error has occurred\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
        if (execute_command(command, arg1, arg2) != 0) {
            fprintf(stderr, "Error has occurred\n");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
    return 0;
}

