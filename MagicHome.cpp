#include "MagicHome.h"
#include <PushbulletAPI.hpp>


void MagicHome::init(const char* ip, DEVICE_TYPE type, bool ka)
{
    // Initialize a device

    deviceIp = ip;
    API_PORT = 5577;
    deviceType = type;
    latestConnection = millis();

    keepAlive = ka;

    if (!client.connect(deviceIp, API_PORT)){
      // Serial.println("Connection to device failed");
      if (hasPushBullet == true){
        pb->pushNotifcation("MagicHome", "Connection to device failed"); 
      }
      delay(1000);
      return;
    }
}


void MagicHome::addPushBullet(char* TOKEN_PUSHBULLET)
{
    // Add Pushbullet for logging
    pb = new PushbulletAPI(TOKEN_PUSHBULLET);
    hasPushBullet = true;
}


void MagicHome::turnOff()
{
    switch (deviceType)
    {
    case RGB:
    case RGB_WW:
    case RGB_WW_CW:
    case BLUB_V4:
    {
        uint8_t buffer[] = {0x71, 0x24, 0x0F, 0xA4};
        sendBytes(buffer);
        break;
    }
    case BLUB_V3:
    {
        uint8_t buffer[] = {0xCC, 0x24, 0x33};
        sendBytes(buffer);
        break;
    }
    case RGB_RAW:
    {
        sendBytes(NULL, "q$\x0f\xa4");
        break;
    }
    }
}

void MagicHome::turnOn()
{
    switch (deviceType)
    {
    case RGB:
    case RGB_WW:
    case RGB_WW_CW:
    case BLUB_V4:
    {
        uint8_t buffer[] = {0x71, 0x23, 0x0F, 0xA3};
        sendBytes(buffer);
        break;
    }
    case BLUB_V3:
    {
        uint8_t buffer[] = {0xCC, 0x23, 0x33};
        sendBytes(buffer);
        break;
    }
    case RGB_RAW:
    {
        sendBytes(NULL, "q#\x0f\xa3");
        break;
    }
    }
}


void MagicHome::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t ww, uint8_t cw)
{
    switch (deviceType)
    {

    case RGB:
    case RGB_RAW:
    case RGB_WW:
    {
        uint8_t buffer[] = {0x31, r, g, b, ww, 0x00, 0x0f, 0x00};
        buffer[7] = checksum(buffer, 7);
        sendBytes(buffer);
        break;
    }

    case RGB_WW_CW:
    {
        uint8_t buffer[] = {0x31, r, g, b, ww, cw, 0x0f, 0x0f, 0x00};
        buffer[8] = checksum(buffer, 8);
        sendBytes(buffer);
        break;
    }

    case BLUB_V4:
    {
        if (ww != NULL){
          uint8_t buffer[] = {0x31, r, g, b, ww, 0x0f, 0x0f, 0x00};
          buffer[7] = checksum(buffer, 7);
          sendBytes(buffer);
        }else{
          uint8_t buffer[] = {0x31, r, g, b, 0x00, 0xf0, 0x0f, 0x00};
          buffer[7] = checksum(buffer, 7);
          sendBytes(buffer);
        }

        break;
    }

    case BLUB_V3:
    {
        if (ww != NULL){
          uint8_t buffer[] = {0x31, r, g, b, ww, 0x0f, 0xaa, 0x56, r, g, b, ww, 0x0f, 0xaa, 0x00};
          buffer[14] = checksum(buffer, 14);
          sendBytes(buffer);
        }else{
          uint8_t buffer[] = {0x56, r, g, b, 0x00, 0xf0, 0xaa};
          buffer[6] = checksum(buffer, 6);
          sendBytes(buffer);
        }
        break;
    }

    }
}


void MagicHome::sendBytes(uint8_t buffer[], char* msg)
{
    float checkConnectionTime = (millis() - latestConnection)/60;
    if (checkConnectionTime >= 290 || keepAlive == false){
        // Serial.println("Connection timed out, reestablishing.");

        if (!client.connect(deviceIp, API_PORT)){
          // Serial.println("Connection to device failed");
          if (hasPushBullet == true){
            pb->pushNotifcation("MagicHome", "Connection to device failed"); 
          }
          delay(1000);
          return;
        }
    }

    if (buffer != NULL){
        client.println((char *)buffer);
    }else{
      client.println(msg);
    }

    if (keepAlive == false){
        client.stop();
    }
}


uint8_t MagicHome::checksum(uint8_t *buffer, uint8_t size)
{
    uint8_t result = 0;
    for (uint8_t i = 0; i < size; i++)
    {
        result += buffer[i];
    }
    return result;
}
