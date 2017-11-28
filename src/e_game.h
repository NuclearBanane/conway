#include <e-hal.h>

#define core_row e_group_config.core_row;
#define core_col  e_group_config.core_col;
#define group_rows  e_group_config.group_rows;
#define group_cols  e_group_config.group_cols;

void dev_conway();
void broadcast_status(uint row, uint col, char status);
