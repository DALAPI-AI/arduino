#include <Arduino.h>
#include <WiFiNINA.h>

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
const int serverPort = 8888;

const int varYpin = A0;
const int varXpin = A1;

int sensorValueX;
const int deadZoneLow = 512 - 32;
const int deadZoneHigh = 512 + 32;

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
  int motorValue;
  if (sensorValueX > deadZoneHigh)
  {
    motorValue = map(sensorValueX, deadZoneHigh, 1023, 0, 100);
  }
  else
  {
    if (sensorValueX < deadZoneLow)
    {
      motorValue = map(sensorValueX, 0, deadZoneLow, -100, 0);
    }
    else
    {
      motorValue = 0;
    }
  }

  // Connect to the server
  WiFiClient client;
  if (client.connect("192.168.4.1", serverPort))
  {
    Serial.println(motorValue);
    client.println(motorValue);
    client.stop();
  }
  else
  {
    Serial.println("Failed to connect to server");
  }
  delay(200);
}