#include "floodfill_weigths_generator.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct cell_weigth {
  uint16_t speed;
  uint16_t time;
  uint16_t total_time;
  uint16_t penalty;
};

static float second_degree_equation(float a, float b, float c) {
  double delta = b * b - 4 * a * c;
  if (delta < 0) {
    return -1;
  }
  return (float)((-b + sqrt(delta)) / (2 * a));
}

static float time_penalty(uint16_t speed, uint16_t init_speed, uint16_t accel) {
  if (init_speed < speed) {
    return (speed - init_speed) / (float)accel;
  } else {
    return 0.0f;
  }
}

static float time_taken_distance(float distance, uint16_t speed, uint16_t max_speed, uint16_t accel) {
  if (speed < max_speed) {
    float time_to_max_speed = (max_speed - speed) / (float)accel;
    float time_to_distance = second_degree_equation(0.5f * accel, speed, -distance);
    if (time_to_distance < time_to_max_speed) {
      return time_to_distance;
    } else {
      float left_distance = distance - (speed * time_to_max_speed) + (0.5f * accel * time_to_max_speed * time_to_max_speed);
      return time_to_max_speed + (left_distance / max_speed);
    }
  } else {
    return distance / (float)speed;
  }
  return 0;
}

static uint16_t speed_after_time(float time, uint16_t speed, uint16_t max_speed, uint16_t accel) {
  if (speed < max_speed) {
    float speed_at_time = speed + (accel * time);
    return speed_at_time > max_speed ? max_speed : (uint16_t)speed_at_time;
  } else {
    return max_speed;
  }
}

static void weight_floodfill_table(float distance, uint16_t init_speed, uint16_t max_speed, uint16_t accel) {
  float time_to_max_speed = (max_speed - init_speed) / (float)accel;
  float distance_to_max_speed = (init_speed * time_to_max_speed) + (0.5f * accel * time_to_max_speed * time_to_max_speed);
  uint16_t cells_to_max_speed = (uint16_t)round((distance_to_max_speed / distance)) + 2 + 1;

  struct cell_weigth weights[cells_to_max_speed];

  uint16_t speed = init_speed;
  float time = distance / speed;
  float penalty = 0.0f;
  float total_time = time;
  weights[0].speed = speed;
  weights[0].time = (uint16_t)(time * 1000);
  weights[0].total_time = (uint16_t)(total_time * 1000);
  weights[0].penalty = (uint16_t)(penalty * 1000);
  for (uint16_t i = 1; i < cells_to_max_speed; i++) {
    time = time_taken_distance(distance, speed, max_speed, accel);
    total_time += time;
    speed = speed_after_time(time, speed, max_speed, accel);
    penalty = time_penalty(speed, init_speed, accel);
    weights[i].speed = speed;
    weights[i].time = (uint16_t)(time * 1000);
    weights[i].total_time = (uint16_t)(total_time * 1000);
    weights[i].penalty = (uint16_t)(penalty * 1000);
  }

  for (uint16_t i = 0; i < cells_to_max_speed; i++) {
    printf("CELL %d: \n", i + 1);
    printf("  Speed: %d\n", weights[i].speed);
    printf("  Time: %d\n", weights[i].time);
    printf("  Total Time: %d\n", weights[i].total_time);
    printf("  Penalty: %d\n", weights[i].penalty);
    printf("\n");
  }
}

void floodfill_generate_weights() {
  printf("STRAIGHT LINE\n");
  weight_floodfill_table(180.0f, 1000, 3435, 10000);

  printf("\nDIAGONAL\n");
  weight_floodfill_table(127.3f, 1000, 3435, 10000);
}