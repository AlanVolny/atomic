


#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "atom.h"


/* atom.cpp: code dealing with basic functions on atoms */




/* atom type name and enum getters */

char *    getAtomName(atom_type type){
	if (type==atype_water) return "water";
	if (type==atype_metal) return "metal";
	if (type==atype_air  ) return "air";
	printf("Received invalid atom type! Quitting\n");
	error_n_escape();
	return NULL;
}

atom_type getAtomType(char *name){
	if (!strcmp("water", name)) return atype_water;
	if (!strcmp("metal", name)) return atype_metal;
	if (!strcmp("air", name))   return atype_air;
	printf("Received invalid atom name! Quitting\n");
	error_n_escape();
	return atype_INVALID;
}


atom_chars *atom_prop(game_state *state, atom_type type){
	return state->aType_table + type;
}




/* load atom types into the game state */
void load_atomTypes(game_state *state)
{
	state->n_aTypes = N_ATYPES;

	/* allocate space */
	state->aType_table = (atom_chars *) calloc(sizeof(atom_chars), state->n_aTypes);


	/* put in the (for now) hardcoded values */
	atom_chars *achars;

		/* water */
	achars = state->aType_table + atype_water;
	achars->color = "\x1B[34;1m";
	achars->min_bond_len  = 4.0;
	achars->ideal_bond_len= 4.5;
	achars->max_bond_len  = 12.0;
	
	achars->bond_strength = 0.8;
	achars->compress_resistForce = 4;
	achars->weight        = 5.0;


		/* metal */
	achars = state->aType_table + atype_metal;
	achars->color = "\x1B[37;1m";
	achars->min_bond_len  = 4.5;
	achars->ideal_bond_len= 5.5;
	achars->max_bond_len  = 8.0;
	achars->compress_resistForce = 1.8;

	achars->bond_strength = 0.2;
	achars->weight        = 9.0;


		/* air */
	achars = state->aType_table + atype_air;
	achars->color = "\x1B[36;1m";
	// achars->min_bond_len  = 9.0;

	achars->min_bond_len  = 8.0;
	achars->ideal_bond_len= 9.0;
	achars->max_bond_len  = 9.0;

	achars->bond_strength = 0.00;
	achars->compress_resistForce = 0.2;
	achars->weight        = 0.1;



		/* Element X */
	achars = state->aType_table + atype_x;
	achars->color = "\x1B[32;1m";
	achars->min_bond_len  = 12.0;
	achars->ideal_bond_len= 14.0;
	achars->max_bond_len  = 16.0;

	achars->compress_resistForce = 0.4; /* can and do stabilize next to each other */
	// achars->compress_resistForce = 0.5; /* cannot stabilize next to each other */
	achars->bond_strength = 0.1;
	achars->weight        = 6.0;

}









