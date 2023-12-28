#include <LiquidCrystalIO.h>

#ifndef INCUBATOR_HELPERS_H
#define INCUBATOR_HELPERS_H 1

#define DEGREE_CHAR char(223)

const char UP_ARROW    = 0x00;
const char DOWN_ARROW  = 0x01;
const char HEATER_CHAR = 0x02;
const char HUMID_CHAR  = 0x03;

static unsigned char up_arrow[8]    = {0x04, 0x0E, 0x15, 0x04, 0x04, 0x04, 0x04, 0x00};
static unsigned char down_arrow[8]  = {0x00, 0x04, 0x04, 0x04, 0x04, 0x15, 0x0E, 0x04};
static unsigned char heater_char[8] = {0x00, 0x04, 0x15, 0x15, 0x15, 0x00, 0x1F, 0x1F};
static unsigned char humid_char[8] =  {0x04, 0x04, 0x0E, 0x0E, 0x1F, 0x1F, 0x0E, 0x04};

void defChar (LiquidCrystal &thelcd, int asc, const unsigned char row[8]);

#ifdef LOG_LOCAL_LEVEL
#undef LOG_LOCAL_LEVEL
#endif /* ifdef LOG_LOCAL_LEVEL */
#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#define CONFIG_LOG_MASTER_LEVEL CONFIG_LOG_DEFAULT_LEVEL_VERBOSE

#endif /* INCUBATOR_HELPERS_H */