#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include "Grove_I2C_Motor_Driver.h"

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;

WiFiServer server(8888);

void setup()
{
  Motor.begin(0x0f);

  startServer();
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        char command = client.read();
        processCommand(command);
      }
    }
    client.stop();
  }
}

void startServer()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Access Point Web Server");
  // test de connection au module wifi
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("echec de communication avec wifi module ");

    while (true)
      ;
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println("Please upgrade the firmware");
  }

  Serial.print("Creating access point named :");
  Serial.println(ssid);

  status = WiFi.beginAP(ssid, pass);
  if (status != WL_AP_LISTENING)
  {
    Serial.println("Creating access point failed");

    while (true)
      ;
  }

  delay(1000);

  server.begin();

  // wifi status => connected
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void processCommand(char command)
{
  int intValue = atoi(&command);
  switch (intValue)
  {
  case 1:
    // forward the motor
    Motor.speed(MOTOR1, 100);
    Motor.speed(MOTOR2, 100);
    break;
  case 2:
    // reverse the motor
    Motor.speed(MOTOR1, -100);
    Motor.speed(MOTOR2, -100);
    break;
  default:
    // Stop the motor
    Motor.speed(MOTOR1, 0);
    Motor.speed(MOTOR2, 0);
    break;
  }
}
