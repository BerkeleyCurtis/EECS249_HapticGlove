#ifndef MOVING_AVG_H
#define MOVING_AVG_H

#include <Arduino.h>


typedef struct {
  int window_size;
  int* queue; // queue[0] should be most recent value
} MovingAverage;

static MovingAverage* construct_moving_average(int window_size) {
  MovingAverage* avg = (MovingAverage*) malloc(sizeof(MovingAverage));
  avg->window_size = window_size;
  avg->queue = (int*) malloc(sizeof(int) * window_size);
  return avg;
}

static void deconstruct_moving_average(MovingAverage* avg) {
  free(avg->queue);
  free(avg);
}

static MovingAverage* add_moving_average_value(MovingAverage* avg, int val) {
  int i;
  for (i = avg->window_size - 1; i > 0; i--) {
    avg->queue[i] = avg->queue[i-1];
  }
  avg->queue[0] = val;
  return avg;
}

static float get_moving_average_value(MovingAverage* avg) {
  float total = 0;
  int i;
  for (i = 0; i < avg->window_size; i++) {
    total += avg->queue[i];
  }
  return total / avg->window_size;
}

static float update_moving_average_value(MovingAverage* avg, int val) {
  add_moving_average_value(avg, val);
  return get_moving_average_value(avg);
}

#endif