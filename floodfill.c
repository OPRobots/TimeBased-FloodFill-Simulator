#include "floodfill.h"

static uint8_t floodfill[MAZE_CELLS];

static struct cells_queue cells_queue;

static struct cells_stack target_cells;

static struct compass_direction_values directions_values = {
    .EAST = 1,
    .SOUTH = -MAZE_COLUMNS,
    .WEST = -1,
    .NORTH = MAZE_COLUMNS,
};

static int8_t get_direction_value(enum compass_direction direction) {
  switch (direction) {
    case EAST:
      return directions_values.EAST;
    case SOUTH:
      return directions_values.SOUTH;
    case WEST:
      return directions_values.WEST;
    case NORTH:
      return directions_values.NORTH;
    default:
      return 0;
  }
}

static void reset_floodfill_and_queue(void) {
  for (uint16_t i = 0; i < MAZE_CELLS; i++) {
    floodfill[i] = MAZE_MAX_DISTANCE;
  }
  cells_queue.head = 0;
  cells_queue.tail = 0;
}

static void queue_push(uint8_t position) {
  cells_queue.queue[cells_queue.head++] = position;
}

static uint8_t queue_pop(void) {
  return cells_queue.queue[cells_queue.tail++];
}

static void add_target(uint8_t x, uint8_t y) {
  target_cells.stack[target_cells.size++] = (x - 1) + (y - 1) * (uint16_t)maze_get_columns();
}

void update_floodfill(void) {
  reset_floodfill_and_queue();

  target_cells.size = 0;
  // target_cells.stack[target_cells.size++] = 119;
  add_target(8, 8);
  add_target(8, 9);
  add_target(9, 8);
  add_target(9, 9);

  for (uint8_t i = 0; i < target_cells.size; i++) {
    floodfill[target_cells.stack[i]] = 0;
    queue_push(target_cells.stack[i]);
  }

  while (cells_queue.head != cells_queue.tail) {
    uint8_t current_cell = queue_pop();
    uint8_t next_distance = floodfill[current_cell] + 1;
    if (!wall_exists(current_cell, EAST_BIT) && floodfill[current_cell + get_direction_value(EAST)] > next_distance) {
      floodfill[current_cell + get_direction_value(EAST)] = next_distance;
      queue_push(current_cell + get_direction_value(EAST));
    }
    if (!wall_exists(current_cell, SOUTH_BIT) && floodfill[current_cell + get_direction_value(SOUTH)] > next_distance) {
      floodfill[current_cell + get_direction_value(SOUTH)] = next_distance;
      queue_push(current_cell + get_direction_value(SOUTH));
    }
    if (!wall_exists(current_cell, WEST_BIT) && floodfill[current_cell + get_direction_value(WEST)] > next_distance) {
      floodfill[current_cell + get_direction_value(WEST)] = next_distance;
      queue_push(current_cell + get_direction_value(WEST));
    }
    if (!wall_exists(current_cell, NORTH_BIT) && floodfill[current_cell + get_direction_value(NORTH)] > next_distance) {
      floodfill[current_cell + get_direction_value(NORTH)] = next_distance;
      queue_push(current_cell + get_direction_value(NORTH));
    }
  }
}

void floodfill_maze_print(void) {
  // initialize_directions_values();
  // build_run_sequence();
  // smooth_run_sequence(menu_run_get_speed());
  for (int16_t r = maze_get_cells() - maze_get_columns(); r >= 0; r = r - maze_get_columns()) {
    // Borde superior del laberinto
    if (r == maze_get_cells() - maze_get_columns()) {
      printf("o");
      for (uint16_t i = ((uint16_t)maze_get_cells()) - maze_get_columns(); i < maze_get_cells(); i++) {
        if (wall_exists((uint8_t)i, NORTH_BIT)) {
          printf("---o");
        } else {
          printf("   o");
        }
      }
    }
    printf("\n");

    // Paredes laterales del laberinto y VISITADO
    for (int16_t c = r; c < r + maze_get_columns(); c++) {
      if (wall_exists((uint8_t)c, WEST_BIT) /* || c % maze_get_columns() == 0 */) {
        printf("|");
      } else {
        printf(" ");
      }
      if (maze_is_visited((uint8_t)c)) {
        // printf(" V ");
        printf("%3d", floodfill[c]);
      } else {
        // printf("   ");
        printf("%3d", floodfill[c]);
      }
      //   if ((c + 1 % maze_get_columns()) == 0) {
      //     printf("|");
      //   }
      if (wall_exists((uint8_t)c, EAST_BIT) && ((c + 1) % maze_get_columns()) == 0) {
        printf("|");
      }
    }
    // printf("|\n");
    printf("\n");

    // Paredes inferiores del laberinto
    printf("o");
    for (int16_t c = r; c < r + maze_get_columns(); c++) {
      if (wall_exists((uint8_t)c, SOUTH_BIT) /* || c / maze_get_columns() == 0 */) {
        printf("---o");
      } else {
        printf("   o");
      }
    }
  }
  printf("\n");

  // for (int16_t r = maze_get_cells() - maze_get_columns(); r >= 0; r = r - maze_get_columns()) {
  //   // Borde superior del laberinto
  //   if (r == maze_get_cells() - maze_get_columns()) {
  //     printf("o");
  //     for (uint16_t i = maze_get_cells() - maze_get_columns(); i < maze_get_cells(); i++) {
  //       if (wall_exists(i, NORTH_BIT)) {
  //         printf("---o");
  //       } else {
  //         printf("   o");
  //       }
  //     }
  //   }
  //   printf("\n");

  //   // Paredes laterales del laberinto y VISITADO
  //   for (int16_t c = r; c < r + maze_get_columns(); c++) {
  //     if (wall_exists(c, WEST_BIT) /* || c % maze_get_columns() == 0 */) {
  //       printf("|");
  //     } else {
  //       printf(" ");
  //     }
  //     if (maze_is_visited(c)) {
  //       printf(" V ");
  //       // printf("%3d", floodfill[c]);
  //     } else {
  //       printf("   ");
  //       // printf("%3d", floodfill[c]);
  //     }
  //     //   if ((c + 1 % maze_get_columns()) == 0) {
  //     //     printf("|");
  //     //   }
  //     if (wall_exists(c, EAST_BIT) && ((c + 1) % maze_get_columns()) == 0) {
  //       printf("|");
  //     }
  //   }
  //   // printf("|\n");
  //   printf("\n");

  //   // Paredes inferiores del laberinto
  //   printf("o");
  //   for (int16_t c = r; c < r + maze_get_columns(); c++) {
  //     if (wall_exists(c, SOUTH_BIT) /* || c / maze_get_columns() == 0 */) {
  //       printf("---o");
  //     } else {
  //       printf("   o");
  //     }
  //   }
  // }
  // printf("\n");
}