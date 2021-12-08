#ifndef MOVING_AVG_H
#define MOVING_AVG_H

#include <Arduino.h>

typedef struct {
  int window_size; //Aym: so this acts as your buffer? like a temporal window?
  int* queue; // queue[0] should be most recent value
} MovingAverage;

/**
 * @brief Understanding the MovingAverage structure
 * @param window_size integer giving to the temporal window of force commands
 * for controls
 * @param queue list of integers that will be of length window_size
 */


//You do not need to return a MovingAverage structure!
//Initializing a static structure is sufficient!

void construct_moving_average(int window_size, MovingAverage* avg) {
  avg->window_size = window_size;
  avg->queue = (int*) malloc(sizeof(int) * window_size);
}

void deconstruct_moving_average(MovingAverage* avg) {
  free(avg->queue);
  free(avg);
}

void add_moving_average_value(MovingAverage* avg, int val) {
  int i;
  for (i = avg->window_size - 1; i > 0; i--) {
    avg->queue[i] = avg->queue[i-1];
  }
  avg->queue[0] = val;
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