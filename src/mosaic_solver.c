#define F_NOCOLOUR 0
#define F_BLACK 0x10
#define F_WHITE 0x20
#define F_NUMBER(i, field) ((field[i]) & 0xF)
#define F_GETCOLOUR(i, field) ((field[i]) & 0x30)
#define F_SETCOLOUR(i, field, colour) field[i] = (field[i] & ~0x30) | colour;
#define F_ISCOMPLETE(i, field) (((field[i]) & 0x80) == 0x80)
#define F_MARKCOMPLETE(i, field) field[i] |= 0x80;

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
    // TODO
    //n.push(i);
    if (i%width != 0) { //not at the left
        //n.push(i-1);
        if (i >= width) { //not at the top
            //n.push(i-1-width);
        }
        if (i < (height-1)*width) { //not at the bottom
            //n.push(i-1+width);
        }
    }
    if (i%width != width-1) { //not at the right
        //n.push(i+1);
        if (i >= width) { //not at the top
            //n.push(i+1-width);
        }
        if (i < (height-1)*width) { //not at the bottom
            //n.push(i+1+width);
        }
    }
    if (i >= width) { //not at the top
        //n.push(i-width);
    }
    if (i < (height-1)*width) { //not at the bottom
        //n.push(i+width);
    }
}
/**
 * param colour: 0x10 BLACK, 0x20 WHITE
 * returns 1 if something changed, 0 otherwise
 */
static int set_neighbours_to(int i, int width, int height, uint8_t *field, int colour) {
    assert(i >= 0 && i < width * height);
    int something_changed = 0;
    // TODO
    //n.push(i);
    if (i%width != 0) { //not at the left
        //n.push(i-1);
        if (i >= width) { //not at the top
            //n.push(i-1-width);
        }
        if (i < (height-1)*width) { //not at the bottom
            //n.push(i-1+width);
        }
    }
    if (i%width != width-1) { //not at the right
        //n.push(i+1);
        if (i >= width) { //not at the top
            //n.push(i+1-width);
        }
        if (i < (height-1)*width) { //not at the bottom
            //n.push(i+1+width);
        }
    }
    if (i >= width) { //not at the top
        //n.push(i-width);
    }
    if (i < (height-1)*width) { //not at the bottom
        //n.push(i+width);
    }
    return something_changed;
}

typedef enum {
   NOT_SOLVED,
   SOLVED,
   NOT_SOLVABLE
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
    return SOLVED;
}