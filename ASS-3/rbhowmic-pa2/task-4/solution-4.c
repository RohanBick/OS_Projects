/* Your code goes here */
/* Your code goes here */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_INPUT_SIZE 1024
int main() {
    char cmd[MAX_INPUT_SIZE];
    char *token;
    while (1) {
    //Reading input from user
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            fprintf(stderr, "Error has occurred\n");
            exit(1);
        }
        if (cmd[strlen(cmd) - 1] == '\n') {
            cmd[strlen(cmd) - 1] = '\0';
        }
        if (strcmp(cmd, "exit") == 0) {
            break;
        }
        token = strtok(cmd, " \t");
        while (token != NULL) {
            printf("%s\n", token);
            token = strtok(NULL, " \t");
        }
    }
    return 0;
}
