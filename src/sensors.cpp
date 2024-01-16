#include <DHT_Async.h>
#include "sensors.h"

bool measure_environment(DHT_Async *sensor, float *temperature, float *humidity)
{
    static unsigned long measurement_timestamp = millis();

    /* Measure once every four seconds. */
    if (millis() - measurement_timestamp > 2000ul)
    {
        if (sensor->measure(temperature, humidity))
        {
            measurement_timestamp = millis();
            return true;
        }
    }

    return false;
}