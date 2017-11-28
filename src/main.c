#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <e-hal.h>
#include "game.h"

#define DEFAULT_GENERATIONS 1

int parse_line(char* line, uint* row){
	for(int i = 0; i < strlen(line)-1; i++){
		if((line[i] == '1')||(line[i] =='0')){
			row[i] = (uint)line[i] % 2;
		} else {
			printf("Illegal character: %c\n", line[i]);
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}

int main(int argc, char **argv){
	int generations = DEFAULT_GENERATIONS;
	char *input_file;
	uint **world;
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

	run_conway(world);
	return 0;
}

int parse_input(char* input_file, uint **world){
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
	size_t world_dim = read - 1;
	world = malloc(sizeof(uint) * world_dim);
	if (world) {
		for (uint i = 0; i < world_dim; i++){
    		world[i] = malloc(sizeof(uint) * world_dim);
  		}
	}
	parse_line(line, world[0]);
    for(uint rows_seen = 1;
			(rows_seen < world_dim) &&
			((read = getline(&line, &len, fp)) != -1);
			rows_seen++) {
		parse_line(line, world[rows_seen]);
    }

	if( read = getline(&line, &len, fp) != -1){
		printf("File wasn't an n by n game file\n");
		exit(EXIT_FAILURE);
	}
    free(line);
	for(int i = 0; i< world_dim; i++){
		for(int j = 0; j< world_dim; j++)
			printf("%u", world[i][j]);
		printf("\n");
	}
	return 1;
}
