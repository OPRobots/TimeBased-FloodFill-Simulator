#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>
#include <stdint.h>


#define VISITED_BIT 1
#define EAST_BIT 2
#define SOUTH_BIT 4
#define WEST_BIT 8
#define NORTH_BIT 16

#define MAZE_ROWS 16
#define MAZE_COLUMNS 16
#define MAZE_CELLS MAZE_ROWS *MAZE_COLUMNS
#define MAZE_MAX_DISTANCE MAZE_CELLS - 1

int16_t maze_get_cells(void);
int16_t maze_get_columns(void);
int16_t maze_get_rows(void);
bool wall_exists(uint8_t position, uint8_t wall_bit);
bool maze_is_visited(uint8_t position);
void initialize_maze(void);

#endif