#ifndef FLOODFILL_H
#define FLOODFILL_H

#include "maze.h"
#include "floodfill_weigths_generator.h"
#include <stdint.h>
#include <stdio.h>

#define VISITED_BIT 1
#define EAST_BIT 2
#define SOUTH_BIT 4
#define WEST_BIT 8
#define NORTH_BIT 16

#define MAX_TARGETS 10

enum compass_direction {
  TARGET = 0,
  EAST = 1,
  SOUTH_EAST = 1 - MAZE_COLUMNS,
  SOUTH = -MAZE_COLUMNS,
  SOUTH_WEST = -1 - MAZE_COLUMNS,
  WEST = -1,
  NORTH_WEST = -1 + MAZE_COLUMNS,
  NORTH = MAZE_COLUMNS,
  NORTH_EAST = 1 + MAZE_COLUMNS,
};

struct compass_direction_values {
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

struct queue_cell {
  uint8_t cell;
  enum compass_direction direction;
  enum compass_direction last_step;
  uint8_t count;
};

struct cells_queue {
  struct queue_cell queue[MAZE_CELLS];
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