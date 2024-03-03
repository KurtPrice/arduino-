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
    mqttClient.read();
  }

  delay(10000);

  int resistance_1 = analogRead(A0);
  int resistance_2 = analogRead(A1);
  int average_resistance = (resistance_1 + resistance_2) / 2;

  Serial.print("Resistor 1:\t");
  Serial.print(resistance_1);
  Serial.print("\tResistor 2:\t");
  Serial.print(resistance_2);
  Serial.print("\tAvg:\t");
  Serial.println(average_resistance);

  mqttClient.beginMessage(light_topic);
  mqttClient.println(average_resistance);
  mqttClient.endMessage();
}
