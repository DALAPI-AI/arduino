#include <Arduino.h>
#include <WiFiNINA.h>

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
const int serverPort = 8888;

const int varYpin = A0;
const int varXpin = A1;

int sensorValueX;

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
  Serial.println(sensorValueX);
  if (sensorValueX > 723)
  {
    sensorValueX = 1;
  }
  else
  {
    if (sensorValueX < 323)
    {
      sensorValueX = 2;
    }
    else
    {
      sensorValueX = 0;
    }
  }
  // Connect to the server
  WiFiClient client;
  if (client.connect("192.168.4.1", serverPort))
  {
    Serial.println(sensorValueX);
    client.print(sensorValueX);
    client.stop();
  }
  else
  {
    Serial.println("Failed to connect to server");
  }
  delay(200);
}