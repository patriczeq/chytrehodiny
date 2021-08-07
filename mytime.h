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
extern uint32_t currentTime;
extern void logger(String module, String message);
extern CFG cfg;
class MYTIME {
    public:
      void setup(datetimeformat d = {{1970, 1, 1}, {0, 0, 0}});
      void update();
      void setTime(timeformat t);
      void setDate(dateformat d);
      void setDateTime(datetimeformat d);
      void setDateTime(String d);
      
      timeformat getTime();
      dateformat getDate();
      datetimeformat getDateTime();
      String getTimeStr();
      String getDateStr();
      String getDateTimeStr();
      String strNum(uint16_t num);
      uint8_t getDow(bool startmonday = true);
      uint8_t getDow(dateformat dd, bool startmonday = true);
      String svatek();
      void GetNtpTime(byte ntz = 200);
      void SetNtpTZ();
      bool isDST();
      uint8_t getTZ();
      void setTZ(uint8_t h);
      MYTIME();
      ~MYTIME();
    private:
      WiFiUDP ntpUDP;
      NTPClient timeClient = NTPClient(this->ntpUDP, 0);
      uint8_t dow(uint16_t y, uint8_t m, uint8_t d, bool startmonday = false);
      void addSec();
      void addDay();
      datetimeformat getDateTimeDST();
      dateformat addDay(dateformat d);
      bool DST = false;
      uint8_t tz = NTP_TIMEZONE_GMT;
      uint32_t   updateAt = 0;
      timeformat t;
      dateformat d;
 };
 #endif
