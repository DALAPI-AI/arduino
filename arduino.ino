#include <Arduino.h>
#include <WiFiNINA.h>

char ssid[] = "ArduinoMotor";
char pass[] = "12345678";
const int serverPort = 8888;

const int varYpin = A0;
const int varXpin = A1;

int sensorValueX;
int sensorValueY;
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
    sensorValueX = analogRead(varXpin);
    sensorValueY = analogRead(varYpin);

    // Check for deadZone for X and Y axis
    if (sensorValueX >= deadZoneLow && sensorValueX <= deadZoneHigh)
    {
        sensorValueX = 512; // Resting position
    }
    if (sensorValueY >= deadZoneLow && sensorValueY <= deadZoneHigh)
    {
        sensorValueY = 512; // Resting position
    }

    int xMotorVal;
    int yMotorVal;

    if (sensorValueX > deadZoneHigh)
    {
        xMotorVal = map(sensorValueX, deadZoneHigh, 1023, 0, 100);
    }
    else
    {
        if (sensorValueX < deadZoneLow)
        {
            xMotorVal = map(sensorValueX, 0, deadZoneLow, -100, 0);
        }
        else
        {
            xMotorVal = 0;
        }
    }
    if (sensorValueY > deadZoneHigh)
    {
        yMotorVal = map(sensorValueY, deadZoneHigh, 1023, 0, 100);
    }
    else
    {
        if (sensorValueY < deadZoneLow)
        {
            yMotorVal = map(sensorValueY, 0, deadZoneLow, -100, 0);
        }
        else
        {
            yMotorVal = 0;
        }
    }
    int leftMotor = xMotorVal + yMotorVal;
    int rightMotor = xMotorVal - yMotorVal;
    if (leftMotor < -100)
    {
        leftMotor = -100;
    }else if (leftMotor > 100)
    {
        leftMotor = 100;
    }
    if (rightMotor < -100)
    {
        rightMotor = -100;
    }else if (rightMotor > 100)
    {
        rightMotor = 100;
    }
    
    
    // Connect to the server
    WiFiClient client;
    if (client.connect("192.168.4.1", serverPort))
    {
        String message = String(rightMotor) + "," + String(leftMotor);
        Serial.println(message);
        client.println(message);
        client.stop();
    }
    else
    {
        Serial.println("Failed to connect to server");
    }
    delay(200);
}