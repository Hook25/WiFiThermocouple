# Wifi MAX6675 Thermocouple

The objective of this repository is to give a flash-and-forget implementation
for a WiFi-enabled Thermocouple usable from Home Assistant via the MQTT
integration.

## Configuration

Create a `env.h` file in `include/` where you place your configuration
variables. I suggest avoid placing them in the `cpp` file as you may commit
them by mistake.

Here is an example:
```c
#define STASSID "MyExampleAccessPointSSID" # SSID of the wifi network
#define STAPSK "My Very Secure Password :TM:"  # PWD of the wifi network
#define BROKER_ADDR IPAddress(192,168,1,253) # IP address of the MQTT broker
#define MQTT_USR "ConfiguredMQTTSensorUsername" # Username of the MQTT broker
#define MQTT_PWD "Yet another very secure password" # Password of the MQTT broker
```

## Hardware

I have used a MAX6675 sensor and a WiFi ESP8266 micro controller for this
project. If you decide to change the hardware, remember to add a new build
target to the `platformio.ini` file.

## Building and uploading

Open a terminal in the root of the repository and run the following:

```shell
$ pio run --target upload
```
