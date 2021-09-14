#include "wifi.h"
#include "progmem_wifi.h"
WIFI::WIFI() {}

WIFI::~WIFI() {}

HTTPClient http;
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

      
String WIFI::cfgJSON() {
  return this->JSONtree(
            this->JSONkey("wifi_mode",    this->JSONval(mode), true) +
            this->JSONkey("sta_ssid",     this->JSONval(getCliSSID()), true) +
            this->JSONkey("time",         this->JSONval(mytime.getTime().h, mytime.getTime().m, mytime.getTime().s), true) +
            this->JSONkey("date",         this->JSONval(mytime.getDate().y, mytime.getDate().m, mytime.getDate().d), true) +
            this->JSONkey("dow",          this->JSONval(mytime.getDow()), true) +
            this->JSONkey("dayof",        this->JSONval(mytime.svatek()), true) +
            this->JSONkey("timezone",     this->JSONval(mytime.getTZ()), true) +
            this->JSONkey("main_color",   this->JSONval(neopixel.getColor().r, neopixel.getColor().g, neopixel.getColor().b), true) +
            this->JSONkey("bg_color",     this->JSONval(neopixel.getBgColor().r, neopixel.getBgColor().g, neopixel.getBgColor().b), true) +
            this->JSONkey("bright",       this->JSONval(neopixel.getBright()), true) +
            this->JSONkey("auto_bright",  this->JSONval(neopixel.auto_brightness), true) +
            this->JSONkey("board_mode",   this->JSONval(neopixel.mode), true) +
            this->JSONkey("redraw_mode",   this->JSONval(neopixel.rmode), true) +
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
        this->socket.textAll(mytime.GetNtpTime() ? this->JSONtree(this->JSONkey("time", this->JSONval(mytime.getTime().h, mytime.getTime().m, mytime.getTime().s))) : this->JSONtree(this->JSONkey("error", this->JSONval("NTP ERROR"))));
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
        this->socket.textAll(this->JSONtree(this->JSONkey("error", "Unknown command: " + String((char*)data))));
      }
    }
    else  // settings json
    {
      if(json.containsKey("board_mode")){
        neopixel.setBoardMode(uint8_t(json["board_mode"]));
        cfg.setBoardMode(uint8_t(json["board_mode"]), false);
        this->JSONsyncCli(id, String("board_mode"), uint16_t(neopixel.mode));
      }
      if(json.containsKey("redraw_mode")){
        neopixel.setRedrawMode(uint8_t(json["redraw_mode"]));
        cfg.setRedrawMode(uint8_t(json["redraw_mode"]), false);
        this->JSONsyncCli(id, String("redraw_mode"), uint16_t(neopixel.rmode));
      }
      if(json.containsKey("time")){
        String t_time = json["time"];
        mytime.setTime(t_time, true);
        this->JSONsyncCli(id, String("time"), mytime.getTimeStr());
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
      if(json.containsKey("auto_bright")){
        neopixel.useSensor(bool(json["auto_bright"]));
        this->JSONsyncCli(id, String("auto_bright"), neopixel.auto_brightness);
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
                );//String(n > 0 ? "," : "") + "[\"" + WiFi.SSID(n) + "\", " + String(WiFi.RSSI(n)) + ", " + (WiFi.encryptionType(n) == ENC_TYPE_NONE ? "false" : "true") + "]";
      }
      request->send(200, str(W_JSON).c_str(), this->ARRtree(buff));
    });
  });
  
  server.on("/connect", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (request->hasParam("ssid") && request->hasParam("pwd")) {
      cfg.setNetwork(String(request->getParam("ssid")->value()), String(request->getParam("pwd")->value()));
    }
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
    logger("WiFi", "CONNECTED! IP: " + WiFi.localIP().toString());
    logger("mDNS", "ADDRESS: \"" + String(HOSTNAME) + ".local\"");
    // force NTP if RTC not found...
    if(!mytime.hasRTC()){
      mytime.GetNtpTime();
    }
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
  if(currentTime - this->lastPxSent >= 1000){
    this->lastPxSent = currentTime;
    this->sendPixels();
  }
}
