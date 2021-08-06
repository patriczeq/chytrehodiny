#ifndef wifi_h
#define wifi_h

#include "Arduino.h"
extern "C" {
  #include "user_interface.h"
}

#include "A_config.h"
#include "cfg.h"
#include "neopixel.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>   // HTTP server
#include <ESP8266HTTPClient.h>  // HTTP client
#include <DNSServer.h>          // DNS server
#include <WiFiUdp.h>            // UDP proccessing
#include <NTPClient.h>          // NTP client
#include <ESP8266mDNS.h>        // mDNS


extern uint32_t currentTime;
extern timeformat  myTime;
extern void prnt(String p);
extern void prntln(String p);
extern void logger(String module, String message);
extern String str(const char* ptr);
extern uint8_t tz;
extern void setMyTime(uint8_t h, uint8_t m, uint8_t s);
extern void setMyTime(timeformat t);
extern network s2net(String ssid, String password);

extern NeoPixel    neopixel;
extern CFG    cfg;

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
      uint8_t mode = cfg.getWifiMode();
 };
 #endif
