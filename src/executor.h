#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "utils.h"



int exec_word(Command command);

// built-in methods
int exec_exit();
int exec_cd(char ** args);
int exec_history();
void replace_tild(char ** argv);
int handle_command(Line line, int last_status);
int write_to_file(char * type, int shell_status, Line words, int tcmds);
int file_to_stdin(int shell_status, Line words, int tcmds) ;
void ZombieHandler(int signal_num) ;

#endif