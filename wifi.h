#ifndef wifi_h
#define wifi_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "mytime.h"
#include "cfg.h"
#include "neopixel.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // HTTP server
#include <ESP8266HTTPClient.h>  // HTTP client
#include <DNSServer.h>          // DNS server
#include <ESP8266mDNS.h>        // mDNS


extern uint32_t currentTime;
extern void prnt(String p);
extern void prntln(String p);
extern void logger(String module, String message);
extern String str(const char* ptr);
extern network s2net(String ssid, String password);

extern NeoPixel    neopixel;
extern CFG    cfg;
extern MYTIME    mytime;

class WIFI {
    public:
      void setup();
      void update();
      void setMode(uint8_t mode);
      void toggleMode();
      void startAP();
      void startClient();
      void startClient(network net);
      void startServer();
      void handleIndex();
      String scanJSON();
      String cfgJSON();
      void pbc();
      void scanNetworks();
      bool connected = false;
      void GetNtpTime();
      void SetNtpTZ(uint8_t tz_);
      String getCliSSID();
      String getCliPWD();
      void updateLED();
      WIFI();
      ~WIFI();
    private:
      bool ap_pbc           = false;
      uint32_t ap_pbc_timeout   = 600000;
      uint32_t ap_pbc_start  = 0;
      uint8_t ap_pbc_prevstate = MODES::OFF;
      uint8_t scanned = 0;
      bool conn = false;
      network client;
      network ap;
      enum MODES{
        OFF  = 0,
        AP  = 1,
        ST = 2
      };
      enum LED_MODES{
        LED_ST = 0,
        LED_AP = 1,
        LED_CONN = 2
      };
      byte ledMode = 0;
      bool ledUp = true;
      uint32_t ledTime = 0;
      uint8_t mode = cfg.getWifiMode();
 };
 #endif
