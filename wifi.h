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
#include <ESP8266HTTPClient.h>  // HTTP client
#include <DNSServer.h>          // DNS server
#include <ESP8266mDNS.h>        // mDNS

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

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
      uint8_t getMode();
      void toggleMode();
      void startAP();
      void startClient();
      void startClient(network net);
      void startServer();
      void handleIndex();
      String cfgJSON();
      void pbc();
      void scanNetworks();
      bool connected = false;
      void GetNtpTime();
      String getCliSSID();
      String getCliPWD();
      void updateLED();
      WIFI();
      ~WIFI();
    private:
    
      uint8_t  st_conn_try     = 0;
      bool ap_pbc           = false;
      uint32_t st_conn_start = 0;
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
        ST = 2,
        CONN = 3
      };
      enum LED_MODES{
        LED_ST = 0,
        LED_AP = 1,
        LED_CONN = 2
      };
      AsyncWebSocket socket = AsyncWebSocket("/socket");
      AsyncWebServer server = AsyncWebServer(80);
      uint32_t lastPxSent = 0;
      void sendPixels();
      void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len);
      void handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint8_t id);
      void JSONsyncCli(uint8_t nid, String str);
      void JSONsyncCli(uint8_t nid, String key, bool value);
      void JSONsyncCli(uint8_t nid, String key, int value);
      void JSONsyncCli(uint8_t nid, String key, String value);
      String JSONval(String v);
      String JSONval(int v);
      String JSONval(bool v);
      String JSONval(int v1, int v2, int v3);
      String JSONkey(String k, String v, bool comma = false);
      String JSONtree(String in, bool comma = false);
      String ARRtree(String in, bool comma = false);
      String ARRval(String v, bool precomma = false);
      String ARRval(int v, bool precomma = false);
      String ARRval(bool v, bool precomma = true);
      uint8_t ledMode = 0;
      bool ledUp = true;
      uint32_t ledTime = 0;
      uint8_t mode = cfg.getWifiMode();
 };
 #endif
