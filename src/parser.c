#include "parser.h"


/* Parse a line into tokens */
Line parse_line( char * line) {
    int buffer_size = BUFFER_SIZE ;
    int position = 0 ;

    char * t ;
    //char ** tokens = malloc(sizeof(char*) * buffer_size);
    Line pline;
    pline.cmds = malloc(sizeof(Command) * MAX_CMDS);
    pline.totalcmds = 0 ;
    pline.cmds[pline.totalcmds].argv = malloc(sizeof(char*) * buffer_size);
    


    if (!pline.cmds[pline.totalcmds].argv) {
        fprintf(stderr, "error: Allocation error\n");
        exit(EXIT_FAILURE);
    }

    t = strtok(line, TOKEN_DELIMITER) ;
    while (t != NULL) {


        if (strcmp(t, ";") == 0 || strcmp(t, "&&") == 0 || strcmp(t,"||") == 0 || strcmp(t,">") == 0) {

            if (strcmp(t, "&&") == 0) {
                pline.cmds[pline.totalcmds].ended = AND_AND ;// && code
            } else if(strcmp(t,"||") == 0) {
                pline.cmds[pline.totalcmds].ended = OR_OR ;
            } else if(strcmp(t,">") == 0) {
                pline.cmds[pline.totalcmds].ended = REDIRECT_OUT ;
            }
             else {
                pline.cmds[pline.totalcmds].ended = SEMICOLON ; 
            }
            
            // add a NULL to the command to finish it
            pline.cmds[pline.totalcmds].argv[position] = NULL ;
            
            // reset values for new cmd
            position = 0 ;
            buffer_size = BUFFER_SIZE ;
            pline.totalcmds ++ ;

            if (pline.totalcmds >= MAX_CMDS) {
                fprintf(stderr, "error: too many commands\n");
                exit(EXIT_FAILURE);
            }

            pline.cmds[pline.totalcmds].argv = malloc(sizeof(char*) * buffer_size);

            if (!pline.cmds[pline.totalcmds].argv) {
                fprintf(stderr, "error: Allocation error\n");
                exit(EXIT_FAILURE);
            }

            
        } else {
            // so we can free argv later 
            pline.cmds[pline.totalcmds].argv[position++] = strdup(t) ;
        }
        
        if (position >= buffer_size) {
            buffer_size += BUFFER_SIZE ;
            pline.cmds[pline.totalcmds].argv = realloc(pline.cmds[pline.totalcmds].argv,sizeof(char * )* buffer_size);

            if (!pline.cmds[pline.totalcmds].argv) {
                fprintf(stderr, "error: Allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        t = strtok(NULL, TOKEN_DELIMITER);

    }

    // to ensure the end of the word
    pline.cmds[pline.totalcmds].argv[position] = NULL ;
    pline.cmds[pline.totalcmds].ended = SEMICOLON ; 
    pline.totalcmds++;
    return pline ;

}