/**
 * returns 0 if successful, 1 otherwise
 */
int get_mosaic_dimension(FILE *file, int *width, int *height) {
    int c = fscanf(file, "%d\n%d\n", width, height); // get width and height
    if (c != 2) {
        fputs("ERROR while reading width and height!\n", stderr);
        return EXIT_FAILURE;
    }
    if (*width <= 0 || *height <= 0) {
        fputs("ERROR: width and height must be greater than 0!\n", stderr);
        return EXIT_FAILURE;
    }
    if (*width >= 10000 || *height >= 10000) {
        fputs("ERROR: width and height must be less than 10000!\n", stderr);
        return EXIT_FAILURE;
    }
    assert(*width > 0 && *height >= 0);
    assert(*width < 10000 && *height < 10000);
    return EXIT_SUCCESS;
}
/**
 * returns 0 if successful, 1 otherwise
 */
int read_mosaic_file(FILE *file, int width, int height, uint8_t *fcontents) {
    assert(width > 0 && height >= 0);
    assert(width < 10000 && height < 10000);
    
    int col_count = 0;
    int row_count = 0;
    int i = 0;
    int c;
    while ((c = fgetc(file)) != EOF) { // read file byte by bytes
        if (row_count == height) {
            break;
        }
        assert(col_count <= width + 1);
        assert(row_count < height);
        if (col_count < width) {
            // c is the number of black neighbours (0-9 or .)
            if (c >= '0' && c <= '9') {
                fcontents[i] = c - '0';
                i++;
                col_count++;
            } else if (c == '.') {
                fcontents[i] = 10;
                i++;
                col_count++;
            } else if (c == '\r' || c == '\n') {
                fprintf(stderr, "ERROR line %d too short!\n", row_count + 3);
                return EXIT_FAILURE;
            } else {
                fprintf(stderr, "ERROR invalid symbol '%c' in line %d!\n", c, row_count + 3);
                return EXIT_FAILURE;
            }
        } else if (col_count == width) {
            // expect new line (CR or LF)
            if (c == '\n') {
                col_count = 0;
                row_count++;
            } else if (c == '\r') {
                col_count++;
            } else {
                fprintf(stderr, "ERROR line %d too long!\n", row_count + 3);
                return EXIT_FAILURE;
            }
        } else {
            assert(col_count == width + 1);
            // expect LF (after CR)
            if (c != '\n') {
                fprintf(stderr, "ERROR expected new line character at line %d\n", row_count + 3);
                return EXIT_FAILURE;
            }
            col_count = 0;
            row_count++;
        }
    }

    if (feof(file)) {
        // reached end of file
        if (i != width * height) {
            //fputs("ERROR too few or many fields\n", stderr);
            fputs("ERROR too few lines\n", stderr);
            return EXIT_FAILURE;
        }
    } else if (ferror(file)) {
        fputs("I/O error when reading\n", stderr);
        return EXIT_FAILURE;
    } else {
        fputs("ERROR too many lines\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
