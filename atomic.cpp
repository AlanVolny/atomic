

#include <cstdio>
#include <cstdlib>

#include <unistd.h>

#include "atomic.h"
#include "atom.h"
#include "term_disp.h"
#include "game_step.h"


// int frame_width  = 48;
// int frame_height = 48;

int frame_width  = 100;
int frame_height = 50;


/* the main file of atomic, the atom physics engine */

game_state *init_gameState();

void main_loop(game_state *state);

void clean_up(game_state *state);


int main(int argc, char *argv[])
{
	game_state *state = init_gameState();
	// state->friction = 0; /* global friction coefficient */
	state->friction = 0.3; /* global friction coefficient */
	// state->friction = 0.999; /* global friction coefficient */


	// init_rand_atoms(state, 13, atype_air, frame_width, frame_height);


	// init_rand_atoms(state, 26, atype_metal, frame_width, frame_height);
	// init_rand_atoms(state, 26, atype_water, frame_width, frame_height);
	// init_rand_atoms(state, 26, atype_air, frame_width, frame_height);
	// init_rand_atoms(state, 26, atype_x, frame_width, frame_height);


	// init_rand_atoms(state, 52, atype_metal, frame_width, frame_height);
	// init_rand_atoms(state, 52, atype_air, frame_width, frame_height);
	init_rand_atoms(state, 52, atype_water, frame_width, frame_height);
	// init_rand_atoms(state, 52, atype_x, frame_width, frame_height);



	bond_atoms(state);


	print_atoms_to_term(state, frame_width, frame_height);
	main_loop(state);

	clean_up(state);
	free(state);

}


void main_loop(game_state *state)
{
	int running=1;

	while (running) {

		//less granularity. for more efficient terminals
		update_atoms(state, 0.01, frame_width, frame_height);
		usleep(5000);

		// update_atoms(state, 0.1, frame_width, frame_height);
		// usleep(50000);  //normal time, with interval of .1

		//Most granularity
		// update_atoms(state, 0.16, frame_width, frame_height);
		// usleep(80000); //fine for anything

		print_atoms_to_term(state, frame_width, frame_height);
	}
}



game_state *init_gameState()
{
	printf("atomic: initing game state\n");

	game_state *state;
	state = (game_state *) calloc(sizeof(game_state), 1);

	load_atomTypes(state);
	state->n_atoms = 0;
	state->atoms = NULL;

	printf("atomic: finished initing game state\n");

	return state;
}




void clean_up(game_state *state)
{
	int a;

	for (a=0; a<state->n_atoms; a++){
		free(state->atoms[a].bonds);
	}

	free(state->atoms);
	free(state->aType_table);
}




/* disgracefully exit */
void error_n_escape()
{
	printf("Atom Physics Engine: error has occured, quitting immediately\n");
	exit(EXIT_FAILURE);
}
