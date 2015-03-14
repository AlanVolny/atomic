

#ifndef ATOM_H
#define ATOM_H


typedef struct atom_chars atom_chars;
typedef struct atomDat atomDat;


#include "atomic.h"

/* defines an atom, and basic functions on atoms */


/* the types of atoms. Hardcoded for now. 
 * 	If modified, update the getter funs. */
#define N_ATYPES 5

enum atom_type {
	atype_INVALID,
	atype_water,
	atype_metal,
	atype_air,
	atype_x
};

char *      getAtomName(atom_type type);
atom_type   getAtomType(char *name);
atom_chars *atom_prop(game_state *state, atom_type type);



/* the characteristics of atoms */
struct atom_chars
{
	char *color; /* color is VT100 terminal color code */
	float min_bond_len;	/* minimum bond length before compress_resistForce acts */
	float max_bond_len;
	float ideal_bond_len; /* with stregnth bond_strength, returns to this length */

	float bond_strength;
	float compress_resistForce; /* decompress force */

	// float friction;

	float weight; /* the weight of each atom */
};



struct atomDat
{
	atom_type type;

	float x;
	float y;

	float xspeed;
	float yspeed;

	char n_bonds;
	int *bonds; /* number IDs of bonded atoms */
};


/* load all atom types into the game state */
void load_atomTypes(game_state *state);


#endif
