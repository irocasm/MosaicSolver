#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "mosaic_utils.c"
#include "mosaic_solver.c"

void help_text(char* cmd) {
    char* help_str = "Usage:\n\
\t%s [options] [file]\n\
\n\
file\t\tfile to read the puzzle from [default=STDIN]\n\
\n\
-s, --show\tshow the solution\n\
-p, --print\tprint the mosaic before solving\n\
--help\t\tshow this help\n";
    printf(help_str, cmd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int arg_print = 0;
    int arg_show = 0;
    int width = 0, height = 0; // width and height of puzzle
    uint8_t *m_contents; // puzzle data
                       // bit-layout per field/byte: cxssnnnn
                       // c: completed flag (all neighbours of this field are coloured)
                       // s: state (NONE, BLACK, WHITE)
                       // n: number (0-9 or 10 if no number in input)

    fp = stdin;
    // handle args/command line parameters
    size_t argn = 1;
    while (argn < argc) {
        if (argv[argn][0] != '-') {
            fp = fopen(argv[argn], "rb");
            argn++;
            continue;
        }
        if (strcmp(argv[argn], "--help") == 0) {
            help_text(argv[0]);
            return EXIT_SUCCESS;
        }
        if (strcmp(argv[argn], "-p") == 0 || strcmp(argv[argn], "--print") == 0) {
            arg_print = 1;
        } else
        if (strcmp(argv[argn], "-s") == 0 || strcmp(argv[argn], "--show") == 0) {
            arg_show = 1;
        } else {
            fprintf(stderr, "unknown parameter: %s\n", argv[argn]);
        }
        argn++;
    }
    if (fp == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }
    int err = get_mosaic_dimension(fp, &width, &height); // reads the file and stores the dimension in "width" and "height"
    if (err) {
        return EXIT_FAILURE;
    }
    //printf("width: %d, height: %d\n", width, height);
    m_contents = (uint8_t*) malloc(width * height * sizeof(uint8_t));
    if (m_contents == NULL) {
        fputs("ERROR: not sufficient memory!\n", stderr);
        return EXIT_FAILURE;
    }
    err = read_mosaic_file(fp, width, height, m_contents); // reads the file and stores the numbers in "m_contents"
    if (err) {
        return EXIT_FAILURE;
    }
    fclose(fp);
    // file has been read and is stored in m_contents

    if (arg_print) {
        print_mosaic_field_num(width, height, m_contents);
    }
    game_state st = solve_easy_steps(width, height, m_contents);
    if (st == SOLVED) {
        puts("EASY SOLVABLE");
        if (arg_show) {
            print_mosaic_field_colour(width, height, m_contents);
        }
        return EXIT_SUCCESS;
    }
    
    
    
    //free(m_contents);
    return EXIT_SUCCESS;
}
