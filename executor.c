#include "executor.h"

/* Execute a command */
int exec_word(Command command) {
    pid_t child_pid ;
    int exec_status ;

   
        if (strcmp(command.argv[0],"exit") == 0) {
            return exec_exit(command.argv);
        }
        if (strcmp(command.argv[0],"cd") == 0) {
            return exec_cd(command.argv);
        }

        child_pid = fork() ;

        if (child_pid == 0) {
            if (execvp(command.argv[0], command.argv) < 0) {
                printf("error: command doesn't exist : %s\n", command.argv[0]);
                exit(EXIT_FAILURE);
            }
        } else if (child_pid == -1) {
            fprintf(stderr, "error: process creation error\n");
            exit(EXIT_FAILURE);
        } else {
            waitpid(child_pid, &exec_status, 0) ; // 0 since we don't need any option for now
        }
    
    

    return 1 ;

}

int exec_exit(char ** words) {
    return 0 ;
}

int exec_cd(char ** words) {

    int status = 1 ;

    if (words[1] == NULL || strcmp(words[1], "~") == 0) {
        words[1] = getenv("HOME") ;
    }


    if (chdir(words[1]) != 0) {
        printf("error: directory doesn't exist : %s\n", words[1]);
        status = -1 ;
    }


    return status ;
}