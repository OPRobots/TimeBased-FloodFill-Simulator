#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "maze.h"
#include <stdint.h>
#include <stdio.h>

#define VISITED_BIT 1
#define EAST_BIT 2
#define SOUTH_BIT 4
#define WEST_BIT 8
#define NORTH_BIT 16

#define MAX_TARGETS 10

enum compass_direction {
  EAST = 1,
  SOUTH = -MAZE_COLUMNS,
  WEST = -1,
  NORTH = MAZE_COLUMNS,
};

struct compass_direction_values{
  int8_t EAST;
  int8_t SOUTH;
  int8_t WEST;
  int8_t NORTH;
};

enum step_direction {
  NONE = -1,
  FRONT = 0,
  LEFT = 1,
  RIGHT = 2,
  BACK = 3,
};

struct cells_queue {
  uint8_t queue[MAZE_CELLS];
  uint8_t head;
  uint8_t tail;
};

struct cells_stack {
  uint8_t stack[MAX_TARGETS];
  uint8_t size;
};

void update_floodfill(void);
void floodfill_maze_print(void);

#endif