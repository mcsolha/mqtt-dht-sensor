#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include "DHT.h"
#include "config.h"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, mqtt_server, mqtt_port, mqtt_username, mqtt_passwd);

Adafruit_MQTT_Publish temp_mqtt = Adafruit_MQTT_Publish(&mqtt, mqtt_username "/iot/temp");
Adafruit_MQTT_Publish humid_mqtt = Adafruit_MQTT_Publish(&mqtt, mqtt_username "/iot/humid");


// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void MQTT_connect();

void setup() {
  pinMode(5, INPUT);
  Serial.begin(115200);
  Serial.println("DHT11 test!");

  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(2, INPUT);
  Serial.println("DHT11 MQTT Client");
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  blink(5);

  dht.begin();
}

void loop() {
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humid = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temp = dht.readTemperature();

  if (isnan(humid) || isnan(temp)) {
    Serial.println("Failed to read DHT!");
    return;
  }

  if (! temp_mqtt.publish(temp)) {
      Serial.println(F("Failed temp mqtt"));
  } else {
    Serial.println(F("OK!"));
  }

  if (! humid_mqtt.publish(humid)) {
      Serial.println(F("Failed humid mqtt"));
  } else {
    Serial.println(F("OK!"));
  }

  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print(" *C ");
}
