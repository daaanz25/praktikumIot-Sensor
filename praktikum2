#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <rgb_lcd.h>

// DHT
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Ultrasonik single-pin
#define ULTRA_PIN 5

// Grove LCD
rgb_lcd lcd;

// WiFi
const char* ssid = "Elissss";
const char* password = "12345678";

// ThingSpeak
String apiKey = "1CN3RXQDN58KLTUA";

// Ultrasonic function
long readUltrasonic() {
  pinMode(ULTRA_PIN, OUTPUT);
  digitalWrite(ULTRA_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(ULTRA_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_PIN, LOW);

  pinMode(ULTRA_PIN, INPUT);
  long duration = pulseIn(ULTRA_PIN, HIGH, 30000);

  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // LCD init
  lcd.begin(16, 2);
  lcd.setRGB(0, 128, 255); // Warna biru muda

  lcd.print("Connecting WiFi");

  // WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WiFi Connected");
  delay(1000);
}

void loop() {
  // Baca sensor
  float suhu = dht.readTemperature();
  float hum = dht.readHumidity();
  long jarak = readUltrasonic();

  Serial.print("Suhu: "); Serial.print(suhu);
  Serial.print(" C | Hum: "); Serial.print(hum);
  Serial.print(" % | Jarak: "); Serial.print(jarak);
  Serial.println(" cm");

  // Tampilkan ke LCD Grove
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:"); lcd.print(suhu);
  lcd.print("C H:"); lcd.print(hum);

  lcd.setCursor(0, 1);
  lcd.print("Jarak: ");
  lcd.print(jarak);
  lcd.print("cm");

  // Kirim ke ThingSpeak
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.thingspeak.com/update?api_key=" + apiKey +
                 "&field1=" + String(jarak) +
                 "&field2=" + String(suhu) +
                 "&field3=" + String(hum);

    http.begin(url);
    int code = http.GET();
    http.end();
  }

  delay(2000);
}
