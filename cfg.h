#ifndef cfg_h
#define cfg_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}
#include <EEPROM.h>
#include "A_config.h"

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
      void setTimeZone(uint8_t tz, bool autosave = true);
      void setWifiMode(uint8_t mod, bool autosave = true);
      void setBoardMode(uint8_t mod, bool autosave = true);
      void setBright(uint8_t b, bool autosave = true);
      void setSpeed(uint8_t s, bool autosave = true);
      void save(bool reboot = false);
      void FactoryReset();
      void setSchedule(schedule sch, bool autosave = true);
      void setSchedule(bool enable, bool autosave = true);
      void setMsg(String msg, bool autosave = true);
      void setDST(bool use, bool autosave = true);
      void setCustomBG(rgb pixels[NUMPIXELS], bool autosave = true);
      /* getters */
      bool setupCmplt();
      bool load();
      network getNetwork();
      rgb getMainColor();
      rgb getBgColor();
      uint8_t getBright();
      uint8_t getSpeed();
      uint8_t getTimeZone();
      uint8_t getWifiMode();
      uint8_t getBoardMode();
      schedule getSchedule();
      float remoteVersion = MYVERSION;
      const float currentVersion = MYVERSION;
      String msg();
      void getCustomBG(rgb * ar);
      bool getDST();
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
