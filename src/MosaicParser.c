#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include "mosaic_utils.c"
#include "mosaic_solver.c"

void help_text(char cmd[]) {
    printf("Usage:\n\t%s [file]\n\nfile\t\tfile to read the puzzle from [optional; default=STDIN]]", cmd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
    FILE *fp;
    int width = 0, height = 0; // width and height of puzzle
    uint8_t *m_contents; // puzzle data
                       // bit-layout per field/byte: cxssnnnn
                       // c: completed flag (all neighbours of this field are coloured)
                       // s: state (NONE, BLACK, WHITE)
                       // n: number (0-9 or 10 if no number in input)

    if (argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            help_text(argv[0]);
            return EXIT_SUCCESS;
        }
        fp = fopen(argv[1], "rb");
    } else {
        fp = stdin;
    }
    if (fp == NULL) {
        perror("Failed to open file.\n");
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
    
    // for (int i = 0; i < width * height; i++) {
        // printf("%c", m_contents[i] + '0');
    // }
    
    
    
    //free(m_contents);
    return EXIT_SUCCESS;
}