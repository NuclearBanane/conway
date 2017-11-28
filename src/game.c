#include "game.h"

/*int write_to_core(e_epiphany_t dev, uint8_t value, uint8_t x, uint8_t y){
	e_write(&dev, i, j, 0x2000, &a, sizeof(unsigned));
}*/

int* lol(uint8_t x, uint8_t y, uint8_t n){
	int core_cell_num =0;
	uint8_t *core_cells = malloc(sizeof(uint8_t) * ((n*n)/16));
	for(unit8_t h = 0; h < n; h+=4) {
		for (unit8_t i = 0; i < n; h+=4) {
			p[core_cell_num++] = world[x+h][y+i];
		}
	}
}

void run_conway(uint8_t **world, uint8_t n){
	e_platform_t platform;
	e_epiphany_t dev;

	e_init(NULL);
    e_reset_system(); //reset Epiphany
    e_get_platform_info(&platform);
    e_open(&dev, 0, 0, platform.rows, platform.cols); //open all cores

	e_load_group("emain.elf", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);


	/*
	First 8 bytes :  unit8 = n
	Second array of all states a core represents
	*/
	int *core_cells;
	for (int i=0; i<platform.rows; i++){
		for (int j=0; j<platform.cols; j++){
			core_cells = lol(i,j,n);
			for(int p = 0; p<(n*n)/16;p++)
				printf("%u\n", core_cells);
			printf("next row: %u,%u\n",i,j);
		}
	}
	e_start_group(&dev);
}
