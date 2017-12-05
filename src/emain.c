#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <e-lib.h>

#define N_READY 0x0
#define READY 0x1
#define DEAD ' '
#define ALIVE 'O'

volatile uint32_t *shared_status;
volatile uint32_t *shared_state;
char* swap SECTION(".text_bank2"); // 0x4000
/* Swap Structure:
 *  -> 0x4000+n_res-1:
 *    General-purpose swap chars
 *  -> 0x4000+n_res+n-1:
 *    States for all game pnts managed by this core
 *  -> 0x4000+n_res+n+8n:
 *    Adjacent states for all managed cores
 *    Written in by adjacency broadcasts
 */

char next_gen(char* state, char* adj_states) {
	unsigned n_alive = 0;
	for (int i = 0; i < 8; i++) n_alive += adj_states[i] == DEAD ? 1 : 0;
	if	 (n_alive == 3) return ALIVE;
	else if (n_alive < 2) return DEAD;
	else if (n_alive > 3) return DEAD;
	else                  return *state;
}

void broadcast(char* state, unsigned row, unsigned col,
		char* rmt_adj_states) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;
			e_write(&e_group_config,
					state,
					// (-1,-1) offset to point to real adjacencies
					(row+i-1)%4,
					(col+j-1)%4,
					rmt_adj_states + (i*3) + j,
					1);
		}
	}
}

int main(void) {
	uint32_t iterations = 0;
	uint32_t iof = 0; // Sticky Integer Overflow Flag
	unsigned n = ((uint8_t) swap[0]) / 4; // # core-managed pnts

	unsigned e_row = e_group_config.core_row;
	unsigned e_col = e_group_config.core_col;
	unsigned core_num = e_row * e_group_config.group_cols + e_col;

	// starts at the beginning of sdram
	shared_status = (volatile uint32_t*) (0x8f000000 + 0x4*core_num);
	// we add offset of 0x40 = 16 * sizeof(uint32_t)
	shared_state = (volatile uint32_t*) (0x8f000040 + 0x4*core_num);

	volatile e_barrier_t barriers[16];
	volatile e_barrier_t *tgt_barriers[16];
	e_barrier_init(barriers, tgt_barriers);

	unsigned n_res = 1;
	swap[0] = N_READY;

	for (unsigned i = 0; i < n; i++) {
		char *rmt_adj_states = (char *)0x4000+n_res+n+8*i;
		broadcast(&swap[n_res+i], e_row, e_col, rmt_adj_states);
	}

	e_barrier(barriers, tgt_barriers);

	while (1) {
		iterations++;
		unsigned tmp_iof = e_reg_read(E_REG_STATUS);
		tmp_iof = tmp_iof & (4096); // use the sticky overflow integer flag
		iof = iof | tmp_iof;

		for (unsigned i = 0; i < n; i++) {
			char *state = &swap[n_res+i];
			char *adj_states = &swap[n_res+n+8*i];
			*state = next_gen(state, adj_states);
		}

		swap[0] = READY;
		e_barrier(barriers, tgt_barriers);

		for (unsigned i = 0; i < n; i++) {
			char *rmt_adj_states = (char *)0x4000+n_res+n+8*i;
			broadcast(&swap[n_res+i], e_row, e_col, rmt_adj_states);
		}

		e_barrier(barriers, tgt_barriers);

		*shared_status = iterations;
		*shared_state = iof;
	}
}
