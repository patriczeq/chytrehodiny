#ifndef neopixel_h
#define neopixel_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "cfg.h"
#include "mytime.h"
#include <Adafruit_NeoPixel.h>

extern uint32_t currentTime;
extern void prnt(String p);
extern void prntln(String p);
extern void logger(String module, String message);
extern CFG    cfg;
extern MYTIME    mytime;

class NeoPixel {
    public:
      void setup();
      void update();
      void draw();
      void enable();
      void disable();
      void toggle();
      void intro();
      // draws
      void drawTime(bool ineffect = false, bool disableDraw = false);
      void drawAnalog();
      void drawMatrix(bool v2);
      void drawFlame(bool v2);
      void drawSET();
      void setBoardMode(uint8_t bm);
      void setRedrawMode(uint8_t rm);
      void drawHeart();
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
      void setPxColor(pxcoor coor, rgb c);
      void setPxColor(pxcoor coor, uint8_t r, uint8_t g, uint8_t b);
      void fadePx(uint8_t px, uint8_t dec);
      void setBright(uint8_t b);
      uint8_t getBright();
      rgb getColor();
      rgb getBgColor();
      rgb getPxColor(uint8_t px);
      rgb hexRgb(String hex);
      String RgbHex(rgb color);
      String getPxColorHex(uint8_t px);
      rgb dimm(rgb color, uint8_t d);
      void offPx(uint8_t px);
      bool isPxActive(uint8_t px);
      bool pxUnder(uint8_t mpx, uint8_t px);
      void setMode(uint8_t m);
      void debug();
      void setWifi(uint8_t m);
      map_colrow col(uint8_t id);
      map_colrow row(uint8_t id);
      uint8_t getPxID(uint8_t x, uint8_t y);
      void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
      void drawCircle(uint8_t x0, uint8_t y0, uint8_t sz, rgb color);
      void drawCircle(uint8_t x0, uint8_t y0, uint8_t sz);

      void fillSquare(uint8_t x0, uint8_t y0, uint8_t sz, rgb color);
      void fillSquare(uint8_t x0, uint8_t y0, uint8_t sz);

      void useSensor(bool bol);
      uint8_t sensorValue();
      enum MOD {
        SET = 0,
        HODINY = 1,
        MATRIX = 2,
        MATRIX_V2 = 3,
        OHEN = 4,
        OHEN_V2 = 5,
        ANALOG = 6,
        SRDCE = 7
      };
      enum RMOD {
        NONE = 0,
        FALL = 1,
        SLIDE = 2,
        FADE = 3,
        MATRIX_R = 4
      };
      uint8_t mode = MOD::SET;
      uint8_t rmode = RMOD::NONE;
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
      uint16_t getUpdateAt();
      bool auto_brightness = true;
      NeoPixel();
      ~NeoPixel();
    private:
      Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
      enum WIFI_MOD{
        OFF  = 0,
        AP  = 1,
        ST = 2,
        CONN = 3
      };
      bool timePxls[NUMPIXELS];
      bool timePxlsNew[NUMPIXELS];
      void runRedrawEffect();
      uint8_t timePxLastRow(uint8_t col);
      uint8_t wifi_mode = 0;
      uint8_t wifi_counter = 0;
      bool ready = false;
      bool enabled = false;
      uint8_t bright = 0;
      uint32_t lastUpdate = 0;
      uint32_t updateAt = 500;
      uint32_t effect_slide = 0;
      uint32_t effect_speed = 500;
      rgb backColor = {0, 0, 0};
      rgb mainColor = {0, 0, 0};
      rgb board[NUMPIXELS];
      void check_bright();
      uint8_t matrix_max_frames = 0;
      bool redraw_done = true;
      uint8_t redraw_frame = 0;
      uint8_t redraw_speed = 100;
      uint32_t redraw_loop = 0;
      uint8_t matrix_rain_len[PX_COLS];
      int matrix_rain_pos[PX_COLS];
 };
 #endif
