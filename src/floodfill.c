#include "floodfill.h"

static float floodfill[MAZE_CELLS];

static struct cells_queue cells_queue;

static struct cells_stack target_cells;

static struct cell_weigth straight_weights[15];
static uint16_t straight_weights_count;
static struct cell_weigth diagonal_weights[15];
static uint16_t diagonal_weights_count;

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

static void queue_push(uint8_t position, enum compass_direction direction, enum compass_direction step, uint8_t count) {
  struct queue_cell queue = {
      .cell = position,
      .direction = direction,
      .last_step = step,
      .count = count,
  };

  float value = floodfill[position];
  int i = cells_queue.head;

  while (i > 0 && floodfill[cells_queue.queue[i - 1].cell] > value) {
    cells_queue.queue[i] = cells_queue.queue[i - 1];
    i--;
  }

  cells_queue.queue[i] = queue;
  cells_queue.head++;
}

static struct queue_cell queue_pop(void) {
  return cells_queue.queue[cells_queue.tail++];
}

static void add_target(uint8_t x, uint8_t y) {
  target_cells.stack[target_cells.size++] = (x - 1) + (y - 1) * (uint16_t)maze_get_columns();
}

static float get_next_floodfill_distance(float distance, enum compass_direction from_direction, enum compass_direction to_direction, uint8_t count, uint8_t last_count) {
  // Floodfill estandar
  // return distance + 1.0f;

  // Floodfill con prioridad diagonales
  // switch (to_direction) {
  //   case EAST:
  //   case SOUTH:
  //   case WEST:
  //   case NORTH:
  //     return distance + 1.0f;
  //   default:
  //     return distance + 0.7f;
  // }

  // Floodfill con penalizacion por velocidad y frenada
  bool from_orthogonal = false;
  bool from_diagonal = false;
  bool to_orthogonal = false;
  bool to_diagonal = false;
  float next_distance = 0.0f;
  switch (from_direction) {
    case TARGET:
    case EAST:
    case SOUTH:
    case WEST:
    case NORTH:
      from_orthogonal = true;
      break;
    case SOUTH_EAST:
    case SOUTH_WEST:
    case NORTH_WEST:
    case NORTH_EAST:
      from_diagonal = true;
      break;
  }
  switch (to_direction) {
    case TARGET:
    case EAST:
    case SOUTH:
    case WEST:
    case NORTH:
      to_orthogonal = true;
      break;
    case SOUTH_EAST:
    case SOUTH_WEST:
    case NORTH_WEST:
    case NORTH_EAST:
      to_diagonal = true;
      break;
  }
  if (from_orthogonal && to_orthogonal) {
    if (count < straight_weights_count) {
      next_distance += (float)(straight_weights[count].time);
    } else {
      next_distance += (float)straight_weights[straight_weights_count - 1].time;
    }
  } else if (from_diagonal && to_diagonal) {
    if (from_direction == to_direction) {
      if (count < diagonal_weights_count) {
        next_distance += (float)diagonal_weights[count].time;
      } else {
        next_distance += (float)diagonal_weights[diagonal_weights_count - 1].time;
      }
    } else {
      next_distance += (float)diagonal_weights[0].time;
      if (last_count < diagonal_weights_count) {
        next_distance += (float)diagonal_weights[last_count].penalty;
      } else {
        next_distance += (float)diagonal_weights[diagonal_weights_count - 1].penalty;
      }
    }
  } else if (from_orthogonal && to_diagonal) {
    next_distance += (float)diagonal_weights[0].time;
    if (last_count < straight_weights_count) {
      next_distance += (float)straight_weights[last_count].penalty;
    } else {
      next_distance += (float)straight_weights[straight_weights_count - 1].penalty;
    }
  } else if (from_diagonal && to_orthogonal) {
    next_distance += (float)straight_weights[0].time;
    if (last_count < diagonal_weights_count) {
      next_distance += (float)diagonal_weights[last_count].penalty;
    } else {
      next_distance += (float)diagonal_weights[diagonal_weights_count - 1].penalty;
    }
  }
  return distance + next_distance;
}

static uint8_t get_next_floodfill_count(enum compass_direction from_direction, enum compass_direction to_direction, uint8_t count) {
  if (from_direction == to_direction) {
    return count + 1;
  } else {
    return 0;
  }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
static enum compass_direction get_next_floodfill_direction(enum compass_direction from_direction, enum compass_direction from_step, enum compass_direction to_step) {
  switch (from_direction) {
    case TARGET:
      return to_step;
    case EAST:
      switch (to_step) {
        case NORTH:
          return NORTH_EAST;
        case SOUTH:
          return SOUTH_EAST;
        default:
          return to_step;
      }
      break;
    case SOUTH_EAST:
      switch (to_step) {
        case NORTH:
          return NORTH_EAST;
        case SOUTH:
          switch (from_step) {
            case SOUTH:
              return SOUTH;
            case EAST:
              return SOUTH_EAST;
          }
          break;
        case EAST:
          switch (from_step) {
            case SOUTH:
              return SOUTH_EAST;
            case EAST:
              return EAST;
          }
          break;
        case WEST:
          return SOUTH_WEST;
        default:
          return to_step;
      }
      break;
    case SOUTH:
      switch (to_step) {
        case EAST:
          return SOUTH_EAST;
        case WEST:
          return SOUTH_WEST;
        default:
          return to_step;
      }
      break;
    case SOUTH_WEST:
      switch (to_step) {
        case NORTH:
          return NORTH_WEST;
        case SOUTH:
          switch (from_step) {
            case SOUTH:
              return SOUTH;
            case WEST:
              return SOUTH_WEST;
          }
          break;
        case EAST:
          return SOUTH_EAST;
        case WEST:
          switch (from_step) {
            case SOUTH:
              return SOUTH_WEST;
            case WEST:
              return WEST;
          }
          break;
        default:
          return to_step;
      }
      break;
    case WEST:
      switch (to_step) {
        case NORTH:
          return NORTH_WEST;
        case SOUTH:
          return SOUTH_WEST;
        default:
          return to_step;
      }
      break;
    case NORTH_WEST:
      switch (to_step) {
        case NORTH:
          switch (from_step) {
            case NORTH:
              return NORTH;
            case WEST:
              return NORTH_WEST;
          }
          break;
        case EAST:
          return NORTH_EAST;
        case SOUTH:
          return SOUTH_WEST;
        case WEST:
          switch (from_step) {
            case NORTH:
              return NORTH_WEST;
            case WEST:
              return WEST;
          }
          break;
        default:
          return to_step;
      }
      break;
    case NORTH:
      switch (to_step) {
        case EAST:
          return NORTH_EAST;
        case WEST:
          return NORTH_WEST;
        default:
          return to_step;
      }
      break;
    case NORTH_EAST:
      switch (to_step) {
        case NORTH:
          switch (from_step) {
            case NORTH:
              return NORTH;
            case EAST:
              return NORTH_EAST;
          }
          break;
        case SOUTH:
          return SOUTH_EAST;
        case WEST:
          return NORTH_WEST;
          break;
        case EAST:
          switch (from_step) {
            case NORTH:
              return NORTH_EAST;
            case EAST:
              return EAST;
          }
          break;
        default:
          return to_step;
      }
      break;
  }
  return to_step;
}
#pragma GCC diagnostic pop

const char *direccion_a_string(enum compass_direction dir) {
  switch (dir) {
    case TARGET:
      return "Objetivo";
    case EAST:
      return "Este";
    case SOUTH_EAST:
      return "Sureste";
    case SOUTH:
      return "Sur";
    case SOUTH_WEST:
      return "Suroeste";
    case WEST:
      return "Oeste";
    case NORTH_WEST:
      return "Noroeste";
    case NORTH:
      return "Norte";
    case NORTH_EAST:
      return "Noreste";
    default:
      return "Desconocida";
  }
}

void update_floodfill(bool step_by_step) {
  reset_floodfill_and_queue();

  straight_weights_count = get_cells_to_max_speed(180.0f, 2600, 6000, 25000);
  weight_floodfill_table(180.0f, 2600, 6000, 25000, straight_weights_count, straight_weights);
  diagonal_weights_count = get_cells_to_max_speed(127.3f, 2600, 6000, 25000);
  weight_floodfill_table(127.3f, 2600, 6000, 25000, diagonal_weights_count, diagonal_weights);

  target_cells.size = 0;
  // add_target(8, 8);
  // add_target(8, 9);
  // add_target(9, 8);
  // add_target(9, 9);
  // add_target(5, 5);
  add_target(1, 1);

  for (uint8_t i = 0; i < target_cells.size; i++) {
    floodfill[target_cells.stack[i]] = 0;
    queue_push(target_cells.stack[i], TARGET, TARGET, 0);
  }

  uint16_t cell_count = 0;
  while (cells_queue.head != cells_queue.tail) {
    cell_count++;
    // if (cell_count > 5) {
    //   return;
    // }
    // scanf("%*[^\n]%*c");

    struct queue_cell queue_cell = queue_pop();
    uint8_t current_cell = queue_cell.cell;
    enum compass_direction direction = queue_cell.direction;
    enum compass_direction last_step = queue_cell.last_step;
    uint8_t count = queue_cell.count;

    if (step_by_step) {
      printf("Current cell: [%d,%d], Direction: %s, Last step: %s, Count: %d\n", (current_cell % 16) + 1, (current_cell / 16) + 1, direccion_a_string(direction), direccion_a_string(last_step), count);
    }

    float next_distance = 0;
    uint8_t next_cell = 0;
    uint8_t next_count = 0;
    enum compass_direction next_direction = 0;
    if (!wall_exists(current_cell, EAST_BIT)) {
      if (step_by_step) {
        getchar();
      }
      next_cell = current_cell + get_direction_value(EAST);
      next_direction = get_next_floodfill_direction(direction, last_step, EAST);
      next_count = get_next_floodfill_count(direction, next_direction, count);
      next_distance = get_next_floodfill_distance(floodfill[current_cell], direction, next_direction, next_count, count);
      if (floodfill[next_cell] >= next_distance) {
        if (step_by_step) {
          printf("Next direction: %s, Next distance: %f\n", direccion_a_string(next_direction), next_distance);
        }
        floodfill[next_cell] = next_distance;
        queue_push(next_cell, next_direction, EAST, next_count);
      }
    }
    if (!wall_exists(current_cell, SOUTH_BIT)) {
      if (step_by_step) {
        getchar();
      }
      next_cell = current_cell + get_direction_value(SOUTH);
      next_direction = get_next_floodfill_direction(direction, last_step, SOUTH);
      next_count = get_next_floodfill_count(direction, next_direction, count);
      next_distance = get_next_floodfill_distance(floodfill[current_cell], direction, next_direction, next_count, count);
      if (floodfill[next_cell] >= next_distance) {
        if (step_by_step) {
          printf("Next direction: %s, Next distance: %f\n", direccion_a_string(next_direction), next_distance);
        }
        floodfill[next_cell] = next_distance;
        queue_push(next_cell, next_direction, SOUTH, next_count);
      }
    }
    if (!wall_exists(current_cell, WEST_BIT)) {
      if (step_by_step) {
        getchar();
      }
      next_cell = current_cell + get_direction_value(WEST);
      next_direction = get_next_floodfill_direction(direction, last_step, WEST);
      next_count = get_next_floodfill_count(direction, next_direction, count);
      next_distance = get_next_floodfill_distance(floodfill[current_cell], direction, next_direction, next_count, count);
      if (floodfill[next_cell] >= next_distance) {
        if (step_by_step) {
          printf("Next direction: %s, Next distance: %f\n", direccion_a_string(next_direction), next_distance);
        }
        floodfill[next_cell] = next_distance;
        queue_push(next_cell, next_direction, WEST, next_count);
      }
    }
    if (!wall_exists(current_cell, NORTH_BIT)) {
      if (step_by_step) {
        getchar();
      }
      next_cell = current_cell + get_direction_value(NORTH);
      next_direction = get_next_floodfill_direction(direction, last_step, NORTH);
      next_count = get_next_floodfill_count(direction, next_direction, count);
      next_distance = get_next_floodfill_distance(floodfill[current_cell], direction, next_direction, next_count, count);
      if (floodfill[next_cell] >= next_distance) {
        if (step_by_step) {
          printf("Next direction: %s, Next distance: %f\n", direccion_a_string(next_direction), next_distance);
        }
        floodfill[next_cell] = next_distance;
        queue_push(next_cell, next_direction, NORTH, next_count);
      }
    }
    if (step_by_step) {
      floodfill_maze_print();
    }
  }
  printf("Total cells: %d\n", cell_count);
}

void floodfill_maze_print(void) {
  for (int16_t r = maze_get_cells() - maze_get_columns(); r >= 0; r = r - maze_get_columns()) {
    // Borde superior del laberinto
    if (r == maze_get_cells() - maze_get_columns()) {
      printf("o");
      for (uint16_t i = ((uint16_t)maze_get_cells()) - maze_get_columns(); i < maze_get_cells(); i++) {
        if (wall_exists((uint8_t)i, NORTH_BIT)) {
          printf("-------o");
        } else {
          printf("       o");
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
        if (floodfill[c] != 255.0f) {
          printf("%7.4f", floodfill[c] /*  - 0.5f */);
        } else {
          printf("       ");
        }
      } else {
        // printf("     ");

        if (floodfill[c] != 255.0f) {
          printf("%7.4f", floodfill[c] /*  - 0.5f */);
        } else {
          printf("       ");
        }
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
        printf("-------o");
      } else {
        printf("       o");
      }
    }
  }
  printf("\n");
}