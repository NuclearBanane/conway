#include "game.h"

/*int write_to_core(e_epiphany_t dev, uint8_t value, uint8_t x, uint8_t y){
	e_write(&dev, i, j, 0x2000, &a, sizeof(unsigned));
}*/

struct Board;

uint8_t* lol(uint8_t x, uint8_t y, Board* world){
	int core_cell_num =0;
	uint8_t *core_cells = malloc(sizeof(uint8_t) * ((world->dim * world->dim)/16));
	for(uint8_t h = 0; h < world->dim; h+=4) {
		for (uint8_t i = 0; i < world->dim; i+=4) {
			core_cells[core_cell_num] = world->world[x+h][y+i];
			core_cell_num++;
		}
	}
	return core_cells;
}

void run_conway(Board* world){
	e_platform_t platform;
	e_epiphany_t dev;

	e_init(NULL);
    e_reset_system(); //reset Epiphany
    e_get_platform_info(&platform);
    e_open(&dev, 0, 0, platform.rows, platform.cols); //open all cores

	e_load_group("./build/emain.elf", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

	/*
	First 8 bytes :  unit8 = n
	Second array of all states a core represents
	*/
	uint8_t *core_cells;
	for (int i=0; i<platform.rows; i++){
		for (int j=0; j<platform.cols; j++){
			core_cells = lol(i,j,world);
			for(int p = 0; p<(world->dim*world->dim)/16;p++)
				printf("%u\n", core_cells[p]);
			printf("next row: %u,%u\n",i,j);
		}
	}
	e_start_group(&dev);
	char test;
	int op = 0;
	while(op < 5){
		e_read(&dev, 0, 1, 0x4001, &test, sizeof(char));
		if(test==' '){
			printf("dead11\n");
		} else {
			printf("alive11\n");
		}
		e_read(&dev, 0, 0, 0x4001, &test, sizeof(char));
		if(test==' '){
			printf("dead00\n");
		} else {
			printf("alive00\n");
		}
		e_read(&dev, 1, 0, 0x4001, &test, sizeof(char));
		if(test==' '){
			printf("dead10\n");
		} else {
			printf("alive10\n");
		}
		op++;
	}
}
