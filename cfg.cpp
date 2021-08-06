#include "cfg.h"

/*
struct cfgObject{
    network   client;
    rgb       mainColor;
    rgb bgColor;
    uint8_t   timeZone;
    uint8_t   wifiMode;
    uint8_t   boardMode;
    uint32_t  mynum;
  };
*/
CFG::CFG() {}
CFG::~CFG() {}
void CFG::setup(){
  if(!load()){
    defaults();
  }
}
void CFG::defaults(){
  
  data.client     = network {ST_SSID, ST_PASS};
  data.mainColor  = rgb {255, 255, 255};
  data.bgColor    = rgb {0, 0, 0};
  data.timeZone   = NTP_TIMEZONE_GMT;
  data.wifiMode   = 2;  // 1 AP, 2 ST
  data.boardMode  = 0;
  data.mynum      = MYNUM;
  data.setupComplete = false;
}

void CFG::FactoryReset(){
  // format EEPROM
  for (unsigned long i = 0; i < EEPROM_SIZE; i++) EEPROM.write(i, 0x00);
  EEPROM.commit();
  delay(500);
  ESP.reset();
}

/* EEPROM */
void CFG::save(bool reboot){
  if(!data.setupComplete){
    data.setupComplete = true;
    data.boardMode = 1;
  }
  EEPROM.begin(EEPROM_SIZE);
  saveObject(SETTINGS_ADDR, data);
  logger("CFG", "Konfigurace byla ulozena!");
  if(reboot){
    delay(500);
    ESP.reset();
  }
}

bool CFG::load(){
  cfgObject newData;
  EEPROM.begin(EEPROM_SIZE);
  getObject(SETTINGS_ADDR, newData);
  if(newData.mynum == MYNUM)
    {
      data = newData;
      logger("CFG", "Konfigurace byla nactena z pameti EEPROM");
      return true;
    }
    logger("CFG", "Konfigurace nebyla nalezena. Zavadim vychozi nastaveni!");
  return false;
}

/* getters */

bool CFG::setupCmplt(){
  return data.setupComplete;
}
network CFG::getNetwork(){
  return data.client;
}
rgb CFG::getMainColor(){
  return data.mainColor;
}
byte CFG::getTimeZone(){
  return data.timeZone;
}
byte CFG::getWifiMode(){
  return data.wifiMode;
}
byte CFG::getBoardMode(){
  return data.boardMode;
}

/* setters */

void CFG::setMainColor(rgb color, bool autosave){
  data.mainColor  = color;
  if(autosave){
    save();
  }
}

void CFG::setBgColor(rgb color, bool autosave){
  data.bgColor  = color;
  if(autosave){
    save();
  }
}

void CFG::setNetwork(network net){
  data.client  = net;
  save(true);
}
void CFG::setNetwork(String ssid, String password){
  strncpy(data.client.ssid, ssid.c_str(), 32);
  strncpy(data.client.password, password.c_str(), 64);
  save(true);
}
