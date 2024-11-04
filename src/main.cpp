#include "max6675.h"
#include <ESP8266WiFi.h>
#include <ArduinoHA.h>

#include "env.h"

#define SENSOR_VERSION "1.0.0"
#define SENSOR_MANUFACTURER "Hook25"
#define SENSOR_MODEL "max6675_wifi_thermocouple"

/*
// these are defined in env.h
#define STASSID # SSID of the wifi network
#define STAPSK  # PWD of the wifi network
#define BROKER_ADDR # IP address of the MQTT broker
#define MQTT_USR # Username of the MQTT broker
#define MQTT_PWD # Password of the MQTT broker
*/
#define THERMO_DO D0
#define THERMO_CS D1
#define THERMO_CLK D2
MAX6675 thermocouple(THERMO_CLK, THERMO_CS, THERMO_DO);

WiFiClient client;
byte mac[] = { 0x92, 0x4D, 0xC8, 0xF8, 0x29, 0x16 };
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(client, device);
HASensorNumber temperatureSensor("temperature", HASensorNumber::PrecisionP1);
HASensorNumber signalSensor("wifi_signal_strength", HASensorNumber::PrecisionP0);

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }

  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  device.setName("uc25_wifi_termocouple");
  device.setModel(SENSOR_MODEL);
  device.setSoftwareVersion(SENSOR_VERSION);
  device.setManufacturer(SENSOR_MANUFACTURER);

  temperatureSensor.setUnitOfMeasurement("°C");
  temperatureSensor.setIcon("mdi:temperature-celsius");
  temperatureSensor.setName("temperature");

  signalSensor.setUnitOfMeasurement("dBm");
  signalSensor.setIcon("mdi:wifi");
  signalSensor.setName("wifi_signal_strength");

  device.enableSharedAvailability();
  device.enableLastWill();

  mqtt.begin(BROKER_ADDR, MQTT_USR, MQTT_PWD);
}

float last_read_temperature = 0;

void loop() {
  mqtt.loop();
  delay(10000);

  float temperature = thermocouple.readCelsius();
  if (abs(temperature - last_read_temperature) > 1) {
    int32_t rssi = WiFi.RSSI();
    temperatureSensor.setValue(temperature);
    signalSensor.setValue(rssi);

    last_read_temperature = temperature;
    Serial.printf("Sent temperature: %.1f °C -- rssi: %d dBm\n", last_read_temperature, rssi);
  } else {
    Serial.println("Nothing new to report");
  }
}
