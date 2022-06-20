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

simplebutton::Button* btn = new simplebutton::ButtonPullup(START_BTN);

uint32_t currentTime  = 0;

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


void setup() {
  Serial.begin(DEBUG_BAUD);
  delay(10);
  logger("BOOT", "...");
  cfg.setup();
  mytime.setup();
  neopixel.setup();
  wifi.setup();
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
