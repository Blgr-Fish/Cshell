#include "executor.h"

int handle_command(Line words, int last_status){

    int shell_status = last_status ;

    // executing all the commands with their args if they have some
    for (int tcmds = 0 ; tcmds < words.totalcmds ; ++tcmds) {

        if ( words.cmds[tcmds].argv[0] != NULL) {

            if ((words.cmds[tcmds].ended == REDIRECT_OUT || words.cmds[tcmds].ended == REDIRECT_OUT_APPEND || words.cmds[tcmds].ended == REDIRECT_IN)) {
                continue ;
            } 
            else if (tcmds == 0) {
                shell_status = exec_word(words.cmds[tcmds]);
                continue;
            }

           else {
                switch (words.cmds[tcmds-1].ended) {
                    case SEMICOLON:
                        shell_status = exec_word(words.cmds[tcmds]);
                        break;
                    case AND_AND:
                        if (shell_status == SHELL_VALID)
                            shell_status = exec_word(words.cmds[tcmds]);
                        break;
                    case OR_OR:
                        if (shell_status != SHELL_VALID)
                            shell_status = exec_word(words.cmds[tcmds]);
                        break;
                    case PIPE:
                        // TODO 
                        break;
                    case REDIRECT_IN: // <
                        shell_status = file_to_stdin( shell_status, words, tcmds);
                        break;
                    case REDIRECT_OUT:
                        shell_status = write_to_file("w", shell_status, words, tcmds);
                        break;
                    case REDIRECT_OUT_APPEND:
                        shell_status = write_to_file("a", shell_status, words, tcmds);
                        break;
                    default:
                        shell_status = exec_word(words.cmds[tcmds]);
                        break;
                }
            }

        }        
    }
        return shell_status ;
}

/* Execute a command */
int exec_word(Command command) {

    
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);

    pid_t child_pid ;
    int exec_status ;

    replace_tild(command.argv);
        
    if (strcmp(command.argv[0],"exit") == 0) {
        return exec_exit();
    }
    if (strcmp(command.argv[0],"cd") == 0) {
        return exec_cd(command.argv);
    }
    if (strcmp(command.argv[0],"history") == 0) {
        return exec_history();
    }

    child_pid = fork() ;

    if (child_pid == 0) {
        if (execvp(command.argv[0], command.argv) < 0) {
            printf("error: command doesn't exist : %s\n", command.argv[0]);
            exit(SHELL_UNKOWN_COMMAND);
        }
    } else if (child_pid == -1) {
        fprintf(stderr, "error: process creation error\n");
        return SHELL_ERROR;
    } else {
        if (command.background == 0) {
            waitpid(child_pid, &exec_status, 0) ; // 0 since we don't need any option for now
            if (WIFEXITED(exec_status)) {
                return WEXITSTATUS(exec_status) ;
            }
        }  else {
            sa.sa_handler = &ZombieHandler;
            sigaction(SIGCHLD, &sa, NULL);  // set sa as sigaction when SIGCHLD signal occurs
            printf("[-] %d in background\n", child_pid);
            return SHELL_VALID ;
        } 
    }
    
    return SHELL_ERROR ;
}

void replace_tild(char ** argv){

    int a = 0 ;
    int b = 1 ;
    char * home = getenv("HOME") ; 

    if (!home) return ;

    while (argv[a] != NULL ) {
            if (argv[a][0] == '~') {
                char * newArg = malloc(strlen(home) + strlen(argv[a]) + 1 );
                strcpy(newArg, home);
                while(argv[a][b]){
                    strncat(newArg,&argv[a][b],1);
                    b++ ; 
                }
                b = 1 ;
                free(argv[a]);
                argv[a] = newArg ;
            }
            a++;
        }
}

int exec_exit() {
    return SHELL_EXIT ;
}

int exec_cd(char ** words) {

    int status = SHELL_VALID ;
    char * d ;
    
    if (words[1] == NULL) {
        d = strdup(getenv("HOME"));
        
    } else {
        d = strdup(words[1]);
    }

    if (chdir(d) != 0) {
        printf("error: directory doesn't exist : %s\n", d);
        status = SHELL_ERROR ;
    }
    free(d);
    return status ;
}

int exec_history() {

    for (int i = 0 ; i < history.total_lines ; ++i){
        printf("%d : %s\n",i+1, history.lines[i]);
    }


    return SHELL_VALID;
}

// >, >>
int  write_to_file(char * type, int shell_status, Line words, int tcmds) {
    FILE * inputFile = fopen(words.cmds[tcmds].argv[0], type) ;
    if ( !inputFile ) {
        printf( "error; no such file or directory: %s\n", words.cmds[tcmds].argv[0]);
        shell_status = SHELL_ERROR ;
    } else {
        int  stdout_bk = dup(fileno(stdout));
        dup2(fileno(inputFile), fileno(stdout));
        
        shell_status = exec_word(words.cmds[tcmds-1]);
        
        dup2(stdout_bk, fileno(stdout));
        close(stdout_bk);
        fclose(inputFile);
    }
    
    return shell_status ;
}

// <
int file_to_stdin(int shell_status, Line words, int tcmds) {
    
    FILE * inputFile = fopen(words.cmds[tcmds].argv[0], "r") ;
    if ( !inputFile ) {
        printf( "error; no such file or directory: %s\n", words.cmds[tcmds].argv[0]);
        shell_status = SHELL_ERROR ;
    } else {
        int  stdin_bk = dup(fileno(stdin));
        dup2(fileno(inputFile), fileno(stdin));
        
        shell_status = exec_word(words.cmds[tcmds-1]);
        
        dup2(stdin_bk, fileno(stdin));
        close(stdin_bk);
        fclose(inputFile);
    }
    
    return shell_status ;

}

void ZombieHandler(int signal_num) 
{
    pid_t pid ;
    char buf[64] ;
    int len ;
    
    while ((pid = waitpid(-1, NULL, WNOHANG)) > 0){
        len = snprintf(buf, sizeof(buf), "[-] %d terminated\n", pid);
        write(STDOUT_FILENO, buf, len);
    }
    
}