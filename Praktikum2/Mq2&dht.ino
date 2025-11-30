#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Wire.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define MQ2_PIN 34

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// ------- MQTT RECONNECT -------
void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Terhubung!");
    } else {
      Serial.print("Gagal, rc=");
      Serial.print(client.state());
      Serial.println(" Coba lagi dalam 2 detik");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // ---- WIFI SETUP ----
  Serial.println("Menghubungkan WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi Terhubung!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // ---- MQTT SETUP ----
  client.setServer(mqtt_server, mqtt_port);
  reconnect();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // ------- SENSOR READ LOOP -------
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int gasValue = analogRead(MQ2_PIN);

  Serial.print("Temp: "); Serial.print(temp);
  Serial.print("  Hum: "); Serial.print(hum);
  Serial.print("  Gas: "); Serial.println(gasValue);

  client.publish("iot/suhu", String(temp).c_str());
  client.publish("iot/kelembapan", String(hum).c_str());
  client.publish("iot/gas", String(gasValue).c_str());

  delay(2000);
}
