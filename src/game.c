#include "game.h"

/*int write_to_core(e_epiphany_t dev, uint8_t value, uint8_t x, uint8_t y){
	e_write(&dev, i, j, 0x2000, &a, sizeof(unsigned));
}*/

struct Board;
struct CoreState;

CoreState* lol(uint8_t x, uint8_t y, Board* world){
	int core_cell_num =0;
	uint8_t h;
	uint8_t i;
	CoreState *core_state = malloc(sizeof(CoreState));
	core_state->cells = malloc(sizeof(uint8_t) * ((world->dim * world->dim)/16));
	core_state->dim = world->dim;
	for(h = 0; h < world->dim; h+=4) {
		for (i = 0; i < world->dim; i+=4) {
			core_state->cells[core_cell_num] = world->world[x+h][y+i];
			core_cell_num++;
		}
	}
	return core_state;
}

void run_conway(Board* world){
	e_platform_t platform;
	e_epiphany_t dev;

	int i=0;
	int j=0;
	int p =0;
	e_init(NULL);
    e_reset_system(); //reset Epiphany
	e_reset_group(&dev);
    e_get_platform_info(&platform);

    e_open(&dev, 0, 0, platform.rows, platform.cols); //open all cores

	e_load_group("./build/emain.elf", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

	/*
	First 8 bytes :  unit8 = n
	Second array of all states a core represents
	*/
	CoreState *core_state;
	for (i=0; i<platform.rows; i++){
		for (j=0; j<platform.cols; j++){
			core_state = lol(i,j,world);
			e_write(&dev, i, j, 0x4100, &core_state->dim, sizeof(uint8_t));
			e_write(&dev, i, j, 0x4104, &core_state->cells, sizeof(uint8_t)*(world->dim*world->dim)/16);
			for(p= 0; p<(world->dim*world->dim)/16;p++)
				printf("%u\n", core_state->cells[p]);
			printf("next row: %u,%u\n",i,j);
		}
	}
	e_start_group(&dev);
	char test[1024];
	int op = 0;
	sleep(1);
	while(op <20){
		e_read(&dev, 0, 1, 0x4009, &test[2],  sizeof(char));
		printf("dead11  %c\n", test[2]);
		printf("dead11  %i\n", test[2]);
	/*	e_read(&dev, 0, 0, 0x4105, test, sizeof(uint8_t));
		if(test[0]==0){
			printf("dead00 %u\n", *test);
		} else {
			printf("alive00 %u\n",*test);
		}
		e_read(&dev, 1, 0, 0x4105, test, sizeof(uint8_t));
		if(test[0]==0){
			printf("dead10 %u\n", test[0]);
		} else {
			printf("alive10 %u\n", test[0]);
		}*/
		op++;
	}
}
