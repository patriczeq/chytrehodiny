#include "wifi.h"
#include "progmem_wifi.h"
WIFI::WIFI() {}

WIFI::~WIFI() {}

HTTPClient http;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, tz * 3600);
ESP8266WebServer server(80);
DNSServer dns;

void WIFI::setup() {
  client = cfg.getNetwork();

  strncpy(ap.ssid, AP_SSID, 32);
  strncpy(ap.password, AP_PWD, 64);
  
  
  pinMode(LED_PIN, OUTPUT);

  logger("WiFi", "Ulozena sit: \"" + getCliSSID() + "\"");
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

void WIFI::pbc() {
  if (!ap_pbc)
  {
    ap_pbc_start = currentTime;
    setMode(MODES::AP);
    ap_pbc = true;
    logger("WiFi-PBC", "Vytvarim AP na " + String(ap_pbc_timeout / 60000) + "min");
  }
  else
  {
    ap_pbc_start -= ap_pbc_timeout;
  }
}
String WIFI::scanJSON() {
  String buff = "[\n";
  for (uint8_t net = 0; net < scanned; net++)
  {
    buff += "[\"" + WiFi.SSID(net) + "\", " + String(WiFi.RSSI(net)) + ", " + (WiFi.encryptionType(net) == ENC_TYPE_NONE ? "false" : "true") + "]" + (net != scanned - 1 ? "," : "") + "\n";
  }
  return buff + "]";
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
      digitalWrite(LED_PIN, LOW);
      startAP();
      break;
    case MODES::ST:
      digitalWrite(LED_PIN, HIGH);
      startClient();
      break;
    case MODES::OFF:
      digitalWrite(LED_PIN, HIGH);
      WiFi.mode(WIFI_OFF);
      break;
  }
  ap_pbc = false;
}
void WIFI::startAP() {
  mode = MODES::AP;
  WiFi.mode(WIFI_AP);
  IPAddress ip(192, 168, 4, 1);
  IPAddress netmask(255, 255, 255, 0);
  logger("WiFi", "Spoustim sit \"" + String(AP_SSID) + "\"");
  WiFi.softAPConfig(ip, ip, netmask);
  connected = WiFi.softAP(ap.ssid, ap.password);
  dns.setErrorReplyCode(DNSReplyCode::NoError);
  dns.start(53, "*", ip);
}
void WIFI::handleIndex() {
  server.send(200, str(W_HTML).c_str(), file_index_html);
}
String WIFI::cfgJSON() {
  String  json = "{\n";
  json += "\"wifi_mode\": " + String(mode) + ",\n";
  json += "\"sta_ssid\": \"" + getCliSSID() + "\",\n";
  json += "\"time\": [" + String(myTime.h) + "," + String(myTime.m) + "," + String(myTime.s) + "],\n";
  json += "\"timezone\": " + String(tz) + ",\n";
  json += "\"main_color\": [" + String(neopixel.getColor().r) + "," + String(neopixel.getColor().g) + "," + String(neopixel.getColor().b) + "],\n";
  json += "\"bg_color\": [" + String(neopixel.getBgColor().r) + "," + String(neopixel.getBgColor().g) + "," + String(neopixel.getBgColor().b) + "],\n";
  json += "\"board_mode\": " + String(neopixel.mode) + "\n}";
  return json;
}
void WIFI::startServer() {
  /* FIXED */
  server.on("/", [this]() {
    handleIndex();
  });
  server.on("/style.css", [this]() {
    server.send(200, str(W_CSS).c_str(), file_style_css);
  });
  server.on("/clock.js", [this]() {
    server.send(200, str(W_JS).c_str(), file_clock_js);
  });
  server.onNotFound([this]() {
    handleIndex();
  });

  server.on("/cfg.json", HTTP_GET, [this]() {
    server.send(200, str(W_JSON).c_str(), cfgJSON());
  });
  server.on("/scan.json", [this]() {
    this->scanNetworks();
    server.send(200, str(W_JSON).c_str(), scanJSON());
  });
  server.on("/time.json", [this]() {
    server.send(200, str(W_JSON).c_str(), "[" + String(myTime.h) + "," + String(myTime.m) + "," + String(myTime.s) + "]");
  });
  server.on("/pixels.json", [this]() {
    String px = "{\n\"px\":[\n";
    bool first = true;
    for (uint8_t p = 0; p < NUMPIXELS; p++)
    {
      if (neopixel.isPxActive(p))
      {
        rgb pxColor = neopixel.getPxColor(p);
        px += String(!first ? ",\n" : "") + "[" + String(p) + "," + String(pxColor.r) + "," + String(pxColor.g) + "," + String(pxColor.b) + "]";
        first = false;
      }
    }
    px += "\n]\n}";
    server.send(200, str(W_JSON).c_str(), px);
  });

  server.on("/ntp", [this]() {
    GetNtpTime();
    server.send(200, str(W_JSON).c_str(), "{\"status\": \"ok\"}");
  });

  server.on("/apply", [this]() {
    // time
    if (server.hasArg("time_h") && server.hasArg("time_m") && server.hasArg("time_s")) {
      setMyTime(
      timeformat({
        server.arg("time_h").toInt(),
        server.arg("time_m").toInt(),
        server.arg("time_s").toInt()
      })
      );
    }
    // timezone
    if (server.hasArg("timezone")) {
      SetNtpTZ(server.arg("timezone").toInt());
    }
    // mainColor
    if (server.hasArg("color_r") && server.hasArg("color_g") && server.hasArg("color_b")) {
      neopixel.setColor(
        rgb({
          server.arg("color_r").toInt(),
          server.arg("color_g").toInt(),
          server.arg("color_b").toInt()
        })
      );
      cfg.setMainColor(
        rgb({
          server.arg("color_r").toInt(),
          server.arg("color_g").toInt(),
          server.arg("color_b").toInt()
        }), false
      );
    }
    // bgColor
    if (server.hasArg("bcolor_r") && server.hasArg("bcolor_g") && server.hasArg("bcolor_b")) {
      neopixel.setBgColor(
        rgb({
          server.arg("bcolor_r").toInt(),
          server.arg("bcolor_g").toInt(),
          server.arg("bcolor_b").toInt()
        })
      );
      cfg.setBgColor(
        rgb({
          server.arg("bcolor_r").toInt(),
          server.arg("bcolor_g").toInt(),
          server.arg("bcolor_b").toInt()
        }), false
      );
    }
    // wifi-cli
    if (server.hasArg("ssid") && server.hasArg("pwd")) {
      //network newnet = {server.arg("ssid").c_str(), server.arg("pwd").c_str()};
      logger("WiFi", "Nove SSID: " + server.arg("ssid"));
      cfg.setNetwork(server.arg("ssid"), server.arg("pwd"));
    }
    
    // factory
    if (server.hasArg("factoryReset"))
      {
        cfg.FactoryReset();
      }   
    cfg.save();
    server.send(200, str(W_JSON).c_str(), cfgJSON());
  });

  /*
      if(server.hasArg("save")){
      cfg.save();
    }
    else if (server.hasArg("set") && server.hasArg("value"))
    {
      logger("SETTER", server.arg("set") + ": " + server.arg("value"));

      if(server.arg("set") == "color")
        {
          neopixel.setColor(server.arg("value"));
        }
      else if(server.arg("set") == "tz")
        {
          SetNtpTZ(server.arg("value").toInt());
        }
      else if(server.arg("set") == "wifimode")
        {
          setMode(server.arg("value").toInt());
        }
      else if(server.arg("set") == "ssid")
        {
          client.ssid = server.arg("value");
        }
      else if(server.arg("set") == "password")
        {
          client.password = server.arg("value");
        }
    }
  */
  logger("WiFi", "Zapinam webove rozhrani");
  server.begin();
}

void WIFI::startClient(network net) {
  client = net;
  startClient();
}

void WIFI::startClient() {
  mode = MODES::ST;
  conn = false;
  logger("WiFi", "Pripojuji se k siti\"" + getCliSSID() + "\"");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);
  WiFi.hostname(HOSTNAME);
  WiFi.begin(client.ssid, client.password);
}

//time NTP sync
void WIFI::GetNtpTime() {
  if (mode == MODES::ST && conn)
  {
    logger("NTP", "Synchronizuji cas z internetu");
    uint8_t i = 0;
    while (!timeClient.update() && i < 3) {
      timeClient.forceUpdate();
      i++;
      logger("NTP", "Pokus " + i);
    }
    if (i == 3) {
      logger("NTP", "Chyba");
    }
    else
    {
      setMyTime(
      timeformat({
        timeClient.getHours(),
        timeClient.getMinutes(),
        timeClient.getSeconds()
      })
      );

      logger("NTP", "Synchronizováno");
    }
  }
}
void WIFI::SetNtpTZ(uint8_t tz_) {
  tz = tz_;
  timeClient.setTimeOffset(tz * 3600);
}

void WIFI::update() {
  if (mode == MODES::ST && !conn && WiFi.status() == WL_CONNECTED)
  {
    conn = true;
    MDNS.begin(HOSTNAME);
    logger("WiFi", "Pripojeno k siti\"" + getCliSSID() + "\", IP: " + WiFi.localIP().toString());
    logger("mDNS", "Adresa \"" + String(HOSTNAME) + "\"");
    GetNtpTime();
  }

  // pbc

  if (ap_pbc && (currentTime - ap_pbc_start) > ap_pbc_timeout)
  {
    logger("WiFi-PBC", "Vyprsel casovac");
    setMode(ap_pbc_prevstate);
  }
  dns.processNextRequest();
  server.handleClient();
}
