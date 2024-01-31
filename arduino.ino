#include <SPI.h>
#include <WiFiNINA.h>
#include <Wire.h>
#include "Grove_I2C_Motor_Driver.h"

// use I2C Scanner to find address of both Motor driver...
// right now we will use Left and Right motor side
#define I2C_FRONT_ADDRESS 0x08
#define I2C_REAR_ADDRESS 0x0A

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
int status = WL_IDLE_STATUS;
WiFiServer server(8888);

const char delimiter = ',';

I2CMotorDriver frontDriver;
I2CMotorDriver rearDriver;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    frontDriver.begin(I2C_FRONT_ADDRESS);
    rearDriver.begin(I2C_REAR_ADDRESS);
    startServer();
}

void loop()
{
    digitalWrite(LED_BUILTIN, LOW);
    WiFiClient client = server.available();
    if (client)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        while (client.connected())
        {
            if (client.available())
            {
                const char *message = client.readStringUntil('\n').c_str();
                char *token = strtok(const_cast<char *>(message), &delimiter);
                if (token != NULL)
                {
                    int value1 = atoi(token);
                    token = strtok(NULL, ",");

                    if (token != NULL)
                    {
                        int value2 = atoi(token);
                        Serial.println("Received values: V1 and V2");
                        Serial.println(value1);
                        Serial.println(value2);
                        processCommand(value1, value2);
                    }
                }
            }
        }
        client.stop();
    }
}

void startServer()
{

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

void processCommand(int leftMotorValue, int rightMotorValue)
{
    // Ensure motorValue is within the valid range
    if (leftMotorValue >= 100)
    {
        leftMotorValue = 100;
    }
    else
    {
        if (leftMotorValue <= -100)
        {
            leftMotorValue = -100;
        }
    }
    if (rightMotorValue >= 100)
    {
        rightMotorValue = 100;
    }
    else
    {
        if (rightMotorValue <= -100)
        {
            rightMotorValue = -100;
        }
    }

    frontDriver.speed(MOTOR1, leftMotorValue);
    frontDriver.speed(MOTOR2, rightMotorValue);
}
