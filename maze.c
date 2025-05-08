#include "maze.h"

static int16_t maze[MAZE_CELLS] = {14,12,4,4,4,4,4,4,4,4,4,4,4,4,4,6,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,24,16,16,16,16,16,16,16,16,16,16,16,16,16,16,18};


int16_t maze_get_cells(void) {
  return MAZE_CELLS;
}

int16_t maze_get_columns(void) {
  return MAZE_COLUMNS;
}

int16_t maze_get_rows(void) {
  return MAZE_ROWS;
}

bool wall_exists(uint8_t position, uint8_t wall_bit) {
  return maze[position] & wall_bit;
}

bool maze_is_visited(uint8_t position) {
  return maze[position] & VISITED_BIT;
}

void initialize_maze(void) {
  for (uint16_t i = 0; i < MAZE_CELLS; i++) {
    maze[i] = 0;
  }

  for (uint16_t i = 0; i < MAZE_ROWS; i++) {
    maze[(MAZE_COLUMNS - 1) + i * (MAZE_COLUMNS)] |= EAST_BIT;
    maze[i * MAZE_COLUMNS] |= WEST_BIT;
  }

  for (uint16_t i = 0; i < MAZE_COLUMNS; i++) {
    maze[i] |= SOUTH_BIT;
    maze[(MAZE_ROWS - 1) * MAZE_COLUMNS + i] |= NORTH_BIT;
  }
}