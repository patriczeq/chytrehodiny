#include "neopixel.h"
#include "progmem_data.h"
NeoPixel::NeoPixel() {}

NeoPixel::~NeoPixel() {}

void NeoPixel::setup(){
  logger("BOARD", "Pripravuji promenne");
  setColor(cfg.getMainColor());
  setBgColor(BARVY::CERNA);
  for(uint8_t i = 0; i < NUMPIXELS; i++)
    {
      board[i].r = mainColor.r;
      board[i].g = mainColor.g;
      board[i].b = mainColor.b;
    }
  setMode(cfg.getBoardMode());
}
void NeoPixel::setMode(uint8_t m)
  {
    mode = m;
  }
void NeoPixel::draw(){
  if(enabled)
    {
     pixels.clear();
      for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, board[i].r, board[i].g, board[i].b);
      }
      pixels.show(); 
    }
}

/* helpers */

rgb NeoPixel::hexRgb(String hex){
  hex.replace(" ","");
  hex.replace("#","");
  hex.toUpperCase();
  rgb h = {0, 0, 0};
  if(hex.length() == 6) // parsing hex color
    {
      long rgb = strtol(hex.c_str(), 0, 16);
      h.r = (rgb >> 16) & 0xFF;
      h.g = (rgb >> 8) & 0xFF;
      h.b = rgb & 0xFF;
    }
  return h;
}

/* color functions */
void NeoPixel::setColor(uint8_t r, uint8_t g, uint8_t b){
  mainColor.r = r;
  mainColor.g = g;
  mainColor.b = b;
  cfg.setMainColor(mainColor, false);
  logger("BOARD", "Zmena vychozi barvy na rgb(" + String(r) + "," + String(g) + "," + String(b) + ")");
}
void NeoPixel::setColor(uint8_t predef){
  switch(predef)
    {
      case BARVY::CERNA:    setColor(  0,  0,  0);break;
      case BARVY::CERVENA:  setColor(255,  0,  0);break;
      case BARVY::ZELENA:   setColor(  0,255,  0);break;
      case BARVY::MODRA:    setColor(  0,  0,255);break;
      case BARVY::ZLUTA:    setColor(255,255,255);break;
      case BARVY::FIALOVA:  setColor(255,  0,255);break;
      case BARVY::AZUROVA:  setColor(  0,255,255);break;
      case BARVY::BILA:     setColor(255,255,255);break;
    }
}
void NeoPixel::setColor(rgb c){
  setColor(c.r, c.g, c.b);
}
void NeoPixel::setColor(String hex){
  mainColor = hexRgb(hex);
}

void NeoPixel::setBgColor(uint8_t r, uint8_t g, uint8_t b){
  backColor.r = r;
  backColor.g = g;
  backColor.b = b;
  cfg.setBgColor(backColor, false);
  logger("BOARD", "Zmena barvy pozadi na (" + String(r) + "," + String(g) + "," + String(b) + ")");
}
void NeoPixel::setBgColor(uint8_t predef){
  switch(predef)
    {
      case BARVY::CERNA:    setBgColor(  0,  0,  0);break;
      case BARVY::CERVENA:  setBgColor(255,  0,  0);break;
      case BARVY::ZELENA:   setBgColor(  0,255,  0);break;
      case BARVY::MODRA:    setBgColor(  0,  0,255);break;
      case BARVY::ZLUTA:    setBgColor(255,255,255);break;
      case BARVY::FIALOVA:  setBgColor(255,  0,255);break;
      case BARVY::AZUROVA:  setBgColor(  0,255,255);break;
      case BARVY::BILA:     setBgColor(255,255,255);break;
    }
}
void NeoPixel::setBgColor(rgb c){
  setBgColor(c.r, c.g, c.b);
}
void NeoPixel::setBgColor(String hex){
  backColor = hexRgb(hex);
}



void NeoPixel::setPxColor(uint8_t px, uint8_t predef){
  switch(predef)
    {
      case BARVY::CERNA:    setPxColor(px,  0,  0,  0);break;
      case BARVY::CERVENA:  setPxColor(px,255,  0,  0);break;
      case BARVY::ZELENA:   setPxColor(px,  0,255,  0);break;
      case BARVY::MODRA:    setPxColor(px,  0,  0,255);break;
      case BARVY::ZLUTA:    setPxColor(px,255,255,255);break;
      case BARVY::FIALOVA:  setPxColor(px,255,  0,255);break;
      case BARVY::AZUROVA:  setPxColor(px,  0,255,255);break;
      case BARVY::BILA:     setPxColor(px,255,255,255);break;
    }
}
void NeoPixel::setPxColor(uint8_t px, uint8_t r, uint8_t g, uint8_t b){
  board[px].r = r;
  board[px].g = g;
  board[px].b = b;
}
void NeoPixel::setPxColor(uint8_t px, rgb c){
  board[px] = c;
}
void NeoPixel::setPxColor(uint8_t px, String hex){
  board[px] = hexRgb(hex);
}
rgb NeoPixel::getColor(){
  return mainColor;
}
rgb NeoPixel::getBgColor(){
  return backColor;
}
rgb NeoPixel::getPxColor(uint8_t px){
  return board[px];
}
void NeoPixel::offPx(uint8_t px){
  board[px] = backColor;
}
bool NeoPixel::isPxActive(uint8_t px){
  return board[px].r || board[px].g || board[px].b;
}

/* class functions */

void NeoPixel::update(){
  if(currentTime - lastUpdate >= updateAt)
    {
      switch(mode){
        case MOD::SET:
          drawSET();
          break;
        case MOD::HODINY:
          drawTime(myTime.h, myTime.m);
          break;
      }
      lastUpdate = currentTime;
    }
}
void NeoPixel::enable(){
  if(!ready){
    logger("BOARD", "Detekuji (" + String(NUMPIXELS) + ") ledek");
    pixels = Adafruit_NeoPixel(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
    pixels.begin();
    pixels.show();
    ready = true;
  }
  logger("BOARD", "Povoleno");
  enabled = true;
}
void NeoPixel::disable(){
  if(ready){
    pixels.clear();
    pixels.show();
  }
  logger("BOARD", "Zakazano");
  enabled = false;
}
void NeoPixel::toggle(){
  if(enabled)
    {
      disable();
    }
  else
    {
      enable();
    }
}
void NeoPixel::drawTime(uint8_t h, uint8_t m){
  uint8_t px[NUMPIXELS] = {};
    for(int i=1; i<NUMPIXELS; i++) // start at 1 (0 = wifi)
      {
        px[i] = 0;
      }
    h = h > 12 ? (h - 12) : (!h ? 12 : h);
    h = m > 55 ? (h < 12 ? h + 1 : 1) : h;
    uint8_t m_recalc = (m - (m % 10) + ( (m % 10) > 7 ? 10 : 0 )) + ((m % 10) >= 3 && (m % 10) <= 7 ? 5 : 0);
    // JE | JSOU | BUDE | BUDOU
    if(m > 55 && (h == 2 || h == 3 || h == 4))  // budou
      {
        for(int i=0; i<5; i++)
        {
          px[budou[i]] = 1;
         }
      }
    else if(m > 55)  // bude
      {
        for(int i=0; i<4; i++)
        {
          px[bude[i]] = 1;
         }
      }
    else if(h == 2 || h == 3 || h == 4) // jsou
      {
        for(int i=0; i<4; i++)
        {
          px[jsou[i]] = 1;
         }
      }
    else // je
      {
        for(int i=0; i<2; i++)
        {
          px[je[i]] = 1;
         }
      }
    // HODIN
    for(int i=0; i < pgm_read_byte(&(pole_hodin_sizes[h])); i++)
      {
        px[pgm_read_byte(&(pole_hodin[h][i]))] = 1;
      }
    // MINUT
    if(m >= 3 && m <= 55){
      uint8_t indexMinut[11] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
      for(int i=0; i<11; i++)
        {
          if(m_recalc == indexMinut[i])
            {
              for(int im=0; im < pgm_read_byte(&(pole_minut_sizes[i])); im++)
              {
                px[pgm_read_byte(&(pole_minut[i][im]))] = 1;
              }
            }
        }
    }

    for(int i=0; i < NUMPIXELS; i++)
      {
        setPxColor(i, px[i] ? mainColor : backColor);
      }
}


void NeoPixel::drawSET(){
  for(int i=0; i < NUMPIXELS; i++)
    {
      setPxColor(i, backColor);
    }
  for(int i=0; i<3; i++)
  {
    setPxColor(pgm_read_byte(&(w_set[i])), mainColor);
   }
}

void NeoPixel::setUpdate(uint32_t u){
  updateAt = u;
}

void NeoPixel::debug(){
  prntln("");
  prntln("Vizualizace:");
  prntln("||=============>"+ myTimeStr() + "<==========||");
  prnt(F("||"));
  for(int i=1; i < (NUMPIXELS + 1); i++)
    {
      prnt(isPxActive(i-1) ? "[" + String(char(pgm_read_byte(&paleta[i-1]))) + "]" : "   ");
      if(!(i % 11)){
        prntln(F("||"));
        prnt(F("||"));
      }
    }
  prntln(F("=================================||"));
  prntln("Aktivní pixely:");
  for(int i=1; i < (NUMPIXELS + 1); i++)
    {
      if(isPxActive(i-1))
        {
          prnt(String(i) + " ");
        }
    }
}
