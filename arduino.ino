#include <Arduino.h>
#include <WiFiNINA.h>

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
const int serverPort = 8888;

const int varYpin = A0;
const int varXpin = A1;

int sensorValueX;
const int deadZoneLow = 448;
const int deadZoneHigh = 576;

void setup()
{
  Serial.begin(9600);
  Serial.println("Connecting to WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Attempting to connect to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop()
{
  int sensorValueX = analogRead(varXpin);
  if (sensorValueX >= deadZoneLow && sensorValueX <= deadZoneHigh)
  {
    sensorValueX = 512; // Resting position
  }
  int motorValue = map(sensorValueX, 0, 1023, -100, 100);

  // Connect to the server
  WiFiClient client;
  if (client.connect("192.168.4.1", serverPort))
  {
    Serial.println(motorValue);
    client.print(motorValue);
    client.stop();
  }
  else
  {
    Serial.println("Failed to connect to server");
  }
  delay(200);
}