#include <stdio.h>

#include "floodfill.h"
#include "floodfill_weigths_generator.h"
#include "maze.h"

int main() {
  // initialize_maze();
  // floodfill_generate_weights();
  update_floodfill(false);
  floodfill_maze_print();

  return 0;
}