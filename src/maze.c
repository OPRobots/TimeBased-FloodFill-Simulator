#include "maze.h"

static int16_t maze[MAZE_CELLS] = {14,12,20,20,20,20,4,20,20,20,20,6,12,20,20,6,10,24,6,12,20,20,16,22,12,20,6,24,18,12,20,18,10,12,18,24,20,20,6,12,18,14,24,4,22,24,20,6,10,10,12,6,12,6,24,18,12,2,12,18,12,6,12,18,10,10,10,24,18,24,20,20,18,24,18,28,2,10,24,6,8,18,24,20,20,20,20,20,20,20,4,22,10,24,6,10,10,14,14,14,14,14,12,20,20,20,18,14,24,6,24,2,10,8,0,0,0,0,2,12,4,6,12,16,6,24,6,26,10,10,26,26,26,26,26,24,18,10,24,6,24,6,24,6,10,24,20,20,20,20,20,6,12,0,6,24,6,24,6,10,10,12,6,12,6,12,6,24,18,10,24,6,24,6,24,2,10,10,10,10,10,10,10,12,6,24,20,16,6,24,6,26,10,10,10,10,10,10,10,10,10,12,4,20,2,12,16,22,10,10,24,18,24,18,24,18,24,18,26,14,24,18,12,6,10,24,20,20,20,20,20,20,20,20,20,16,20,20,18,10,24,20,20,20,20,20,20,20,20,20,20,20,20,20,20,18};


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