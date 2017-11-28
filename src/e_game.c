#include "e_game.h"

void dev_conway(){
	int32_t iof = 0; // Sticky Integer Overflow Flag
	uint32_t iterations = 0;
	unsigned n_alive;
	while (1) { //TODO Generalize for n
		iterations++;
		unsigned tmp_iof = e_reg_read(E_REG_STATUS);
		tmp_iof = tmp_iof & (4096); // use the sticky overflow integer flags
		iof = iof | tmp_iof;
		n_alive = 0;

		// Update personal aliveness
		for (int i = 0; i < 8; i++) n_alive += swap[n_reserved+i] == DEAD ? 1 : 0;
		if (n_alive == 3) swap[1] = ALIVE;
		else if (n_alive < 2) swap[1] = DEAD;
		else if (n_alive > 3) swap[1] = DEAD;

		// Broadcast aliveness to adjacents
		broadcast_status(core_row, core_col, &swap[1])
		*status = iterations;
		*state = iof;
	}
}

void broadcast_status(uint8_t row, uint8_t col, char *status){
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (i == 1 && j == 1) continue;
			// Shift rows and cols by -1 to point to proper adjacencies
			e_write(&e_group_config,
				status,
				(row+i-1)%4,
				(col+j-1)%4,
				(void *)0x4000+n_reserved+(i*3)+j,
				1);
		}
	}
}
