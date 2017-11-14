#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <e-lib.h>

#define N_READY 0x0
#define READY 0x1
#define DEAD '-'
#define ALIVE 'O'

#define U_ALIVE 0
#define D_ALIVE 1
#define L_ALIVE 2
#define R_ALIVE 3

/*
 * [1] status, - = dead O = alive
 * [0] ready, 0 = not 1 = ready
 */

volatile uint32_t *status;
volatile uint32_t *state;

int main(void) {
	unsigned core_row, core_col,
			group_rows, group_cols,
			core_num;
	char neighbor_status;

	uint32_t iterations = 0;
	uint32_t iof = 0; // Sticky Integer Overflow Flag
	int n = 4 / 4; //TODO Find a way to get this from main process (shared memory maybe?)

	core_row = e_group_config.core_row;
	core_col = e_group_config.core_col;
	group_rows = e_group_config.group_rows;
	group_cols = e_group_config.group_cols;

	core_num = core_row * group_cols + core_col;

	unsigned n_reserved = 2;
	char swap[n_reserved+(n*8)] SECTION(".text_bank2"); // adjacent aliveness
	swap[0] = N_READY;
	swap[1] = DEAD;

	// starts at the beginning of sdram
	status = (volatile uint32_t*) (0x8f000000 + 0x4*core_num);
	// we add offset of 0x40 = 16 * sizeof(uint32_t)
	state = (volatile uint32_t*) (0x8f000040 + 0x4*core_num);

	unsigned n_alive;
	while (1) { //TODO Generalize for n
		iterations++;
		unsigned tmp_iof = e_reg_read(E_REG_STATUS);
		tmp_iof = tmp_iof & (4096); // use the sticky overflow integer flag
		iof = iof | tmp_iof;
		n_alive = 0;

		// Update personal aliveness
		for (int i = 0; i < 8; i++) n_alive += swap[n_reserved+i] == DEAD ? 1 : 0;
		if (alive_neighbor == 3) swap[1] = ALIVE;
		else if (alive_neighbor < 2) swap[1] = DEAD;
		else if (alive_neighbor > 3) swap[1] = DEAD;

		//TODO broadcast aliveness to adjacents
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (i == 1 && j == 1) continue;
				e_write(&e_group_config,
						swap[1],
						// Shift rows and cols by -1 to point to proper adjacencies
						(core_row+i-1)%4,
						(core_col+j-1)%4,
						0x4000+n_reserved+(i*3)+j,
						1);
			}
		}

		*status = iterations;
		*state = iof;
	}
}

