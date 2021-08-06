#ifndef cfg_h
#define cfg_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}
#include <EEPROM.h>

#ifndef NTP_TIMEZONE_GMT
  #define NTP_TIMEZONE_GMT 0
#endif;
#ifndef AP_SSID
  #define AP_SSID "Chytré hodiny"
#endif;
#ifndef AP_PWD
  #define AP_PWD ""
#endif;
#ifndef HOSTNAME
  #define HOSTNAME "chytrehodiny"
#endif;

/*
struct cfgObject{
    network   client;
    rgb       mainColor;
    rgb bgColor
    uint8_t   timeZone;
    uint8_t   wifiMode;
    uint8_t   boardMode;
    String    apSSID;
    String    apPWD;
    String    hostname;
  };
*/

#include "A_config.h"
extern char* s2ch(String command);
extern void logger(String module, String message);

class CFG {
    public:
      void setup();
      void defaults();
      /* setters */
      void setNetwork(network net);
      void setNetwork(String ssid, String password);
      void setMainColor(rgb color, bool autosave = true);
      void setBgColor(rgb color, bool autosave = true);
      void setTimeZone(byte tz, bool autosave = true);
      void setWifiMode(byte mod, bool autosave = true);
      void setBoardMode(byte mod, bool autosave = true);
      void save(bool reboot = false);
      void FactoryReset();
      /* getters */
      bool setupCmplt();
      bool load();
      network getNetwork();
      rgb getMainColor();
      byte getTimeZone();
      byte getWifiMode();
      byte getBoardMode();
            
      
      CFG();
      ~CFG();
    private:
      /* EEPROM */
      template<typename T>
        static void saveObject(const int address, const T& t) {
            EEPROM.put(address, t);

            EEPROM.commit();
        }

        template<typename T>
        static void getObject(const int address, const T& t) {
            EEPROM.get(address, t);
        }
      cfgObject data;
 };
 #endif