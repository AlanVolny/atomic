
#ifndef ATOMIC_H
#define ATOMIC_H


typedef struct game_state game_state;


#include "atom.h"


/* atomic.h: general game state defintions */


void error_n_escape(); /* A bad error has occured, immediately quit the game */


struct game_state
{
	/* our game's periodic table equivalent */
	atom_chars *aType_table;
	int n_aTypes;

	/* where the atoms are held.*/
	atomDat *atoms;
	int n_atoms;

	/* friction coefficient */
	float friction;
};



#endif

