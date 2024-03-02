#include <WiFi101.h>
#include "Adafruit_Si7021.h"
#include <ArduinoMqttClient.h>
#include "secrets.h"

const char BROKER[] = "homeassistant.local";
int port = 1883;
char humidity_topic[] = "/garden/jalapenos/humidity";
char temperature_topic[] = "/garden/jalapenos/temperature";

Adafruit_Si7021 sensor = Adafruit_Si7021();
WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  while(!Serial) {; // wait for serial to begin
  }
  sensor.begin();

  while(WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  mqttClient.setUsernamePassword(MQTT_USER, MQTT_PASS);

  if (!mqttClient.connect(BROKER, port)) {
    Serial.println(mqttClient.connectError());
    exit(8);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  mqttClient.poll();

  mqttClient.beginMessage(humidity_topic);
  mqttClient.print(sensor.readHumidity());
  mqttClient.endMessage();

  mqttClient.beginMessage(temperature_topic);
  mqttClient.print(sensor.readTemperature());
  mqttClient.endMessage();
  delay(10000);

}
