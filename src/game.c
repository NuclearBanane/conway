#include "game.h"

#define N_READY 0x0
#define READY 0x1

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

void printGeneration(CoreState **gen, uint8_t world_dim){
	int x,y,z,width = gen[0][0]->dim;
	char * buff[world_dim];
	for(y=0; x <width; x++)
		for(z=0;z<width; z++){
			for(x=0; y<width;x++)
				world_dim[z+x] = gen[y][x][z];
			printf("%s\n", buff);
			}
}

void run_conway(Board* world, uint32_t iterations){
	e_platform_t platform;
	e_epiphany_t dev;

	int i=0;
	int j=0;
	int p=0;
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
	char *core_state;
	for (i=0; i<platform.rows; i++){
		for (j=0; j<platform.cols; j++){
			core_state = lol(i,j,world);
			e_write(&dev, i, j, 0x4000, &core_state->dim, sizeof(char));
			e_write(&dev, i, j, 0x4001, &core_state->cells, sizeof(char)*(world->dim*world->dim)/16);
			for(p= 0; p<(world->dim*world->dim)/16;p++)
				printf("%c\n", core_state->cells[p]);
			printf("next row: %u,%u\n",i,j);
		}
	}

	e_start_group(&dev);
	char test[1024];
	char DEV_STATE
	uint32_t current_iteration = 0;
	sleep(1);
	while(current_iteration < iterations){
		while(1){
			all_done=0;
			for (i=0; i<platform.rows; i++){
				for (j=0; j<platform.cols;j++){
					e_read(&dev, i, j, 0x4000, &done[i*platform.cols+j], sizeof(char));
					all_done+=done[i*platform.cols+j];
				}
			}
			if(all_done==16){
				break;
			}
		}
		CoreState **iteration_state = malloc(sizeof(CoreState *) * 4);
		for(i=0; i < 4; i++){
			iteration_state[i] = malloc(sizeof(CoreState *) *4)
		}

		for (i=0; i<platform.rows; i++){
			for (j=0; j<platform.cols;j++){
				CoreState core = malloc(sizeof(CoreState))
				core.dim = (world->dim*world->dim)/16;
				e_read(&dev, 0, 1, 0x4001, core.cells, sizeof(char) * (world->dim*world->dim)/16);
				iteration_state[i][j] = core;
			}
		}
		if(current_iteration < iterations){}
		// Set cores to process next generations
			char* go = N_READY;
			for (i=0; i<platform.rows; i++){
				for (j=0; j<platform.cols;j++){
					e_write(&dev, i, j, 0x4000, &go, sizeof(char));
				}
			}
		}
		printGeneration(iteration_state, world->dim);
		current_iteration++;
	}
	return 0;
}
