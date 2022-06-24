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
  
  this->data.client     = network {ST_SSID, ST_PASS};
  this->data.bright     = 255;
  this->data.speed      = 100;
  this->data.mainColor  = rgb {255, 255, 255};
  this->data.bgColor    = rgb {0, 0, 0};
  this->data.timeZone   = NTP_TIMEZONE_GMT;
  this->data.wifiMode   = 2;  // 1 AP, 2 ST
  this->data.boardMode  = 0;
  this->data.mynum      = MYNUM;
  this->data.setupComplete = false;
  this->data.brightSchedule.enable   = false;
  this->data.brightSchedule.from  = {0,0,0};
  this->data.brightSchedule.to  = {0,0,0};
  this->data.brightSchedule.bright = 255;
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
  if(!this->data.setupComplete){
    this->data.setupComplete = true;
    this->data.boardMode = 1;
  }
  EEPROM.begin(EEPROM_SIZE);
  saveObject(SETTINGS_ADDR, data);
  logger("CFG", "SAVED!");
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
      this->data = newData;
      logger("CFG", "LOADED!");
      return true;
    }
    logger("CFG", "NOT FOUND! LOADING DEFAULTS...");
  return false;
}

/* getters */
String CFG::msg(){
  return String(this->data.msg);
}
bool CFG::setupCmplt(){
  return this->data.setupComplete;
}
network CFG::getNetwork(){
  return this->data.client;
}
rgb CFG::getMainColor(){
  return this->data.mainColor;
}
rgb CFG::getBgColor(){
  return this->data.bgColor;
}
uint8_t CFG::getTimeZone(){
  return this->data.timeZone;
}
uint8_t CFG::getWifiMode(){
  return this->data.wifiMode;
}
uint8_t CFG::getBoardMode(){
  return this->data.boardMode;
}
uint8_t CFG::getBright(){
  return this->data.bright;
}
uint8_t CFG::getSpeed(){
  return this->data.speed;
}
schedule CFG::getSchedule(){
  return this->data.brightSchedule;
}

/* setters */
void CFG::setMsg(String msg, bool autosave){
  msg.toCharArray(this->data.msg, msg.length() + 1);
  if(autosave){
    this->save();
  }
}
void CFG::setMainColor(rgb color, bool autosave){
  this->data.mainColor  = color;
  if(autosave){
    this->save();
  }
}


void CFG::setSchedule(schedule sch, bool autosave){
  this->data.brightSchedule = sch;
  if(autosave){
    this->save();
  }
}
void CFG::setSchedule(bool enable, bool autosave){
  this->data.brightSchedule.enable = enable;
  if(autosave){
    this->save();
  }
}

void CFG::setBgColor(rgb color, bool autosave){
  this->data.bgColor  = color;
  if(autosave){
    this->save();
  }
}
void CFG::setBoardMode(uint8_t mod, bool autosave){
  this->data.boardMode  = mod;
  if(autosave){
    this->save();
  }
}
void CFG::setBright(uint8_t b, bool autosave){
  this->data.bright = b;
  if(autosave){
    this->save();
  }
}
void CFG::setSpeed(uint8_t s, bool autosave){
  this->data.speed = s;
  if(autosave){
    this->save();
  }
}
void CFG::setTimeZone(uint8_t tz, bool autosave){
  this->data.timeZone = tz;
  if(autosave){
    this->save();
  }
}

void CFG::setNetwork(network net){
  this->data.client  = net;
  this->save(true);
}
void CFG::setNetwork(String ssid, String password){
  strncpy(data.client.ssid, ssid.c_str(), 32);
  strncpy(data.client.password, password.c_str(), 64);
  this->save(true);
}
