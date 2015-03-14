


#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <math.h>

#include "game_step.h"



/* randomly create atoms with the given params */
void init_rand_atoms(game_state *state, int n_atoms, atom_type atype, int max_x, int max_y)
{
	int i, j;
	float x, y;
	atomDat atom;

	printf("randomly creating atoms\n");

		/* instantiate stuff */
	time_t t;
	t = time(NULL);
	// t = 1379410662;
	printf("seed is %ld\n", t);
	srand(t);
	int atoms_start = 0;
	if (state->n_atoms==0){
		printf("adding new atoms to pristine place\n");
		state->n_atoms = n_atoms;
		state->atoms = (atomDat *) calloc(sizeof(atomDat), state->n_atoms);
	}
	else {
		printf("adding new atoms to existing place");
		atoms_start = state->n_atoms;
		state->n_atoms += n_atoms;
		state->atoms = (atomDat *) realloc(state->atoms, sizeof(atomDat)*state->n_atoms);
	}

	/* setup atom data */
	atom.type = atype;
	atom.xspeed = 0.0;
	atom.yspeed = 0.0;
	atom.n_bonds = 0;
	atom.bonds = NULL;

		/* create the atoms */
	for (i=atoms_start; i<state->n_atoms; i++){

			/* make random unique coordinates */
		while (1) {
			x = (float) (((int) rand())%max_x);
			y = (float) (((int) rand())%max_y);

			/* check that coordinates are unique */
			for (j=0; j<i; j++)
				if (state->atoms[j].x == x && state->atoms[j].y == y)
					break;
			if (j==i)
				break;
		}

		atom.x = x;
		atom.y = y;
		state->atoms[i] = atom;
	}

	printf("finished randomly creating atoms\n----\n");

}



/* calculate and return the attraction force of the bond */
xy_pair getForce(atom_chars *prop1, atom_chars *prop2, float xdist, float ydist)
{
	float dist_sqrd = xdist*xdist + ydist*ydist;
		/* the distance unit vector */
	float xvect = (xdist*xdist) / dist_sqrd; 
	xvect = (xdist>0)? xvect : -xvect;
	float yvect = (ydist*ydist) / dist_sqrd;
	yvect = (ydist>0)? yvect : -yvect;

	float tforce;
	xy_pair force;

	/* get the total force */

		/* if below minimum bond, use compressibility force */
	if (dist_sqrd<= prop1->min_bond_len * prop2->min_bond_len){
		tforce = -prop1->compress_resistForce * prop2->compress_resistForce;
	}
		/* if below ideal bond len */
	else if (dist_sqrd<= prop1->ideal_bond_len * prop2->ideal_bond_len){
		tforce = -prop1->bond_strength*prop2->bond_strength;
	}
		/* if above ideal bond len */
	else{
		tforce = prop1->bond_strength+prop2->bond_strength;
	}

	force.x = tforce*xvect;
	force.y = tforce*yvect;

	return force;
}



/* update the speed of each atom */
void update_speeds(game_state *state, float dt)
{
	int a1, a2;
	float xdir, ydir;
	xy_pair force, tforce;
	atomDat *atom1, *atom2;
	atom_chars *prop1, *prop2;

	for (a1=0; a1<state->n_atoms; a1++){
		atom1 = state->atoms + a1;
		prop1 = atom_prop(state, atom1->type);

			/* get the cumulative force of all bonds */
		tforce.x = 0;
		tforce.y = 0;

		for (a2=0; a2<atom1->n_bonds; a2++){
			atom2 = state->atoms + atom1->bonds[a2];
			prop2 = atom_prop(state, atom2->type);

			xdir = atom2->x - atom1->x;
			ydir = atom2->y - atom1->y;

			force = getForce(prop1, prop2, xdir, ydir);
			tforce.x += force.x;
			tforce.y += force.y;
		}
		tforce.x *= dt;
		tforce.y *= dt;

			/* update the speed */
		atom1->xspeed += (tforce.x)/prop1->weight;
		atom1->yspeed += (tforce.y)/prop1->weight;

		atom1->xspeed -= atom1->xspeed * state->friction * dt;
		atom1->yspeed -= atom1->yspeed * state->friction * dt;
	}

}


/* update the positions of the atoms. For now, wrapping will be enabled */
void move_atoms(game_state *state, float dt, int frame_w, int frame_h) 
{
	int a;
	atomDat *atom;

	for (a=0; a<state->n_atoms; a++){
		atom = state->atoms + a;

			/* update positions */
		atom->x+=atom->xspeed;
		atom->y+=atom->yspeed;

			/* wrapping */
		if (atom->x<0)
			atom->x = (float) frame_w + atom->x;
		else if (atom->x > frame_w)
			atom->x = (float) atom->x - frame_w;
		if (atom->y<0)
			atom->y = (float) frame_h + atom->y;
		else if (atom->y > frame_h)
			atom->y = (float) atom->y - frame_h;
	}
}



void bond_atoms(game_state *state)
{
	int a1, a2, j;
	float xdist, ydist, tdist_sqrd;
	atom_chars *prop1, *prop2; /* properties */
	atomDat *atom1, *atom2;

		/* for every atom, see if it's close enough to bond with any other one */
	for (a1=0; a1<state->n_atoms; a1++){
		atom1 = state->atoms + a1;
		prop1 = atom_prop(state, atom1->type);

			/* check whether it can bond with any other atoms */
		for (a2=a1+1; a2<state->n_atoms; a2++){
			atom2 = state->atoms + a2;
			prop2 = atom_prop(state, atom2->type);

				/* check whether already bonded */
			for (j=0; j<atom1->n_bonds; j++){
				if (atom1->bonds[j] == a2)
					break;
			}
			if (j!=atom1->n_bonds)
				continue;

				/* if close enough, bond them */
			xdist = atom1->x - atom2->x;
			ydist = atom1->y - atom2->y;
			tdist_sqrd = xdist*xdist + ydist*ydist;
			if (tdist_sqrd <= prop1->max_bond_len * prop2->max_bond_len){
				atom1->bonds = (int *) realloc( atom1->bonds, sizeof(int) * ++atom1->n_bonds);
				atom1->bonds[atom1->n_bonds-1] = a2;
				atom2->bonds= (int *) realloc( atom2->bonds, sizeof(int) * ++atom2->n_bonds);
				atom2->bonds[atom2->n_bonds-1]=a1;
			}
		}


	}

}



/* if bonds are too far, break them. */
void break_bonds(game_state *state) //retest bond_atoms before testing this fun
{
	int a1, a2, i, tmp;
	float xdist, ydist, tdist, tdist_sqrd;
	atomDat *atom1, *atom2;
	atom_chars *prop1, *prop2;

		/* for every atom, break the bond with any atoms that are too far */
	for (a1=0; a1<state->n_atoms; a1++){
		atom1 = state->atoms + a1;
		prop1 = atom_prop(state, atom1->type);

			/* check every bonded atom */
		for (a2=0; a2<atom1->n_bonds; a2++){
			atom2 = state->atoms + atom1->bonds[a2];
			prop2 = atom_prop(state, atom2->type);

				/* if too far, break the bond */
			xdist = atom1->x - atom2->x;
			ydist = atom1->y - atom2->y;
			tdist_sqrd = xdist*xdist + ydist*ydist;
			if (tdist_sqrd > prop1->max_bond_len * prop2->max_bond_len){

				tmp = atom1->bonds[atom1->n_bonds-1];
				atom1->bonds[atom1->n_bonds-1] = atom1->bonds[a2];
				atom1->bonds[a2] = tmp;
				atom1->n_bonds--;

				/* find where atom1 is in atom2's bonds */
				for (i=0; i<atom2->n_bonds; i++){
					if (a1==atom2->bonds[i])
						break;
				}

				tmp = atom2->bonds[atom2->n_bonds-1];
				atom2->bonds[atom2->n_bonds-1] = atom2->bonds[i];
				atom2->bonds[i] = tmp;
				atom2->n_bonds--;
			}
		}

	}

}


/* update the speeds, positions, and bonds of the atoms */
void update_atoms(game_state *state, float dt, float frame_w, float frame_h)
{
	update_speeds(state, dt);
	move_atoms(state, dt, frame_w, frame_h);

	bond_atoms(state);
	break_bonds(state);
}




