#include "wifi.h"
#include "progmem_wifi.h"
#include "kalendar.h"
WIFI::WIFI() {}

WIFI::~WIFI() {}
ESP8266WiFiMulti wifiMulti;
DNSServer dns;

void WIFI::setup() {
  client = cfg.getNetwork();
  strncpy(ap.ssid, AP_SSID, 32);
  strncpy(ap.password, AP_PWD, 64);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, 1);

  
  logger("WiFi", "SAVED NETWORK: " + getCliSSID());
  if (getCliSSID() != "") {
    ap_pbc_prevstate = MODES::ST;
    setMode(ap_pbc_prevstate);
  }
  else
  {
    setMode(ap_pbc_prevstate);
  }

  this->ping.on(true, [this](const AsyncPingResponse& response) {
         if (response.answer)
         {
          neopixel.isOffline = false;
         }
         else
         {
          neopixel.isOffline = true;
         }
      return true;
    });
   this->ping.on(false, [this](const AsyncPingResponse& response) {
         if (response.answer)
         {
          neopixel.isOffline = false;
         }
         else
         {
          neopixel.isOffline = true;
         }
      return true;
    });
  startServer();
}

      
void WIFI::updateLED(){
  if(ledMode == LED_MODES::LED_AP){
    ledUp = !ledUp;
  }
  else if(ledMode == LED_MODES::LED_CONN){
    ledUp = true;
  }
  else{
    ledUp = false;
  }
  digitalWrite(LED_PIN, ledUp ? 0 : 1);
  ledTime = currentTime;
}

void WIFI::pbc() {
  if (!ap_pbc)
  {
    ap_pbc_start = currentTime;
    setMode(MODES::AP);
    ap_pbc = true;
    logger("WiFi-PBC", "TIMEOUT: " + String(ap_pbc_timeout / 60000) + "min");
  }
  else
  {
    ap_pbc_start -= ap_pbc_timeout;
  }
}

bool WIFI::checkUpdate()
  {
    WiFiClient wificlient;
    HTTPClient http;
    float my_version = 0.00;
    int v_diff = 0;
    bool checked = false;
    StaticJsonBuffer<100> jsonBuffer;

    logger("OTA", "Current Version: " + String(cfg.currentVersion) + " ... checking update");
    
    if (http.begin(wificlient, String(UPDATE_CHECK).c_str())) {  // HTTP
      int httpCode = http.GET();
      String response = http.getString();
      
      logger("OTA", "HTTP GET: " + String(httpCode) + " " + response);

      if (httpCode > 0) {
        JsonObject& json = jsonBuffer.parseObject(response);
        if(json.success()){
          my_version = json["current"];
          logger("OTA", "Remote Version: " + String(my_version));
        }
      }
      checked = true;
    }
    else
    {
      logger("OTA", "HTTP read error!");
    }
    http.end();
    if(checked)
      {
        cfg.remoteVersion = my_version;
      }
    v_diff = uint16_t(cfg.remoteVersion * 100) - uint16_t(cfg.currentVersion * 100);
    logger("OTA", "v_diff " + String(v_diff));
    if(v_diff > 0 && (cfg.currentVersion * 100 > 0))
      {
        logger("OTA", "Preparing update...");
        neopixel.setWifi(4);
        return true;
      }

    return false;
  }


void WIFI::doUpdate(String url)
  {
    uint8_t prev_neopixel_mode = neopixel.mode;
    neopixel.setMode(199);
    delay(10);
    neopixel.update();
    neopixel.draw();
    delay(10);
    WiFiClient wificlient;
    fw_update_in_progress = true;
    logger("OTA", url);
    
    ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);
    
    t_httpUpdate_return ret = ESPhttpUpdate.update(wificlient, url);
    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.printf("HTTP_UPDATE_FAILD Error (%d): %s\n", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("HTTP_UPDATE_NO_UPDATES");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("HTTP_UPDATE_OK");
        break;
    }
    neopixel.setMode(prev_neopixel_mode);
    this->startServer();
  }

void WIFI::scanNetworks() {
  logger("WiFi-SCAN", "...");
  scanned = WiFi.scanNetworks();
  for (uint8_t net = 0; net < scanned; net++)
  {
    logger("WiFi-SCAN", String(WiFi.encryptionType(net) == ENC_TYPE_NONE ? "" : "*") + WiFi.SSID(net) + ", " + String(WiFi.RSSI(net)) + "dBm");
  }
}

String WIFI::getCliSSID(){
  return client.ssid;
}
String WIFI::getCliPWD(){
  return client.password;
}

void WIFI::setMode(uint8_t mode) {
  switch (mode) {
    case MODES::AP:
      startAP();
      break;
    case MODES::ST:
      startClient();
      break;
    case MODES::OFF:
      WiFi.mode(WIFI_OFF);
      break;
  }
  neopixel.setWifi(mode);
  ap_pbc = false;
}
uint8_t WIFI::getMode(){
  return this->mode;
}
void WIFI::startAP() {
  mode = MODES::AP;
  WiFi.mode(WIFI_AP);
  IPAddress ip(192, 168, 4, 1);
  IPAddress netmask(255, 255, 255, 0);
  logger("WiFi", "AP: \"" + String(AP_SSID) + "\"");
  WiFi.softAPConfig(ip, ip, netmask);
  connected = WiFi.softAP(ap.ssid, ap.password);
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", ip);
}
String WIFI::JSONkey(String k, String v, bool comma){
  return String("\"" + k + "\": " + v) + String(comma ? ",\n" : "\n");
}
String WIFI::JSONval(String v){
  return String("\"" + v + "\"");
}
String WIFI::JSONval(int v){
  return String(v);
}
String WIFI::JSONval(float v){
  return String(v);
}
String WIFI::JSONval(bool v){
  return String(v ? "true" : "false");
}
String WIFI::JSONval(int v1, int v2, int v3){
  return this->ARRtree(
            this->ARRval(v1) +
            this->ARRval(v2, true) +
            this->ARRval(v3, true)
          );
}
String WIFI::JSONtree(String in, bool comma){
  return String("{\n") + in + String("}") + String(comma ? ",\n" : "");
}
String WIFI::ARRtree(String in, bool comma){
  return String("[" + in + "]") + String(comma ? ",\n" : "");
}
String WIFI::ARRval(String v, bool precomma){
  return String(precomma ? "," : "") + String("\"") + v + String("\"");
}
String WIFI::ARRval(int v, bool precomma){
  return  String(precomma ? "," : "") + String(v);
}
String WIFI::ARRval(bool v, bool precomma){
  return String(precomma ? "," : "") + String(v ? "true" : "false");
}

String WIFI::DayInfo(){
  String  output = mytime.getDowStr(false,true); // Pondělí
          output+= ", ";
          output+= String( mytime.getDate().d) + ". ";
          output+= mytime.getMonStr(false, true, true) + ", ";
          output+= mytime.getSvatek(true);
   return output;
}
      
String WIFI::cfgJSON() {
  return this->JSONtree(
            this->JSONkey("wifi_mode",    this->JSONval(mode), true) +
            this->JSONkey("sta_ssid",     this->JSONval(getCliSSID()), true) +
            this->JSONkey("version",      this->JSONval(cfg.currentVersion), true) +
            this->JSONkey("remote_version",     this->JSONval(float(cfg.remoteVersion)), true) +
            this->JSONkey("time",         this->JSONval(mytime.getTime().h, mytime.getTime().m, mytime.getTime().s), true) +
            this->JSONkey("date",         this->JSONval(mytime.getDate().y, mytime.getDate().m, mytime.getDate().d), true) +
            this->JSONkey("dow",          this->JSONval(mytime.getDow()), true) +
            this->JSONkey("timezone",     this->JSONval(mytime.getTZ()), true) +
            this->JSONkey("main_color",   this->JSONval(neopixel.getColor().r, neopixel.getColor().g, neopixel.getColor().b), true) +
            this->JSONkey("bg_color",     this->JSONval(neopixel.getBgColor().r, neopixel.getBgColor().g, neopixel.getBgColor().b), true) +
            this->JSONkey("speed",        this->JSONval(neopixel.getSpeed()), true) +
            this->JSONkey("bright",       this->JSONval(neopixel.getBright()), true) +
            this->JSONkey("board_mode",   this->JSONval(neopixel.mode), true) +
            this->JSONkey("redraw_mode",  this->JSONval(neopixel.rmode), true) +
            this->JSONkey("msg",          this->JSONval(cfg.msg()), true) +
            this->JSONkey("day_info",     this->JSONval(this->DayInfo()), true) +
            this->JSONkey("schedule",
              this->JSONtree(
                this->JSONkey("enabled",    this->JSONval(cfg.getSchedule().enable), true) +
                this->JSONkey("bright",     this->JSONval(cfg.getSchedule().bright), true) +
                this->JSONkey("from",       this->JSONval(cfg.getSchedule().from.h, cfg.getSchedule().from.m, cfg.getSchedule().from.s), true) +
                this->JSONkey("to",         this->JSONval(cfg.getSchedule().to.h, cfg.getSchedule().to.m, cfg.getSchedule().to.s))
              )
            )
  );
}


void WIFI::JSONsyncCli(uint8_t nid, String str){
  for(uint8_t i = 0; i < 255; i++){
    if(nid != i){
      this->socket.text(i, this->JSONtree(str));
    }
  }
}
void WIFI::JSONsyncCli(uint8_t nid, String key, String value){
  this->JSONsyncCli(nid, this->JSONkey(key, this->JSONval(value)));
}
void WIFI::JSONsyncCli(uint8_t nid, String key, bool value){
  this->JSONsyncCli(nid, this->JSONkey(key, this->JSONval(value)));
}
void WIFI::JSONsyncCli(uint8_t nid, String key, int value){
  this->JSONsyncCli(nid, this->JSONkey(key, this->JSONval(value)));
}

void WIFI::handleWebSocketMessage(void *arg, uint8_t *data, size_t len, uint8_t id) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  //uint16_t cliid = client->id();
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
   
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json = jsonBuffer.parseObject((char*)data);
    if(!json.success()){
      if (strcmp((char*)data, "getNTP") == 0) {
        bool ntpsuccess = mytime.GetNtpTime();
        if(ntpsuccess)
          {
            this->socket.textAll(this->JSONtree(this->JSONkey("time", this->JSONval(mytime.getTime().h, mytime.getTime().m, mytime.getTime().s))));
          }
        this->socket.textAll(this->JSONtree(this->JSONkey("ntp_update", this->JSONval(ntpsuccess))));
      }
      else if (strcmp((char*)data, "factoryReset") == 0) {
        cfg.FactoryReset();
      }
      else if (strcmp((char*)data, "reboot") == 0) {
        this->socket.textAll(this->JSONtree(this->JSONkey("reboot", "ok")));
        delay(2000);
        ESP.restart();
      }
      else if (strcmp((char*)data, "saveDisplay") == 0) {
        cfg.save();
        this->socket.textAll(this->JSONtree(this->JSONkey("display_apply", this->JSONval("ok"))));
      }
      else{
        this->socket.textAll(this->JSONtree(this->JSONkey("error", "\"Unknown command: " + String((char*)data)) + "\""));
      }
    }
    else  // settings json
    {
      if(json.containsKey("board_mode")){
        neopixel.setBoardMode(uint8_t(json["board_mode"]));
        cfg.setBoardMode(uint8_t(json["board_mode"]), false);
        this->JSONsyncCli(id, String("board_mode"), uint16_t(neopixel.mode));
      }
      
      if(json.containsKey("custom_bg") && json.containsKey("cust_r") && json.containsKey("cust_g") && json.containsKey("cust_b")){
        neopixel.setCustomBg(byte(json["custom_bg"]), rgb {byte(json["cust_r"]), byte(json["cust_g"]), byte(json["cust_b"])});
        
        this->JSONsyncCli(id, String("custom_bg"), true);
      }
      if(json.containsKey("time")){
        String t_time = json["time"];
        mytime.setTime(t_time, true);
        this->JSONsyncCli(id, String("time"), mytime.getTimeStr());
      }
      if(json.containsKey("msg")){
        String msg = json["msg"];
        neopixel.CharStrSet(msg);
        cfg.setMsg(msg, false);
        this->JSONsyncCli(id, String("msg"), msg);
      }
      if(json.containsKey("date")){
        String t_date = json["date"];
        mytime.setDate(t_date, true);
        this->JSONsyncCli(id, String("date"), mytime.getDateStr());
      }
      if(json.containsKey("timezone")){
        mytime.setTZ(uint8_t(json["timezone"]));
        cfg.setTimeZone(uint8_t(json["timezone"]), false);
        this->JSONsyncCli(id, String("timezone"), mytime.getTZ());
      }
      if(json.containsKey("color")){
        rgb color = neopixel.hexRgb(json["color"]);
        neopixel.setColor(color);
        cfg.setMainColor(color, false);
        this->JSONsyncCli(id, String("color"), String("#") + neopixel.RgbHex(neopixel.getColor()));
      }
      if(json.containsKey("bgcolor")){
        rgb color = neopixel.hexRgb(json["bgcolor"]);
        neopixel.setBgColor(color);
        cfg.setBgColor(color, false);
        this->JSONsyncCli(id, String("bgcolor"), String("#") + neopixel.RgbHex(neopixel.getBgColor()));
      }
      if(json.containsKey("bright")){
        if(!cfg.getSchedule().enable || !mytime.isBetween(cfg.getSchedule().from, cfg.getSchedule().to)){
          neopixel.setBright(uint8_t(json["bright"]));
        }
        cfg.setBright(uint8_t(json["bright"]), false);
        this->JSONsyncCli(id, String("bright"), uint16_t(neopixel.getBright()));
      }
      if(json.containsKey("speed")){
        neopixel.setSpeed(uint8_t(json["speed"]));
        cfg.setSpeed(uint8_t(json["speed"]), false);
        this->JSONsyncCli(id, String("speed"), neopixel.getSpeed());
      }
      if(json.containsKey("schedule_enable")){
        cfg.setSchedule(bool(json["schedule_enable"]), true);
        neopixel.setBright(cfg.getBright());
        this->JSONsyncCli(id, String("schedule_enable"), cfg.getSchedule().enable);
      }
      if(json.containsKey("schedule_from") && json.containsKey("schedule_to") && json.containsKey("schedule_bright")){
        schedule newSchedule;
        newSchedule.enable  = cfg.getSchedule().enable;
        newSchedule.from    = mytime.getTime(json["schedule_from"]);
        newSchedule.to      = mytime.getTime(json["schedule_to"]);
        newSchedule.bright  = uint8_t(json["schedule_bright"]);
        cfg.setSchedule(newSchedule, true);
      }
      
      if(json.containsKey("ssid") && json.containsKey("pwd")){
        String nssid = json["ssid"];
        String npwd  = json["pwd"];
        cfg.setNetwork(nssid, npwd);
      }
    }
    
  }
}

void WIFI::startServer() {
  /* FIXED */
  server.onNotFound([this](AsyncWebServerRequest *request){    request->send_P(200, str(W_HTML).c_str(), file_index_html);  });
  server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request){ request->send_P(200, str(W_HTML).c_str(), file_index_html); });
  server.on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){ request->send_P(200, str(W_CSS).c_str(), file_style_css);  });
  server.on("/clock.js", HTTP_GET, [this](AsyncWebServerRequest *request){    request->send_P(200, str(W_JS).c_str(), file_clock_js);  });
  server.on("/picker.js", HTTP_GET, [this](AsyncWebServerRequest *request){    request->send_P(200, str(W_JS).c_str(), file_picker_js);  });
  server.on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){    request->send_P(200, str(W_ICON).c_str(), file_favicon_ico);  });
  server.on("/favicon256.png", HTTP_GET, [this](AsyncWebServerRequest *request){    request->send_P(200, str(W_PNG).c_str(), file_favicon_png);  });
  server.on("/site.webmanifest", HTTP_GET, [this](AsyncWebServerRequest *request){    request->send_P(200, str(W_MANIFEST).c_str(), file_site_webmanifest);  });
  /* Generated */
  server.on("/cfg.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    request->send(200, str(W_JSON).c_str(), this->cfgJSON());
  });
  server.on("/scan.json", HTTP_GET, [this](AsyncWebServerRequest *request){
    WiFi.scanNetworksAsync([request, this](int numNetworks) {
      String buff;
      for(uint8_t n = 0; n < numNetworks; n++){
        buff += this->ARRtree(
                  this->ARRval(WiFi.SSID(n)) +
                  this->ARRval(WiFi.RSSI(n), true) +
                  this->ARRval(WiFi.encryptionType(n) != ENC_TYPE_NONE, true),
                  n < numNetworks - 1
                );
      }
      request->send(200, str(W_JSON).c_str(), this->ARRtree(buff));
    });
  });
  
  server.on("/connect", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (request->hasParam("ssid") && request->hasParam("pwd")) {
      cfg.setNetwork(String(request->getParam("ssid")->value()), String(request->getParam("pwd")->value()));
    }
  });
  /* Homebridge */
  server.on("/api/brightness", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (request->hasParam("set")) {
      int perc = String(request->getParam("set")->value()).toInt();
      uint8_t uint = (254/100) * perc;
      neopixel.setBright(uint);
      cfg.setBright(uint, false);
    }
    float current = neopixel.getBright();
    int currPerc = round((current/float(254)) * float(100));
    request->send(200, str(W_JSON).c_str(), String(currPerc));
  });

  server.on("/api/lights", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (request->hasParam("set")) {
      neopixel.setBright(String(request->getParam("set")->value()) == "0" ? 0 : cfg.getBright());
    }
    request->send(200, str(W_JSON).c_str(), neopixel.getBright() > 1 ? "1" : "0");
  });
  
  this->socket.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len){
    switch (type) {
      case WS_EVT_CONNECT:
        logger("WS", "#" + String(client->id()) + " CONNECTED: " + client->remoteIP().toString());
        break;
      case WS_EVT_DISCONNECT:
        logger("WS", "#" + String(client->id()) + " DISCONNECTED");
        break;
      case WS_EVT_DATA:
        this->handleWebSocketMessage(arg, data, len, uint8_t(client->id()));
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
    }
  });
  this->server.addHandler(&this->socket);
  this->server.begin();
}

void WIFI::sendPixels(){
  String px;
    bool first = true;
    for (uint8_t p = 0; p < NUMPIXELS; p++)
    {
      if (neopixel.isPxActive(p))
      {
        px += String(!first ? "x" : "") + String(p < 16 ? "0" + String(p, HEX) : String(p, HEX)) + neopixel.getPxColorHex(p);
        first = false;
      }
    }
  this->socket.textAll(this->JSONtree(this->JSONkey("px", this->JSONval(px))));
}



void WIFI::startClient(network net) {
  client = net;
  startClient();
}

void WIFI::startClient() {
  mode = MODES::ST;
  st_conn_start = currentTime;
  conn = false;
  logger("WiFi", "STARTING CLIENT MODE");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(client.ssid, client.password);
}

void WIFI::update() {

  neopixel.setWifi(mode);
  if (mode == MODES::ST && !conn && WiFi.status() != WL_CONNECTED){
    neopixel.setWifi(MODES::CONN);
    if(currentTime - st_conn_start > 1000){
      st_conn_try++;
      st_conn_start = currentTime;
    }
    if(st_conn_try >= 30){
      setMode(MODES::OFF);
    }
  }
  else if (mode == MODES::ST && !conn && WiFi.status() == WL_CONNECTED)
  {
    st_conn_try = 0;
    neopixel.setWifi(MODES::ST);
    conn = true;
    MDNS.begin(HOSTNAME);
    neopixel.strIP = WiFi.localIP().toString();
    logger("WiFi", "CONNECTED! IP: " + neopixel.strIP);
    logger("mDNS", "ADDRESS: \"" + String(HOSTNAME) + ".local\"");
    neopixel.setMode(neopixel.MOD::SHOWIP);
    // check if internet => NTP => OTA
    AsyncPing initPing;
    initPing.on(true, [this](const AsyncPingResponse& response) {
         if (response.answer)
         {
          mytime.GetNtpTime();
          neopixel.isOffline = false;
         }
         else
         {
          neopixel.isOffline = true;
         }
      return true;
    });
    if(this->checkUpdate()){
      this->doUpdate(FIRMWARE_URL);
    }
    initPing.begin(this->GoogleDNS, 1, 500);
  }
  else if(mode == MODES::ST && conn){
    MDNS.update();
  }

  // pbc

  if (ap_pbc && (currentTime - ap_pbc_start) > ap_pbc_timeout)
  {
    logger("WiFi-PBC", "TIMEOUT...");
    setMode(ap_pbc_prevstate);
  }
  // led mode
  if(mode == MODES::ST)
    {
      ledMode = !conn ? LED_MODES::LED_CONN : LED_MODES::LED_ST;
    }
  else if(mode == MODES::AP)
    {
      ledMode = LED_MODES::LED_AP;
    }
  // led
  if((currentTime - ledTime) > 500){
    updateLED();
  }
  
  dns.processNextRequest();
  //server.handleClient();
  this->socket.cleanupClients();
  if(currentTime - this->lastPxSent >= 1000 && !fw_update_in_progress){
    this->lastPxSent = currentTime;
    this->sendPixels();
  }

  if(currentTime - this->lastPingCheck >= 10000 && conn){
    this->lastPingCheck = currentTime;
    this->ping.begin(this->GoogleDNS, 1, 500);
  }

}
