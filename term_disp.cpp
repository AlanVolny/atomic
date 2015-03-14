


#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <math.h>


#include "term_disp.h"
#include "atom.h"

extern int frame_width;
extern int frame_height;

/* print atom names, locations, and bond partners */
void print_state(game_state *state)
{
	int i, j;
	atomDat atom;

	printf("List of atoms:\n");

	for (i=0; i<state->n_atoms; i++){
		atom = state->atoms[i];
		printf("%c:\n",'A'+i);
		printf("  x:%4.1f %+.5f\n", atom.x, atom.xspeed); /* these include speed */
		printf("  y:%4.1f %+.5f\n", atom.y, atom.yspeed);
		// printf("  x:%.1f\n", atom.x);
		// printf("  y:%.1f\n", atom.y);
		for (j=0; j<atom.n_bonds; j++)
			printf("    ->%c\n", 'A' + atom.bonds[j] );
	}

	printf("-----\n");
}


/* print the atoms to the terminal */
void print_atoms_to_term(game_state *state, int max_x, int max_y)
{
	int i, j, a2, x, y, arr_pos;
	float xdir, ydir, tdir, y_inc, x_inc, x_pos, y_pos, m;
	atom_chars achars;
	atomDat atom, atom2;
	char **buf; /* an 2d array of chars, showing the atoms' positions */

	char *b_char = "\x1B[33m-\x1B[0m "; /* character signifying a bond */

	buf = (char **) calloc(sizeof(char *), max_x*max_y);


		/* for every atom, put it into buf */
	for (i=0; i<state->n_atoms; i++){
		atom = state->atoms[i];
		achars = state->aType_table[atom.type];
		x = (int) atom.x;
		y = (int) atom.y;
		arr_pos = y*max_x + x;

		if (arr_pos>=max_x*max_y)
			continue;

		if (buf[arr_pos]==NULL || strcmp(buf[arr_pos], b_char)==0 ){
			buf[arr_pos] = (char *) alloca(sizeof(char *) * 20);
			if ('A'+i<127)
				sprintf( buf[arr_pos], "%s%c\x1B[0m ", achars.color,'A'+i);
			else
				sprintf( buf[arr_pos], "%s%s\x1B[0m ", achars.color,"�");
		}
			/* if the position was already super-imposed */
		else if (buf[arr_pos]!=0){
			buf[arr_pos] = (char *) alloca(sizeof(char *) * 20);
			sprintf( buf[arr_pos], "%s*\x1B[0m ", achars.color);
		}

		/* display bonds as well */
		for (a2=0; a2<atom.n_bonds; a2++){
			if (atom.bonds[a2]<i) /* no need to display already displayed bonds */
				continue;
			atom2 = state->atoms[atom.bonds[a2]];
			xdir = atom2.x - atom.x;
			ydir = atom2.y - atom.y;

			if (abs(ydir)<abs(xdir)){

				m = ydir/((xdir>0)?xdir:-xdir);
				x_inc = (xdir>0.f)?1 : -1;

				x_pos = atom.x;
				y_pos = atom.y;
				while ((x_pos>=atom.x && x_pos<atom2.x) || (x_pos<=atom.x && x_pos>atom2.x)){
					x = (int) x_pos;
					y = (int) y_pos;
					if (x>=max_x || x<0 || y>=max_y || y<0) break;
					arr_pos = y*max_x + x;
					if (buf[arr_pos]==0)
						buf[arr_pos]=b_char;

					y_pos+=m;
					x_pos+=x_inc;
				}
			}

			else {
				m = xdir/((ydir>0)?ydir : -ydir);
				y_inc = (ydir>0.f)?1 : -1;

				x_pos = atom.x;
				y_pos = atom.y;
				while ((y_pos>=atom.y && y_pos<atom2.y) || (y_pos<=atom.y && y_pos>atom2.y)){
					x = (int) x_pos;
					y = (int) y_pos;
					if (x>=max_x || x<0 || y>=max_y || y<0) break;
					arr_pos = y*max_x + x;
					if (buf[arr_pos]==0)
						buf[arr_pos]=b_char;

					y_pos+=y_inc;
					x_pos+=m;
				}
			}
		}
	}

	/* display the atoms */

		/* clear the screen */
	// printf("\x1B[%dA", max_x+3);
	printf("\x1B[H");

		/* print buf, along with framing square */

	char *output = (char *) calloc(sizeof(char), frame_width*frame_height*20);
	// strcat(output, " === Atoms: ===        \n");

	strcat(output, "+");
	for (i=0; i<max_x; i++) strcat(output, "- ");
	strcat(output, "+\n|");
	for (i=0; i<max_x*max_y; i++){
		if (i%max_x==0 && i>0) strcat(output, "|\n|");
		if (buf[i]!=NULL) strcat(output, buf[i]);
		else strcat(output, "  ");
	}
	strcat(output, "|\n+");
	for (i=0; i<max_x; i++) strcat(output, "- ");
	strcat(output, "+\n");

	/* print the final output */
	printf("%s",output);
	free(output);

	/* we're done with buf */
	free(buf);

	/* print the atom names, their locations, and bond partners */

	// print_state(state);
}




