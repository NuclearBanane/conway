#define _GNU_SOURCE
#include <e-hal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


struct Board_{
	uint8_t dim;
	char **world;
};

typedef struct Board_ Board;

struct CoreState_{
	uint8_t dim;
	char *cells;
};

typedef struct CoreState_ CoreState;

void run_conway(Board* world);
CoreState* lol(uint8_t x, uint8_t y, Board* world);
//int write_to_core(uint8_t value, uint8_t x, uint8_t y);
