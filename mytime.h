#ifndef mytime_h
#define mytime_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "cfg.h"
#include <WiFiUdp.h>            // UDP proccessing
#include <NTPClient.h>          // NTP client
#include <RTCDS1307.h>
#include <Wire.h>
#include <ErriezDS1307.h>

extern uint32_t currentTime;
extern void logger(String module, String message);
extern void prnt(String p);
extern void prntln(String p);
extern CFG cfg;

class MYTIME {
    public:
      void setup();
      void update();
      // sets
      void setTime(timeformat t, bool toRTC = true);
      void setTime(String t, bool toRTC = true);
      void setDate(dateformat d, bool toRTC = true);
      void setDate(String d, bool toRTC = true);
      void setDateTime(datetimeformat d, bool toRTC = true);
      void setDateTime(String d, bool toRTC = true);
      void setTZ(uint8_t h);
      // gets
      timeformat getTime();
      timeformat getTime(String t);
      dateformat getDate();
      datetimeformat getDateTime();
      String getTimeStr();
      String getDateStr();
      String getDateTimeStr();
      String strNum(uint16_t num);
      uint8_t getDow(bool startmonday = true);
      uint8_t getDow(dateformat dd, bool startmonday = true);
      String svatek();
      bool GetNtpTime(uint8_t ntz);
      bool GetNtpTime();
      uint8_t getTZ();
      bool hasRTC();
      void i2cList();
      bool isBetween(timeformat from, timeformat to);
      MYTIME();
      ~MYTIME();
    private:
      uint8_t dow(uint16_t y, uint8_t m, uint8_t d, bool startmonday = false);
      // dt add
      void addMillis(uint32_t ms = 1);
      void addSec();
      void addMinute();
      void addHour();
      void addDay();
      void addMonth();
      void addYear();
      // cli
      bool rtcGetDateTime();
      bool rtcSetDateTime(datetimeformat dt);
      WiFiUDP     ntpUDP;
      uint8_t     tz         = NTP_TIMEZONE_GMT;
      uint32_t    updateAt   = 0;
      bool        rtcReady   = false;
      ErriezDS1307 RTC;
      NTPClient   timeClient = NTPClient(this->ntpUDP, this->tz);
      timeformat t;
      dateformat d;
 };
 #endif
