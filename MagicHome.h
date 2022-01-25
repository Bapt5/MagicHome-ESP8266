#ifndef DEF_MAGICHOME
#define DEF_MAGICHOME
#include <WiFiClient.h> // Module wifi
#include <PushbulletAPI.hpp>

class MagicHome{
  // Representation of a MagicHome device

  private:
    WiFiClient client;
    PushbulletAPI* pb;
    bool hasPushBullet;

    uint8_t checksum(uint8_t *buffer, uint8_t size);
    void sendBytes(uint8_t buffer[]=NULL, char* msg=NULL);

  public:
    typedef enum
      {
          RGB = 0,
          RGB_WW = 1,
          RGB_WW_CW = 2,
          BLUB_V4 = 3,
          BLUB_V3 = 4,
          RGB_RAW = 5
      } DEVICE_TYPE;

    const char* deviceIp;
    DEVICE_TYPE deviceType;
    std::uint16_t API_PORT;
    unsigned long latestConnection;
    bool keepAlive;

    void init(const char* ip, DEVICE_TYPE type=RGB, bool ka=true);
    void addPushBullet(char* TOKEN_PUSHBULLET);
    void turnOff();
    void turnOn();
    void setColor(uint8_t r=0, uint8_t g=0, uint8_t b=0, uint8_t ww=NULL, uint8_t cw=NULL);
};

#endif
