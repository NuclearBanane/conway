#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <e-lib.h>

#define READY 0x1
#define DEAD '-'
#define ALIVE 'O'

#define U_ALIVE 0
#define D_ALIVE 1
#define L_ALIVE 2
#define R_ALIVE 3

/*
 *  * [1] status, O = dead X = alive
 *   * [0] ready, 0 = not 1 = ready
 *    */

char swap[8] SECTION(".text_bank2"); // adjacent aliveness
volatile char *result;
volatile uint32_t *status;
volatile uint32_t *state;

int main(void) {
    unsigned core_row, core_col,
             group_rows, group_cols,
             core_num;
    char neighbor_status;
    uint32_t iterations = 0;
    uint32_t iof = 0; // Sticky Integer Overflow Flag

    core_row = e_group_config.core_row;
    core_col = e_group_config.core_col;
    group_rows = e_group_config.group_rows;
    group_cols = e_group_config.group_cols;

    core_num = core_row * group_cols + core_col;


}

