#include "esp_system.h"

#ifndef INCUBATOR_SLEEP_H
#define INCUBATOR_SLEEP_H 1

unsigned long IRAM_ATTR micros();
void IRAM_ATTR delayMicroseconds(uint32_t us);

#endif /* INCUBATOR_SLEEP_H */