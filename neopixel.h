#ifndef neopixel_h
#define neopixel_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "cfg.h"
#include <Adafruit_NeoPixel.h>

extern uint32_t currentTime;
extern timeformat  myTime;
extern void prnt(String p);
extern void prntln(String p);
extern void logger(String module, String message);
extern String myTimeStr();
extern CFG    cfg;


class NeoPixel {
    public:
      void setup();
      void update();
      void draw();
      void enable();
      void disable();
      void toggle();
      // draws
      void drawTime(uint8_t h = 0, uint8_t m = 0);
      void drawSET();
      // px edit
      void setColor(uint8_t predef);
      void setColor(uint8_t r, uint8_t g, uint8_t b);
      void setColor(rgb c);
      void setColor(String hex);
      void setBgColor(uint8_t predef);
      void setBgColor(uint8_t r, uint8_t g, uint8_t b);
      void setBgColor(rgb c);
      void setBgColor(String hex);
      
      void setPxColor(uint8_t px, uint8_t predef);
      void setPxColor(uint8_t px, uint8_t r, uint8_t g, uint8_t b);
      void setPxColor(uint8_t px, rgb c);
      void setPxColor(uint8_t px, String hex);
      rgb getColor();
      rgb getBgColor();
      rgb getPxColor(uint8_t px);
      rgb hexRgb(String hex);
      void offPx(uint8_t px);
      bool isPxActive(uint8_t px);
      void setMode(uint8_t m);
      void debug();
      enum MOD {
        SET = 0,  // sviti SET 74, 75, 76
        HODINY = 1
      };
      byte mode = MOD::SET;
      enum BARVY {
        CERNA     = 0,
        CERVENA   = 1,
        ZELENA    = 2,
        MODRA     = 3,
        ZLUTA     = 4,
        FIALOVA   = 5,
        AZUROVA   = 6,
        BILA      = 7
      };
      void setUpdate(uint32_t u = 1000); 
      NeoPixel();
      ~NeoPixel();
    private:
      Adafruit_NeoPixel pixels = NULL;
      bool ready = false;
      bool enabled = false;
      uint32_t lastUpdate = 0;
      uint32_t updateAt = 1000;
      rgb backColor = {0, 0, 0};
      rgb mainColor = {0, 0, 0};
      rgb board[NUMPIXELS] = {};
 };
 #endif
