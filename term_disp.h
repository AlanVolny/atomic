


#ifndef TEST_H
#define TEST_H


#include "atomic.h"
#include "atom.h"

/* test.h: a bunch of misc. functions for testing the correct functioning and performance
 * 	of atomic phsyics engine. Some functions may for testing ideas may become official functions */


/* print atom names, locations, and bond partners */
void print_state(game_state *state);

/* print the atoms to the terminal */
void print_atoms_to_term(game_state *state, int max_x, int max_y);


#endif
