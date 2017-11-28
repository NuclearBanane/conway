#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <e-lib.h>
#include "e_game.h"

#define N_READY 0x0
#define READY 0x1
#define DEAD ' '
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
char* swap SECTION(".text_bank2"); // adjacent aliveness

int main(void) {
	char neighbor_status;

	uint32_t iterations = 0;
	uint32_t iof = 0; // Sticky Integer Overflow Flag
	int n = 4 / 4; //TODO Find a way to get this from main process (shared memory maybe?)

	core_num = core_row * group_cols + core_col;

	swap[0] = N_READY;
	swap[1] = DEAD;

	// starts at the beginning of sdram
	status = (volatile uint32_t*) (0x8f000000 + 0x4*core_num);
	// we add offset of 0x40 = 16 * sizeof(uint32_t)
	state = (volatile uint32_t*) (0x8f000040 + 0x4*core_num);
	dev_conway();
}
