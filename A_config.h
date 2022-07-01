#ifndef config_h
#define config_h

  #define MYVERSION 1.67
  #define PX_COLS 11
  #define PX_ROWS 11
  #define NUMPIXELS PX_COLS * PX_ROWS
  #define DEBUG_BAUD 9600
  #define NEOPIXEL_PIN  D6 
  #define START_BTN  0
  #define LED_PIN  2
  #define SETTINGS_ADDR 100
  #define EEPROM_SIZE 4095
  #define MYNUM 920820
  #define FACTORY_RESET_DELAY 5000
  #define UPDATE_CHECK "http://eders.cz/hodiny/" // {"current":1}
  #define FIRMWARE_URL "http://eders.cz/hodiny/?d" // ?d&v=VERSION 
  //#define LED_MATRIX_ZIGZAG // zapojení do Z
  #define AP_SSID "Hodiny"
  #define AP_PWD ""
  #define HOSTNAME "hodiny"
  #define NTP_TIMEZONE_GMT 2
  #define ST_SSID_MAXLEN 32
  #define ST_PASS_MAXLEN 64
  #define MSG_MAXLEN 256
  
  #define ST_SSID ""
  #define ST_PASS "" 
  struct map_colrow{
    uint8_t     px[PX_ROWS];
  };
  struct pxCoor{
    uint8_t     x;
    uint8_t     y;
  };
  struct pxChar{
    uint8_t     len;
    const       pxCoor* px;
  };
  struct network{
    char        ssid[ST_SSID_MAXLEN + 1];
    char        password[ST_PASS_MAXLEN + 1];
  };
  struct rgb{
    uint8_t     r;
    uint8_t     g;
    uint8_t     b;
  };
  struct timeformat{
    uint8_t     h;
    uint8_t     m;
    uint8_t     s;
    uint16_t    ms;
  };
  struct dateformat{
    uint16_t    y;
    uint8_t     m;
    uint8_t     d;
  };
  struct datetimeformat{
    dateformat  d;
    timeformat  t;
  };
  struct pxcoor{
    uint8_t     x;
    uint8_t     y;
  };

  struct schedule{
    bool       enable;
    timeformat from;
    timeformat to;
    uint8_t    bright;
  };
  
  struct cfgObject{
    network   client;
    rgb       mainColor;
    rgb       bgColor;
    uint8_t   bright;
    uint8_t   speed;
    uint8_t   timeZone;
    uint8_t   wifiMode;
    uint8_t   boardMode;
    uint32_t  mynum;
    bool      setupComplete;
    char      msg[MSG_MAXLEN];
    bool      useDST;
    rgb       customBG[NUMPIXELS];
    schedule  brightSchedule;
  };

#endif
