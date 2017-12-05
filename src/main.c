#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <e-hal.h>
#include "game.h"

#define DEFAULT_GENERATIONS 1

struct Board;

int parse_line(char* line, uint8_t* row){
	for(int i = 0; i < strlen(line)-1; i++){
		if((line[i] == '1')||(line[i] =='0')){
			row[i] = (uint8_t)line[i] % 2;
		} else {
			printf("Illegal character: %c\n", line[i]);
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

void parse_input(char* input_file, Board* world);

int main(int argc, char **argv){
	int generations = DEFAULT_GENERATIONS;
	char *input_file;
	Board *world = malloc(sizeof(Board));
	printf("lol\n");
	//Handle arguments
	if(argc < 2){
		printf("No args, quitting");
		return 1;
	}
	input_file = argv[1];
	parse_input(input_file, world);
	// If generations parameter is set
	if(argc == 3){
		generations = atoi(argv[2]);
	}

	for(size_t i = 0; i< world->dim; i++){
		for(size_t j = 0; j< world->dim; j++)
			printf("%u", world->world[i][j]);
		printf("\n");
	}

	run_conway(world);
	return 0;
}

void parse_input(char* input_file, Board* world){
	FILE *fp;
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
    fp = fopen(input_file, "r");
    if (fp == NULL)
		exit(EXIT_FAILURE);
	read = getline(&line, &len, fp);
	if(read == -1){
		printf("File was empty\n");
		exit(EXIT_FAILURE);
	}
	printf("Dimension of world is: %zu\n", read-1);
	world->dim = (uint8_t) read - 1;
	world->world = malloc(sizeof(uint8_t) * world->dim);
	if (world->world) {
		for (uint8_t i = 0; i < world->dim; i++){
    		world->world[i] = malloc(sizeof(uint8_t) * world->dim);
  		}
	}
	parse_line(line, world->world[0]);
    for(uint8_t rows_seen = 1;
			(rows_seen < world->dim) &&
			((read = getline(&line, &len, fp)) != -1);
			rows_seen++) {
		parse_line(line, world->world[rows_seen]);
    }
	if( read = getline(&line, &len, fp) != -1){
		printf("File wasn't an n by n game file\n");
		exit(EXIT_FAILURE);
	}
	for(size_t i = 0; i< world->dim; i++){
		for(size_t j = 0; j< world->dim; j++)
			printf("%u", world->world[i][j]);
		printf("\n");
	}
}
