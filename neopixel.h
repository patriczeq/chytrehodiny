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
      bool isOffline = true;
      // font
      String CharMSG = "";
      uint16_t CharShift = 0;
      uint16_t CharSwing = 0;
      int CharPxLen = 0;
      void CharAppend(uint8_t b);
      void CharAppend(char s);
      void CharDraw();
      void CharStrSet(String msg);
      void CharStrSetTime();
      void ClearLED_MATRIX();
      // draws
      void drawTime(bool ineffect = false, bool disableDraw = false);
      void drawMatrix(bool v2);
      void drawFlame(bool v2);
      void drawSET();
      void setBoardMode(uint8_t bm);
      void drawHeart();
      void drawIP();
      void drawCustom();
      void drawMSG(bool showTime = false);
      // board edit
      void setColor(uint8_t predef);
      void setColor(uint8_t r, uint8_t g, uint8_t b);
      void setColor(rgb c);
      void setColor(String hex);
      void setBgColor(uint8_t predef);
      void setBgColor(uint8_t r, uint8_t g, uint8_t b);
      void setBgColor(rgb c);
      void setBgColor(String hex);
      // px edit
      void setPxColor(uint8_t px, rgb c);
      void setPxColor(uint8_t px, uint8_t c);
      /*void setPxColor(uint8_t px, uint8_t r, uint8_t g, uint8_t b);*/
      void setPxColor(uint8_t px, String hex);
      void setPxColor(pxcoor coor, rgb c);
      /*void setPxColor(pxcoor coor, uint8_t r, uint8_t g, uint8_t b);*/

      void fadePx(uint8_t px, uint8_t dec);
      void setBright(uint8_t b);
      uint8_t getBright();
      void setSpeed(uint8_t s);
      uint8_t getSpeed();
      rgb getColor();
      rgb getBgColor();
      rgb getPxColor(uint8_t px);
      rgb hexRgb(String hex);
      rgb getBasicColor(uint8_t color);
      String RgbHex(rgb color);
      String getPxColorHex(uint8_t px);
      rgb dimm(rgb color, uint8_t d);
      bool isPxActive(uint8_t px);
      bool pxUnder(uint8_t mpx, uint8_t px);
      void setMode(uint8_t m);
      void debug();
      void setWifi(uint8_t m);
      map_colrow col(uint8_t id);
      map_colrow row(uint8_t id);
      uint8_t getPxID(uint8_t x, uint8_t y);

      void singleNumberPrint(uint8_t n, rgb color = {255, 255, 255});

      enum MOD {
        SET = 0,
        HODINY = 1,
        MATRIX = 2,
        MATRIX_V2 = 3,
        OHEN = 4,
        OHEN_V2 = 5,
        SRDCE = 7,
        SECONDS = 8,
        MINUTES = 9,
        SHOWIP = 10,
        MSGBG = 20,
        MSG = 21,
        DIGITIME = 22,
        DOW = 23,
        SVATEK = 24,
        WIFI_JEDE = 199,
        CUSTOM = 200
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
      rgb S_BARVY[8] = {
        {  0,  0,  0},  //cerna
        {255,  0,  0},  //cervena
        {  0,255,  0},  //zelena
        {  0,  0,255},  //modra
        {255,167,  0},  //zluta
        {255,  0,255},  //azurova
        {255,255,255}   //bila
      };

      void setUpdate(uint32_t u = 1000); 
      uint16_t getUpdateAt();
      void setCustomBg(uint8_t px, rgb color);
      String strIP = "0.0.0.0";
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
      uint8_t speed = 100;
      uint32_t lastUpdate = 0;
      uint32_t updateAt = 500;
      uint32_t effect_slide = 0;
      uint32_t effect_speed = 500;
      uint8_t ipprintpos = 0;
      rgb backColor = {0, 0, 0};
      rgb mainColor = {0, 0, 0};
      rgb board[NUMPIXELS];
      rgb customBG[NUMPIXELS];
      bool LED_MATRIX[PX_COLS][PX_ROWS];
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
