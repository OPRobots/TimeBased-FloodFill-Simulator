#include "maze.h"


static int16_t maze[MAZE_CELLS] = {14,12,20,20,20,20,20,20,20,20,20,20,20,20,20,6,8,18,12,20,20,20,20,20,20,20,20,20,6,12,20,18,8,20,2,12,20,20,20,20,20,4,20,20,18,24,20,6,8,22,8,16,4,20,6,12,20,18,12,20,6,12,20,18,8,20,16,4,16,4,18,24,20,20,18,12,18,24,20,6,8,20,4,16,4,16,6,12,6,12,6,24,6,12,20,18,8,20,16,20,16,20,18,10,24,18,24,20,18,24,20,6,10,12,20,20,20,20,20,0,4,20,20,20,4,4,4,2,10,10,28,4,20,4,20,0,18,12,20,6,26,10,10,10,10,10,12,16,4,18,12,18,12,18,14,24,6,26,10,10,10,10,24,4,16,6,24,20,18,12,16,6,24,6,26,10,10,10,12,16,4,16,4,20,4,16,4,16,4,16,6,26,10,10,24,4,16,4,16,4,16,4,16,4,16,6,24,6,10,10,28,16,20,16,20,16,20,16,20,16,20,16,22,10,10,24,20,20,20,20,20,20,20,20,20,20,20,20,20,2,24,20,20,20,20,20,20,20,20,20,20,20,20,20,20,18};

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