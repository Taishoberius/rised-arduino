#include "dht_nonblocking.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11
static const int DHT_SENSOR_PIN = 5; // matches D1 pin on esp8266
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);


void setup(){
    Serial.begin(115200);
}



/*
* Poll for a measurement, keeping the state machine alive.  Returns
* true if a measurement is available.
*/
static bool measure_environment(float *temperature, float *humidity) {
    static unsigned long measurement_timestamp = millis();

    /* Measure once every four seconds. */
    if (millis() - measurement_timestamp > 3000ul) {
        if( dht_sensor.measure(temperature, humidity) == true) {
            measurement_timestamp = millis();
            return(true);
        }
    }

    return(false);
}



/*
* Main program loop.
*/
void loop() {
    float temperature;
    float humidity;
    char msgT[15];
    char msgH[15];
    char temp[6];
    char hum[6];
    /* Measure temperature and humidity.  If the functions returns
    true, then a measurement is available. */
    if (measure_environment(&temperature, &humidity) == true) {
        dtostrf(temperature, 2, 0, temp);
        dtostrf(humidity, 2, 0, hum);
        sprintf(msgT, "%s/%s", temp, hum);
        Serial.println(msgT);
        //sprintf(msgT, "t%s", temp);
        //sprintf(msgH, "h%s", hum);
        //Serial.println(msgT);
        //Serial.println(msgH);
    }
}
