# MagicHome-ESP8266
Based on [MagicHome-Python](https://github.com/adamkempenich/magichome-python)

ESP8266 Library for turn on, turn off and change color of MagicHome devices.

## DEVICE_TYPE

```
    0: RGB
    1: RGB+WW
    2: RGB+WW+CW
    3: Blub (v.4+)
    4: Blub (v.3-)
    5: RGB_RAW (raw binary requests)
```

## Install
* Download ZIP of this repository
* In Arduino IDE
  * Sketch > Include Library > Add .ZIP Library...
  * Select the ZIP of this repository

## Basic Example
```cpp
// #include <Arduino.h>
#include <ESP8266WiFi.h>
#include <MagicHome.hpp>

const char* ssid = "<SSID>";
const char* password = "<WIFI-Password>";
const char* pushbullet_key = "<Pushbullet API key>";
const char* pushbullet_fingerprint = "<Pushbullet API SSL Fingerprint>";

MagicHome led;
// optional to get the log on your smartphone
char* TOKEN_PUSHBULLET = "YOUR_TOKEN";
led.addPushBullet(TOKEN_PUSHBULLET);
// DEVICE_TYPE -> Default RGB
// KeepAlive -> Default true
led.init("LOCAL_IP", (MagicHome::DEVICE_TYPE)5, true);

void setup()
{
    // Set baud for serial data
    Serial.begin(9600);

    // Set & Connect to WiFi network
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
    delay(500);
    Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");

    led.turnOn();
    delay(1000);
    // Red, Green, Blue, white1, white2
    led.setColor(255, 165, 0, 0, 0);
    delay(1000);
    led.turnOff();


}

void loop()
{
}
```
