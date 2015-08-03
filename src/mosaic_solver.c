#include "mosaic_macro.h"

typedef struct {
    uint8_t sum;
    //uint8_t colourless;
    uint8_t black;
    uint8_t white;
} neighbour_stats;
/**
 * stats: output parameter, assumed to be initialised to all zeros
 */
static void get_neighbour_stats(int i, int width, int height, uint8_t *field, neighbour_stats *stats) {
    assert(i >= 0 && i < width * height);
    assert(stats->sum == 0 && stats->black == 0 && stats->white == 0);

    stats->sum++;
    if (F_GETCOLOUR(i, field) == F_BLACK) {
        stats->black++;
    } else if (F_GETCOLOUR(i, field) == F_WHITE) {
        stats->white++;
    }
    if (i%width != 0) { //not at the left
        stats->sum++;
        if (F_GETCOLOUR(i-1, field) == F_BLACK) {
            stats->black++;
        } else if (F_GETCOLOUR(i-1, field) == F_WHITE) {
            stats->white++;
        }
        if (i >= width) { //not at the top
            stats->sum++;
            if (F_GETCOLOUR(i-1-width, field) == F_BLACK) {
                stats->black++;
            } else if (F_GETCOLOUR(i-1-width, field) == F_WHITE) {
                stats->white++;
            }
        }
        if (i < (height-1)*width) { //not at the bottom
            stats->sum++;
            if (F_GETCOLOUR(i-1+width, field) == F_BLACK) {
                stats->black++;
            } else if (F_GETCOLOUR(i-1+width, field) == F_WHITE) {
                stats->white++;
            }
        }
    }
    if (i%width != width-1) { //not at the right
        stats->sum++;
        if (F_GETCOLOUR(i+1, field) == F_BLACK) {
            stats->black++;
        } else if (F_GETCOLOUR(i+1, field) == F_WHITE) {
            stats->white++;
        }
        if (i >= width) { //not at the top
            stats->sum++;
            if (F_GETCOLOUR(i+1-width, field) == F_BLACK) {
                stats->black++;
            } else if (F_GETCOLOUR(i+1-width, field) == F_WHITE) {
                stats->white++;
            }
        }
        if (i < (height-1)*width) { //not at the bottom
            stats->sum++;
            if (F_GETCOLOUR(i+1+width, field) == F_BLACK) {
                stats->black++;
            } else if (F_GETCOLOUR(i+1+width, field) == F_WHITE) {
                stats->white++;
            }
        }
    }
    if (i >= width) { //not at the top
        stats->sum++;
        if (F_GETCOLOUR(i-width, field) == F_BLACK) {
            stats->black++;
        } else if (F_GETCOLOUR(i-width, field) == F_WHITE) {
            stats->white++;
        }
    }
    if (i < (height-1)*width) { //not at the bottom
        stats->sum++;
        if (F_GETCOLOUR(i+width, field) == F_BLACK) {
            stats->black++;
        } else if (F_GETCOLOUR(i+width, field) == F_WHITE) {
            stats->white++;
        }
    }
}
/**
 * Sets all unset neighbours of of cell i to the specified colour.
 *
 * param colour: 0x10 BLACK, 0x20 WHITE
 * returns 1 if something changed, 0 otherwise
 */
static int set_neighbours_to(int i, int width, int height, uint8_t *field, int colour) {
    assert(i >= 0 && i < width * height);
    assert(colour == F_BLACK || colour == F_WHITE);
    int something_changed = 0;

    if (F_GETCOLOUR(i, field) == F_NOCOLOUR) {
        F_SETCOLOUR(i, field, colour);
        assert(F_GETCOLOUR(i, field) == colour);
        something_changed = 1;
    }
    if (i%width != 0) { //not at the left
        if (F_GETCOLOUR(i-1, field) == F_NOCOLOUR) {
            F_SETCOLOUR(i-1, field, colour);
            assert(F_GETCOLOUR(i-1, field) == colour);
            something_changed = 1;
        }
        if (i >= width) { //not at the top
            if (F_GETCOLOUR(i-1-width, field) == F_NOCOLOUR) {
                F_SETCOLOUR(i-1-width, field, colour);
                assert(F_GETCOLOUR(i-1-width, field) == colour);
                something_changed = 1;
            }
        }
        if (i < (height-1)*width) { //not at the bottom
            if (F_GETCOLOUR(i-1+width, field) == F_NOCOLOUR) {
                F_SETCOLOUR(i-1+width, field, colour);
                assert(F_GETCOLOUR(i-1+width, field) == colour);
                something_changed = 1;
            }
        }
    }
    if (i%width != width-1) { //not at the right
        if (F_GETCOLOUR(i+1, field) == F_NOCOLOUR) {
            F_SETCOLOUR(i+1, field, colour);
            assert(F_GETCOLOUR(i+1, field) == colour);
            something_changed = 1;
        }
        if (i >= width) { //not at the top
            if (F_GETCOLOUR(i+1-width, field) == F_NOCOLOUR) {
                F_SETCOLOUR(i+1-width, field, colour);
                assert(F_GETCOLOUR(i+1-width, field) == colour);
                something_changed = 1;
            }
        }
        if (i < (height-1)*width) { //not at the bottom
            if (F_GETCOLOUR(i+1+width, field) == F_NOCOLOUR) {
                F_SETCOLOUR(i+1+width, field, colour);
                assert(F_GETCOLOUR(i+1+width, field) == colour);
                something_changed = 1;
            }
        }
    }
    if (i >= width) { //not at the top
        if (F_GETCOLOUR(i-width, field) == F_NOCOLOUR) {
            F_SETCOLOUR(i-width, field, colour);
            assert(F_GETCOLOUR(i-width, field) == colour);
            something_changed = 1;
        }
    }
    if (i < (height-1)*width) { //not at the bottom
        if (F_GETCOLOUR(i+width, field) == F_NOCOLOUR) {
            F_SETCOLOUR(i+width, field, colour);
            assert(F_GETCOLOUR(i+width, field) == colour);
            something_changed = 1;
        }
    }
    return something_changed;
}

typedef enum {
   NOT_SOLVED,
   SOLVED,
   NOT_SOLVABLE // TODO: divide in CONFLICT and UNSUFFICIENT_INFO?
} game_state;
/**
 * Solves a (possibly partially solved) mosaic puzzle
 * using only easy steps.
 */
game_state solve_easy_steps(int width, int height, uint8_t *field) {
    neighbour_stats stats = {0, 0, 0};
    int something_changed = 1;
    while (something_changed) {
        something_changed = 0;
        for (int i = 0; i < width * height; i++) {
            //check if there is no number or the field is already completed
            if (F_NUMBER(i, field) == 10 || F_ISCOMPLETE(i, field)) {
                continue;
            }
            stats.sum = 0; //reset stats
            stats.black = 0;
            stats.white = 0;
            get_neighbour_stats(i, width, height, field, &stats);
            if (stats.black == F_NUMBER(i, field)) {
                // colour 
                something_changed |= set_neighbours_to(i, width, height, field, F_WHITE);
                F_MARKCOMPLETE(i, field)
                continue;
            }
            if (stats.sum - stats.white == F_NUMBER(i, field)) {
                something_changed |= set_neighbours_to(i, width, height, field, F_BLACK);
                F_MARKCOMPLETE(i, field)
                continue;
            }
            if (stats.black > F_NUMBER(i, field) || stats.sum - stats.white < F_NUMBER(i, field)) {
                return NOT_SOLVABLE;
            }
        }
    }
    for (int i = 0; i < width * height; i++) {
        if (F_GETCOLOUR(i, field) == F_NOCOLOUR) {
            return NOT_SOLVED;
        }
    }
    #ifndef NDEBUG
    for (int i = 0; i < width * height; i++) {
        assert(F_NUMBER(i, field) == 10 || F_ISCOMPLETE(i, field));
    }
    #endif
    return SOLVED;
}
