


#ifndef GAME_STEP_H
#define GAME_STEP_H


#include "atomic.h"
#include "atom.h"

	/* for physics calculations */
struct xy_pair
{
	float x;
	float y;
};


/* randomly create atoms with the given params */
void init_rand_atoms(game_state *state, int n_atoms, atom_type atype, int max_x, int max_y);

/* bond any atoms that are close enough */
void bond_atoms(game_state *state);

/* update the speed and positions of all atoms */
void update_atoms(game_state *state, float dt, float frame_w, float frame_h);

#endif
