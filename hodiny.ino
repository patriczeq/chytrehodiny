extern "C" {
#include "user_interface.h"
}
#include "A_config.h"
#include "cfg.h"
#include "neopixel.h"
#include "wifi.h"
#include "src/SimpleButton/SimpleButton.h"

// Run-Time Variables //
CFG       cfg;
NeoPixel  neopixel;
WIFI      wifi;


simplebutton::Button* startb = new simplebutton::ButtonPullup(START_BTN);;

uint32_t currentTime  = 0;
uint32_t prevTime     = 0;
timeformat  myTime    = {0, 0, 0};

uint8_t tz = NTP_TIMEZONE_GMT;

void prnt(String p){
  Serial.print(p);
}
void prntln(String p){
  Serial.println(p);
}
void prntln(){
  Serial.println();
}

void logger(String module, String message){
  prntln("[" + module + "]: " + message);
}
void setMyTime(timeformat t){
  myTime = t;
  logger("TIME", "Nastaven čas " + myTimeStr());
}

void setMyTime(uint8_t h, uint8_t m, uint8_t s){
  setMyTime(timeformat({h,m,s}));
}
String myTimeStr(){
  return String(myTime.h < 10 ? "0" + String(myTime.h) : String(myTime.h)) + ":" + String(myTime.m < 10 ? "0" + String(myTime.m) : String(myTime.m)) + ":" + String(myTime.s < 10 ? "0" + String(myTime.s) : String(myTime.s));
}

String str(const char* ptr) {
  char keyword[strlen_P(ptr)];

  strcpy_P(keyword, ptr);
  return String(keyword);
}

char* s2ch(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  }
}


void tik(){
  myTime.s++;
  if(myTime.s >= 60)
    {
      myTime.s = 0;
      myTime.m++;
    }
   if(myTime.m >= 60)
    {
      myTime.m = 0;
      myTime.h++;
    }
    if(myTime.h > 23)
      {
        myTime.h = 0;
      }
}

void setup() {
  Serial.begin(9600);
  prntln("Startuji...");
  cfg.setup();
  neopixel.setup();
  wifi.setup();
  startb->setOnHolding([](){
    cfg.FactoryReset();
  },5000);

  startb->setOnClicked([](){
    wifi.pbc();
  });

  /*startb->setOnClicked([](){
    neopixel.debug();
  });*/
}

void loop() {
  currentTime = millis();
  startb->update();
  neopixel.update();
  wifi.update();
  if(currentTime - prevTime >= 1000) // přidat sekundu do interního času
    {
      prevTime = currentTime;
      tik();
    }
}
