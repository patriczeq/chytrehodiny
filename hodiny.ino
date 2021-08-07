extern "C" {
#include "user_interface.h"
}
#include "A_config.h"
#include "cfg.h"
#include "neopixel.h"
#include "wifi.h"
#include "mytime.h"
#include "src/SimpleButton/SimpleButton.h"

// Run-Time Variables //
CFG       cfg;
NeoPixel  neopixel;
WIFI      wifi;
MYTIME    mytime;


simplebutton::Button* btn = NULL;

uint32_t currentTime  = 0;
uint32_t prevTime     = 0;

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

void setup() {
  Serial.begin(DEBUG_BAUD);
  cfg.setup();
  mytime.setup();
  neopixel.setup();
  wifi.setup();
  btn = new simplebutton::ButtonPullup(START_BTN);
  btn->setOnHolding([](){cfg.FactoryReset();}, FACTORY_RESET_DELAY);
  btn->setOnClicked([](){wifi.pbc();});
}

void loop() {
  currentTime = millis();
  mytime.update();
  neopixel.update();
  wifi.update();
  btn->update();
}
