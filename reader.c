#include "reader.h"
#include <termios.h>

static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); 
}

void enable_raw_mode() {
    struct termios raw;
    atexit(disable_raw_mode);
    tcgetattr(STDIN_FILENO, &orig_termios);
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

/* Read a line from the standard input */
char * read_line() {
    int buffer_size = BUFFER_SIZE ;
    int position = 0 ;

    char * buffer = malloc(sizeof(char) * buffer_size) ;

    char c ;

    // no space for allocation
    if (!buffer) {
        fprintf(stderr, "error: Allocation error\n") ;
        exit(EXIT_FAILURE) ;
    }

    while (1) {

        c = getchar() ;

        // enf of line
        if (c == EOF || c == '\n') {
            buffer[position] = '\0' ;
            printf("\n");
            return buffer ;
        } 
        // delete character
        else if (c == DELETE_CHAR) {
            
            if (position > 0) {
                position -- ;
                printf("\b \b") ;
                fflush(stdout) ;
            }
        // only for arrow characters for now
        }else if (c == ESC_CHAR) {
            
            // arrows are composed of 3 bytes, so we need to take of the 2 next chars
            char seq1 = getchar();
            char seq2 = getchar();
                /* 
                * for later if i want to handle this...
                    switch (seq2) {
                        case 'A': // up
                        case 'B': // down
                        case 'C': // right
                        case 'D': // left
                    }
                */

        } else {
            buffer[position++] = c ;
            printf("%c", c);
        }

        if (position >= buffer_size) {

            buffer_size += BUFFER_SIZE ;
            buffer = realloc(buffer, sizeof(char)* buffer_size);

            // no space for allocation
            if (!buffer) {
                fprintf(stderr, "error: Allocation error\n") ;
                exit(EXIT_FAILURE) ;
            }
        }
    }
}