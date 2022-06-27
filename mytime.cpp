#include "mytime.h"
#include "kalendar.h"
MYTIME::MYTIME() {}

MYTIME::~MYTIME() {}

void MYTIME::setup(){
  Wire.begin();
  Wire.setClock(100000);
  uint8_t rtcCheck = 255;
  this->i2cList();
  while(rtcCheck > 0 && !this->RTC.begin()){
    rtcCheck--;
  }
  if(rtcCheck > 0){
    this->RTC.setSquareWave(SquareWaveDisable);
    this->rtcGetDateTime();
    this->rtcReady = true;
    logger("RTC", "OK: " + this->getDateTimeStr());
  }
  else
  {
    logger("RTC", "ERROR: NOT FOUND!");
    this->d = {2000, 01, 01};
    this->t = {0, 0, 0};
  }
  this->tz = cfg.getTimeZone();
  this->timeClient.setTimeOffset(this->tz * 3600);
  logger("SVATEK", this->getSvatek());
}
void MYTIME::update(){
  if(!this->rtcReady && (currentTime - this->updateAt >= 10))
    {
      this->addMillis(currentTime - this->updateAt);
      this->updateAt = currentTime;     
    }
  else if(this->rtcReady && (currentTime - this->updateAt >= 1000))
    {
      this->rtcGetDateTime();
      this->updateAt = currentTime;
    }
}
void MYTIME::i2cList(){
    byte error, address;
    int nDevices;
    nDevices = 0;
    for (address = 1; address < 127; address++ )  {
      Wire.beginTransmission(address);
      error = Wire.endTransmission();
      if (error == 0) {
        prnt(String(F("[I2C]: FOUND: 0x")));
        if (address < 16)
          prnt("0");
        prntln(String(address, HEX));
        nDevices++;
      } else if (error == 4) {
        prntln(String(F("[I2C]: ERROR: 0x")));
        if (address < 16)
          prnt("0");
        prntln(String(address, HEX));
      }
    } //for loop
    if (nDevices == 0) {
      prntln(String(F("[I2C]: NO DEVICES FOUND!")));
    }
}

bool MYTIME::rtcGetDateTime(){
  datetimeformat dt;
  uint8_t wd;
  bool done = this->RTC.getDateTime(&dt.t.h, &dt.t.m, &dt.t.s, &dt.d.d, &dt.d.m, &dt.d.y, &wd);
  if(done){
    this->setDateTime(dt,false);
  }
  return done;
}
bool MYTIME::rtcSetDateTime(datetimeformat dt){
  return this->RTC.setDateTime(this->t.h, this->t.m, this->t.s, this->d.d, this->d.m, this->d.y, this->getDow(d, false));
}

bool MYTIME::hasRTC(){
  return this->rtcReady;
}


bool MYTIME::isBetween(timeformat from, timeformat to){
  uint32_t _c = this->t.s + (this->t.m * 60) + (this->t.h * 3600);
  uint32_t _f = from.s + (from.m * 60) + (from.h * 3600);
  uint32_t _t = to.s + (to.m * 60) + (to.h * 3600);

  return  (_f <= _c && _c <= _t) || (_t < _f && (_c <= _t || _c >= _f));
}

uint8_t MYTIME::getTZ(){
  return this->tz;
}
void MYTIME::setTZ(uint8_t h){
  this->tz = h;
  this->timeClient.setTimeOffset(this->tz * 3600);
}
bool MYTIME::GetNtpTime(){
  return this->GetNtpTime(!this->isDST() ? (this->tz - 1) : this->tz);
}
bool MYTIME::GetNtpTime(uint8_t ntz) {
  logger("NTP", "BEGIN");
    uint8_t i = 0;
    if(ntz < 200){
      this->timeClient.setTimeOffset(ntz * 3600);
    }
    while (!timeClient.update() && i < 3) {
      timeClient.forceUpdate();
      i++;
    }
    if (i == 3) {
      logger("NTP", "ERROR");
      return false;
    }
    else
    {
      logger("NTP", "OK: " + timeClient.getFormattedDate());
      this->setDateTime(timeClient.getFormattedDate(), true);
      return true;
    }
}

void MYTIME::addMillis(uint32_t ms){
  uint16_t rest = ms % 1000;
  uint32_t secs = (ms - rest) / 1000;
  this->t.ms += rest;
  if(this->t.ms >= 1000){
    this->addSec();
    this->t.ms -= 1000;
  }
  while(secs){
    this->addSec();
    secs--;
  }
}

void MYTIME::addSec(){
  this->t.s++;
  if(this->t.s > 59)
    {
      this->t.s = 0;
      this->addMinute();
    }
}
void MYTIME::addMinute(){
  this->t.m++;
  if(this->t.m > 59){
    this->t.m = 0;
    this->addHour();
  }
}
void MYTIME::addHour(){
  this->t.h++;
  if(this->t.h > 23){
    this->t.h = 0;
    this->addDay();
  }
}
void MYTIME::addDay(){
  uint8_t monDays[13] = {0, 31, !(this->d.y % 4) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  this->d.d++;
  if(this->d.d > monDays[d.m])
    {
      this->d.d = 1;
      this->addMonth();
    }
}
void MYTIME::addMonth(){
  this->d.m++;
  if(this->d.m > 12){
    this->d.m = 0;
    this->addYear();
  }
}
void MYTIME::addYear(){
  this->d.y++;
}
// reaguje jen na den, upravit ještě 2-3 hodinu
bool MYTIME::isDST(){ 
  if (this->d.m < 3 || this->d.m > 11) { 
    return false; 
  }
  if (this->d.m > 3 && this->d.m < 11) { 
    return true; 
  }
  int previousSunday = this->d.d - this->getDow(false);

  return this->d.m == 3 ? previousSunday >= 8 : previousSunday <= 0;
  
}

datetimeformat MYTIME::DSTdateTime(bool use){
  datetimeformat edit = {this->d, this->t};
  if(!use || !this->isDST()){
    return edit;
  }
  if(edit.t.h > 0){
    edit.t.h--;
  }else{
    edit.t.h = 23;
    if(edit.d.d > 1){
      edit.d.d--;
    }else{
      if(edit.d.m > 1){
        edit.d.m--;
      }else{
        edit.d.m = 12;
        edit.d.y--;
      }
      uint8_t monDays[13] = {0, 31, !(edit.d.y % 4) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      edit.d.d = monDays[edit.d.m];
    }
  }
  return edit;
}

String MYTIME::strNum(uint16_t num){
  return num > 9 ? String(num) : String("0") + String(num);
}
void MYTIME::setTime(timeformat t, bool toRTC){
  this->t = t;
  if(toRTC && this->rtcReady)
    {
      this->rtcSetDateTime(datetimeformat {this->d, t});
    }
  
}
void MYTIME::setTime(String t, bool toRTC){
  this->setTime(
    timeformat {
      t.substring(0, 2).toInt(),
      t.substring(3, 5).toInt(),
      t.substring(6, 8).toInt()
    },
    toRTC
  );
}

void MYTIME::setDate(dateformat d, bool toRTC){
  this->d = d;
  if(toRTC && this->rtcReady)
    {
      this->rtcSetDateTime(datetimeformat {d, this->t});
    }
}
void MYTIME::setDate(String d, bool toRTC){
  this->setDate(
    dateformat {
      d.substring(0, 4).toInt(),
      d.substring(5, 7).toInt(),
      d.substring(8, 10).toInt(),
    },
    toRTC
  );
}

void MYTIME::setDateTime(datetimeformat d, bool toRTC){
  this->setDate(d.d, toRTC);
  this->setTime(d.t, toRTC);
}
void MYTIME::setDateTime(String d, bool toRTC){
  this->setDateTime(
    datetimeformat {
      dateformat {
        d.substring(0, 4).toInt(),
        d.substring(5, 7).toInt(),
        d.substring(8, 10).toInt(),
      },
      timeformat {
        d.substring(11, 13).toInt(),
        d.substring(14, 16).toInt(),
        d.substring(17, 19).toInt()
      }
    },
    toRTC
  );
}

timeformat MYTIME::getTime(){
  return this->DSTdateTime().t;
}
timeformat MYTIME::getTime(String t){
  return timeformat {
    t.substring(0, 2).toInt(),
    t.substring(3, 5).toInt(),
    t.substring(6, 8).toInt()
  };
}
dateformat MYTIME::getDate(){
  return this->DSTdateTime().d;
}
datetimeformat MYTIME::getDateTime(){
  return this->DSTdateTime();
}
String MYTIME::getSvatek(){
  switch(this->getDate().m)
    {
      case 1:  return str(svatky_1 [this->getDate().d - 1]); break;
      case 2:  return str(svatky_2 [this->getDate().d - 1]); break;
      case 3:  return str(svatky_3 [this->getDate().d - 1]); break;
      case 4:  return str(svatky_4 [this->getDate().d - 1]); break;
      case 5:  return str(svatky_5 [this->getDate().d - 1]); break;
      case 6:  return str(svatky_6 [this->getDate().d - 1]); break;
      case 7:  return str(svatky_7 [this->getDate().d - 1]); break;
      case 8:  return str(svatky_8 [this->getDate().d - 1]); break;
      case 9:  return str(svatky_9 [this->getDate().d - 1]); break;
      case 10: return str(svatky_10[this->getDate().d - 1]); break;
      case 11: return str(svatky_11[this->getDate().d - 1]); break;
      case 12: return str(svatky_12[this->getDate().d - 1]); break;
    }
  return String("");
}
String MYTIME::getTimeStr(){
  return this->strNum(this->getTime().h) + ":" + this->strNum(this->getTime().m) + ":" + this->strNum(this->getTime().s);
}
String MYTIME::getTimeHMStr(){
  return this->strNum(this->getTime().h) + ":" + this->strNum(this->getTime().m);
}
String MYTIME::getTimeHMStrDSP(){
  return this->strNum(this->getTime().h) + ":" + this->strNum(this->getTime().m) + " " + this->strNum(this->getTime().h);
}
String MYTIME::getDateStr(){
  return String(this->getDate().y) + "-" + this->strNum(this->getDate().m) + "-" + this->strNum(this->getDate().d);
}
String MYTIME::getDateTimeStr(){
  return this->getDateStr() + " " + this->getTimeStr();
}

uint8_t MYTIME::getDow(dateformat dd, bool startmonday){
  static uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  static uint8_t mo[] = {6, 0, 1, 2, 3, 4, 5};
  static uint16_t yr = dd.m < 3 ? dd.y - 1 : dd.y;
  
  uint8_t day_ = ((yr + dd.y / 4 - yr / 100 + yr / 400 + t[dd.m - 1] + dd.d) % 7);
  if(!startmonday){
    return day_;
  }else{
    return mo[day_];
  }
}
uint8_t MYTIME::getDow(bool startmonday){
  return this->getDow(this->d, startmonday);
}
String MYTIME::getDowStr(bool short_){
  return short_ ? str(dow_short[this->getDow(true)]) : str(dow_long[this->getDow(true)]);
}
