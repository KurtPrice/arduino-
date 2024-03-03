#include <WiFi101.h>
#include <ArduinoMqttClient.h>
#include "secrets.h"

const char BROKER[] = "10.0.0.94"; // using DNS did not work
int port = 1883;
char light_topic[] = "homeassistant/garden/sunlight";

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial to begin
  }

  while (WiFi.begin(WIFI_SSID, WIFI_PASS) != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);
  }

  mqttClient.setUsernamePassword(MQTT_USER, MQTT_PASS);

  Serial.println("Attempting to connect to mqtt broker");
  if (!mqttClient.connect(BROKER, port)) {
    Serial.print("Failure to connect to MQTT Broker: ");
    Serial.println(mqttClient.connectError());
    while(1) {
      ;
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  mqttClient.poll();
  if(mqttClient.available()){
    Serial.println(mqttClient.read());
  }

  delay(1000);

  int resistance_1 = analogRead(A0);
  int resistance_2 = analogRead(A1);
  int average_resistance = (resistance_1 + resistance_2) / 2;

  Serial.print("Avg:\t");
  Serial.println(average_resistance);

  mqttClient.beginMessage(light_topic);
  mqttClient.print("{\nresistance: ");
  mqttClient.println(average_resistance);
  mqttClient.print("}");
  mqttClient.endMessage();
}
