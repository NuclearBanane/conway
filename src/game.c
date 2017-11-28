#include "game.h"

int write_to_core(uint value, uint x, uint y){
	e_write(&dev, i, j, 0x2000, &a, sizeof(unsigned));
}

int run_conway(uint **world){
	e_platform_t platform;
	e_epiphany_t dev;

	e_init(NULL);
    e_reset_system(); //reset Epiphany
    e_get_platform_info(&platform);
    e_open(&dev, 0, 0, platform.rows, platform.cols); //open all cores

	e_load_group("emain.elf", &dev, 0, 0, platform.rows, platform.cols, E_FALSE);

	for (i=0; i<platform.rows; i++){
		for (j=0; j<platform.cols; j++){
			write_to_core()
		}
	}
	e_start_group(&dev);
}
