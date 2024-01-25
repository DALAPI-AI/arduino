#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include "Grove_I2C_Motor_Driver.h"
#define I2C_ADDRESS 0x0F

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;

WiFiServer server(8888);

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);
    Motor.begin(I2C_ADDRESS);
    startServer();
}

void loop()
{
    digitalWrite(LED_BUILTIN,LOW);
    WiFiClient client = server.available();
    if (client)
    {
        digitalWrite(LED_BUILTIN,HIGH);
        while (client.connected())
        {
            if (client.available())
            {
                int command = atoi(client.readStringUntil('\n').c_str());
                processCommand(command);
            }
        }
        client.stop();
    }
}

void startServer()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.println("Access Point Server");
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

void processCommand(int motorValue)
{
    // Ensure motorValue is within the valid range
    motorValue = constrain(motorValue, -100, 100);

    Motor.speed(MOTOR1, motorValue);
    Motor.speed(MOTOR2, motorValue);
}
