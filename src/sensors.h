#include <DHT_Async.h>

#ifndef INCUBATOR_SENSORS_H
#define INCUBATOR_SENSORS_H 1

bool measure_environment(DHT_Async *sensor, float *temperature, float *humidity);

#endif /* INCUBATOR_SENSORS_H */