#ifndef config_h
#define config_h

  #define NUMPIXELS 121
  #define DEBUG_BAUD 9600
  #define NEOPIXEL_PIN  6 
  #define START_BTN  0
  #define LED_PIN  2
  #define SETTINGS_ADDR 100
  #define EEPROM_SIZE 4095
  #define MYNUM 920820
  #define FACTORY_RESET_DELAY 5000

  #define AP_SSID "Hodiny"
  #define HOSTNAME "hodiny"
  #define NTP_TIMEZONE_GMT 2
  
  #define ST_SSID ""
  #define ST_PASS "" 
  
  struct network{
    char ssid[33];
    char password[65];
  };
  struct rgb{
    uint8_t r;
    uint8_t g;
    uint8_t b;
  };
  struct timeformat{
    uint8_t h;
    uint8_t m;
    uint8_t s;
  };
  struct dateformat{
    uint16_t y;
    uint8_t m;
    uint8_t d;
  };
  struct datetimeformat{
    dateformat d;
    timeformat t;
  };

  struct cfgObject{
    network   client;
    rgb       mainColor;
    rgb       bgColor;
    uint8_t   bright;
    uint8_t   timeZone;
    uint8_t   wifiMode;
    uint8_t   boardMode;
    uint32_t  mynum;
    bool      setupComplete;
  };

#endif
