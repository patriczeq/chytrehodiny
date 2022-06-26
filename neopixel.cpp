#include "neopixel.h"
#include "progmem_data.h"
NeoPixel::NeoPixel() {}

NeoPixel::~NeoPixel() {}

void NeoPixel::setup(){
  this->enable();
  this->setColor(cfg.getMainColor());
  this->setBgColor(cfg.getBgColor());
  this->setBright(cfg.getBright());
  this->setSpeed(cfg.getSpeed());
  for(uint8_t i = 0; i < NUMPIXELS; i++)
    {
      this->board[i] = this->mainColor;
    }
  this->ClearLED_MATRIX();
  logger("PIXELS", "READY!");
  this->setMode(cfg.getBoardMode());
  this->CharStrSet(cfg.msg());
}


void NeoPixel::CharStrSetTime(){
  this->CharPxLen = 35;
  this->CharMSG = mytime.getTimeHMStrDSP();
}

void NeoPixel::CharStrSet(String msg)
  { 
      this->CharMSG = msg;
      uint8_t len = this->CharMSG.length() + 1;
      char CharBuffer[len];
      this->CharMSG.toCharArray(CharBuffer, len);
      //this->CharSwing = 0;
      this->CharPxLen = (len * 6);
      if(len == 2){
        this->CharPxLen = 6;
        this->CharShift = 4;
      }else if(len == 3){
        this->CharPxLen = 11;
      }else if(len > 2){
        this->CharMSG += " " + String(CharBuffer[0]) + String(CharBuffer[1]);
        this->CharPxLen -= 1;
      }
  }
void NeoPixel::CharDraw()
  {
      uint8_t len = this->CharMSG.length() + 1;
      char CharBuffer[len];
      this->CharMSG.toCharArray(CharBuffer, len);
      this->CharShift = 0;
      for (int i = 0; i < len; i++)
        {
          this->CharAppend(CharBuffer[i]);
          this->CharShift += 6;
        }
  }

void NeoPixel::CharAppend(uint8_t b){
  this->CharAppend(char(b));
}
void NeoPixel::CharAppend(char s)
  {
     for(byte c = 0; c < 5; c++)
      {
        for(byte r = 0; r < 8; r++)
          {
            int Xpos = (this->CharShift + c) - this->CharSwing;
            int ASCII_STARTPOS = (int(s) * 5) + c;
                //ASCII_STARTPOS = ASCII_STARTPOS > 256 ? 0 : ASCII_STARTPOS;
            if(Xpos >= 0 && Xpos < PX_COLS && bitRead(pgm_read_byte(&(ascii5x7[ASCII_STARTPOS])), r)) // test - ASCII codes
              {
                this->LED_MATRIX[Xpos][2 + r] = true;
              }
          }
      }
  }

void NeoPixel::ClearLED_MATRIX()
  {
    for(uint8_t i = 0; i < NUMPIXELS; i++)
    {
      this->board[i] = rgb{0,0,0};
    }
    
    for(byte c = 0; c < PX_COLS; c++)
    {
      for(byte r = 0; r < PX_ROWS; r++)
        {
          this->LED_MATRIX[c][r] = false;
        }
    }
  }

void NeoPixel::drawMSG(bool showTime)
  {
    this->ClearLED_MATRIX();
    this->CharDraw();
    if(this->CharSwing < this->CharPxLen && this->CharPxLen > 11){
      this->CharSwing++;
    }
    else{
      this->CharSwing = 0;
    }
      
    for(byte c = 0; c < PX_COLS; c++)
    {
      for(byte r = 0; r < PX_ROWS; r++)
        {
          if(showTime && LED_MATRIX[c][r])
            {
              this->setPxColor(pxcoor{c,r}, backColor);
            }
          else if(LED_MATRIX[c][r])
            {
              this->setPxColor(pxcoor{c,r}, mainColor);
            }
        }
    }
    if(showTime)
      {
        this->drawTime(true);
      }
  }
void NeoPixel::check_bright(){
  this->setBright(
      cfg.getSchedule().enable && mytime.isBetween(cfg.getSchedule().from, cfg.getSchedule().to) ? cfg.getSchedule().bright : 
        cfg.getBright()
  );
  this->pixels.setBrightness(this->bright);
}

uint16_t NeoPixel::getUpdateAt(){
  return this->updateAt;
}

void NeoPixel::setCustomBg(uint8_t px, rgb color){
  this->customBG[px] = color;
}

void NeoPixel::update(){
  // update Muxer
  const uint16_t updt = float(float(this->updateAt / 100) * (201 - this->speed));

 
  if(mode == MOD::WIFI_JEDE){
    this->setPxColor(0, {255,255,255}); // wifi
    this->setPxColor(58, {255,255,255}); // J
    this->setPxColor(59, {255,255,255}); // E
    this->setPxColor(60, {255,255,255}); // D
    this->setPxColor(61, {255,255,255}); // E
  }
  else if(!this->redraw_done && currentTime - this->redraw_loop >= updt){
    this->runRedrawEffect();
    this->redraw_loop = currentTime;
  }
  else if(currentTime - this->lastUpdate >= updt)
    {
      
      this->drawTime(true, true); // updateTimePxls
      switch(mode){
        case MOD::SET:
          this->updateAt = 250;
          this->drawSET();
          break;
        case MOD::HODINY:
          this->updateAt = 250;
          this->drawTime();
          break;
        case MOD::SECONDS:
          this->updateAt = 250;
          this->CharPxLen = 2;
          this->CharMSG = mytime.strNum(mytime.getTime().s);
          this->drawMSG(true);
          break;
        case MOD::MINUTES:
          this->updateAt = 250;
          this->CharPxLen = 2;
          this->CharMSG = mytime.strNum(mytime.getTime().m);
          this->drawMSG(true);
          break;
        case MOD::MATRIX:
        case MOD::MATRIX_V2:
          this->updateAt = 100;
          this->drawMatrix(mode == MOD::MATRIX_V2);
          break;
        case MOD::OHEN:
        case MOD::OHEN_V2:
          this->updateAt = 100;
          this->drawFlame(mode == MOD::OHEN_V2);
          break;
        case MOD::SRDCE:
          this->updateAt = 1000;
          this->drawHeart();
          break;
        case MOD::SHOWIP:
          this->updateAt = 1000;
          this->drawIP();
          if(this->ipprintpos == 100)
            {
              this->setMode(cfg.getBoardMode());
            }
          break;
        case MOD::CUSTOM:
          this->updateAt = 250;
          this->drawCustom();
          break;
        case MOD::MSGBG:
        case MOD::MSG:
          this->updateAt = 250;
          this->drawMSG(mode == MOD::MSGBG);
          break;
        case MOD::DIGITIME:
          this->updateAt = 250;
          this->CharStrSetTime();
          this->drawMSG(false);
          break;
        case MOD::DOW:
          this->updateAt = 250;
          this->CharStrSet(mytime.getDowStr());
          this->drawMSG(true);
          break;
        case MOD::SVATEK:
          this->updateAt = 250;
          this->CharStrSet(mytime.getSvatek());
          this->drawMSG(true);
          break;
      }

      
      this->wifi_counter++;
      if(this->wifi_counter > 254){
        this->wifi_counter = 0;
      }
      switch(wifi_mode){
        case WIFI_MOD::OFF:
          this->setPxColor(0, BARVY::CERVENA);
          break;
        case WIFI_MOD::AP:
          this->setPxColor(0, !(this->wifi_counter % 2) ? BARVY::CERNA : BARVY::MODRA);
          break;
        case WIFI_MOD::CONN:
          this->setPxColor(0, !(this->wifi_counter % 2) ? BARVY::CERNA : BARVY::ZELENA);
          break;
        default:
          if(this->isOffline){
            this->setPxColor(0, BARVY::ZLUTA);
          }
          break;
      }
            
      this->lastUpdate = currentTime;
      this->draw();
    }
}


void NeoPixel::drawIP(){
  const String ch = this->strIP.substring(this->ipprintpos, this->ipprintpos + 1);
  this->ClearLED_MATRIX();
  
  if(ch == ".")
    {
      this->setPxColor(5, rgb{255,255,255});
    }
  else
    {
      this->singleNumberPrint(ch.toInt());
    }
  this->ipprintpos++;
  if(this->ipprintpos > this->strIP.length()){
    this->ipprintpos = 100;
  }
}

void NeoPixel::setWifi(uint8_t m){
  this->wifi_mode = m;
}
void NeoPixel::intro(){
    pixels.clear(); // Set all pixel colors to 'off'
    for(int i=0; i<PX_ROWS; i++) { // For each pixel...
      for(int c=0; c<PX_COLS; c++){
        pixels.setPixelColor((i * PX_ROWS) + c, pixels.Color(64,64,64));
        pixels.show();
        delay(5);
      }
    }
    delay(1000);
    pixels.clear();
    pixels.show();
    
}
void NeoPixel::enable(){
  if(!this->ready){
    this->pixels.begin();
    this->intro();
    this->ready = true;
  }
  this->enabled = true;
}
void NeoPixel::disable(){
  if(this->ready){
    this->pixels.clear();
    this->pixels.show();
  }
  this->enabled = false;
}
void NeoPixel::toggle(){
  if(this->enabled)
    {
      this->disable();
    }
  else
    {
      this->enable();
    }
}
void NeoPixel::setMode(uint8_t m)
  {
    this->ClearLED_MATRIX();
    this->effect_slide = 0;
    this->mode = m;
  }
void NeoPixel::setBright(uint8_t b){
  this->bright = b;
}
uint8_t NeoPixel::getBright(){
  return this->bright;
}
void NeoPixel::setSpeed(uint8_t s){
  this->speed = s;
}
uint8_t NeoPixel::getSpeed(){
  return this->speed;
}
void NeoPixel::fadePx(uint8_t px, uint8_t dec){
  rgb color   = getPxColor(px);
      color.r-= color.r - dec >= 0 ? dec : color.r;
      color.g-= color.g - dec >= 0 ? dec : color.g;
      color.b-= color.b - dec >= 0 ? dec : color.b;
  this->setPxColor(px, color);
}

map_colrow NeoPixel::col(uint8_t id){
  map_colrow r = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  for(uint8_t s = 0; s < PX_COLS; s++){
    r.px[s] = id + (s * PX_COLS);
  }
  return r;
}
map_colrow NeoPixel::row(uint8_t id){
  map_colrow r = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  for(uint8_t s = 0; s < PX_ROWS; s++){
    r.px[s] = id + (s * PX_ROWS);
  }
  return r;
}
void NeoPixel::draw(){
  if(this->enabled)
    {
      this->check_bright();            // upravit senzorem
      this->pixels.clear();
      for(int i=0; i<NUMPIXELS; i++)
        {
          this->pixels.setPixelColor(pgm_read_byte(&(matrix_map[i])), this->board[i].r, this->board[i].g, this->board[i].b);
        }
      this->pixels.show(); 
    }
}

void NeoPixel::setBoardMode(uint8_t bm){
  this->mode = bm;
}

/* helpers */

String NeoPixel::RgbHex(rgb color){
  String r = (color.r < 16 ? "0" : "") + String(color.r, HEX);
  String g = (color.g < 16 ? "0" : "") + String(color.g, HEX);
  String b = (color.b < 16 ? "0" : "") + String(color.b, HEX);
  return r + g + b;
}
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
rgb NeoPixel::getBasicColor(uint8_t color){
   return this->S_BARVY[color];
}
void NeoPixel::setColor(uint8_t r, uint8_t g, uint8_t b){
  this->mainColor.r = r;
  this->mainColor.g = g;
  this->mainColor.b = b;
  cfg.setMainColor(mainColor, false);
}
void NeoPixel::setColor(uint8_t predef){
  this->setColor(this->getBasicColor(predef));
}
void NeoPixel::setColor(rgb c){
  this->setColor(c.r, c.g, c.b);
}
void NeoPixel::setColor(String hex){
  this->mainColor = this->hexRgb(hex);
}

void NeoPixel::setBgColor(uint8_t r, uint8_t g, uint8_t b){
  this->backColor.r = r;
  this->backColor.g = g;
  this->backColor.b = b;
  cfg.setBgColor(this->backColor, false);
}
void NeoPixel::setBgColor(uint8_t predef){
  this->setBgColor(this->getBasicColor(predef));
}
void NeoPixel::setBgColor(rgb c){
  this->setBgColor(c.r, c.g, c.b);
}
void NeoPixel::setBgColor(String hex){
  this->backColor = this->hexRgb(hex);
}
uint8_t NeoPixel::getPxID(uint8_t x, uint8_t y){
  return (y * PX_COLS) + x;
}

void NeoPixel::setPxColor(uint8_t px, rgb c){
  if(px < NUMPIXELS){
    this->board[px] = c;
  }
}
/*void NeoPixel::setPxColor(uint8_t px, uint8_t r, uint8_t g, uint8_t b){
  this->setPxColor(px, {r,g,b});
}*/
void NeoPixel::setPxColor(uint8_t px, uint8_t c){
  this->setPxColor(px, rgb{this->S_BARVY[c].r, this->S_BARVY[c].g, this->S_BARVY[c].b});
}
void NeoPixel::setPxColor(uint8_t px, String hex){
  this->setPxColor(px, this->hexRgb(hex));
}
void NeoPixel::setPxColor(pxcoor coor, rgb c){
  this->setPxColor(this->getPxID(coor.x, coor.y), c);
}
/*void NeoPixel::setPxColor(pxcoor coor, uint8_t r, uint8_t g, uint8_t b){
  this->setPxColor(this->getPxID(coor.x, coor.y), r, g, b);
}*/




rgb NeoPixel::getColor(){
  return this->mainColor;
}
rgb NeoPixel::getBgColor(){
  return this->backColor;
}
rgb NeoPixel::getPxColor(uint8_t px){
  return px < NUMPIXELS ? this->board[px] : rgb{0,0,0};
}
String NeoPixel::getPxColorHex(uint8_t px){
  return this->RgbHex(this->board[px]);
}

bool NeoPixel::isPxActive(uint8_t px){
  if(px < NUMPIXELS){
    return this->board[px].r || this->board[px].g || this->board[px].b;
  }
  return false;
}
bool NeoPixel::pxUnder(uint8_t mpx, uint8_t px){
  bool iss = false;
  for(uint8_t c = mpx; c < NUMPIXELS; c += 11){
    iss = px == c;
    if(iss){
      break;
    }
  }
  return iss;
}

/* class functions */
void NeoPixel::drawTime(bool ineffect, bool disableDraw){
  uint8_t h = mytime.getTime().h;
  const uint8_t m = mytime.getTime().m;
          h = h > 12 ? (h - 12) : (!h ? 12 : h);
          h = m > 55 ? (h < 12 ? h + 1 : 1) : h;
  const uint8_t m_recalc = (m - (m % 10) + ( (m % 10) > 7 ? 10 : 0 )) + ((m % 10) >= 3 && (m % 10) <= 7 ? 5 : 0);
  bool px[NUMPIXELS];
  for(uint8_t i=0; i<NUMPIXELS; i++){px[i] = false;}
    // JE | JSOU | BUDE | BUDOU
    if(m > 55 && (h == 2 || h == 3 || h == 4))  // budou
      {
        for(uint8_t i=0; i<5; i++)
        {
          px[budou[i]] = true;
         }
      }
    else if(m > 55)  // bude
      {
        for(uint8_t i=0; i<4; i++)
        {
          px[bude[i]] = true;
         }
      }
    else if(h == 2 || h == 3 || h == 4) // jsou
      {
        for(uint8_t i=0; i<4; i++)
        {
          px[jsou[i]] = true;
         }
      }
    else // je
      {
        for(uint8_t i=0; i<2; i++)
        {
          px[je[i]] = true;
         }
      }
    // HODIN
    for(uint8_t i=0; i < pgm_read_byte(&(pole_hodin_sizes[h])); i++)
      {
        px[pgm_read_byte(&(pole_hodin[h][i]))] = true;
      }
    // MINUT
    if(m >= 3 && m <= 55){
      const uint8_t indexMinut[11] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55};
      for(uint8_t i=0; i<11; i++)
        {
          if(m_recalc == indexMinut[i])
            {
              for(uint8_t im=0; im < pgm_read_byte(&(pole_minut_sizes[i])); im++)
              {
                px[pgm_read_byte(&(pole_minut[i][im]))] = true;
              }
            }
        }
    }

    // Aktivni px
    if(this->redraw_done){
      for(uint8_t i=1; i < NUMPIXELS; i++)
        {
          this->timePxlsNew[i] = px[i];
          // probehla zmena?
          if(this->timePxls[i] != this->timePxlsNew[i])
            {
              this->redraw_done = false;
              break;
            }
        }
    }

    for(uint8_t i=0; i < NUMPIXELS; i++)
      {
        if(!ineffect && !disableDraw){
          this->setPxColor(i, this->timePxls[i] ? this->mainColor : this->backColor);
        }
        else if(ineffect && this->timePxls[i]  && !disableDraw)
        {
          this->setPxColor(i, mainColor);
        }
      }
}


void NeoPixel::singleNumberPrint(uint8_t n, rgb color){
  // HODIN
  if(n == 0)
    {
      this->setPxColor(95, color);
      this->setPxColor(96, color);
      this->setPxColor(97, color);
      this->setPxColor(98, color);
    }
  else
    {
      for(int i=0; i < pgm_read_byte(&(pole_hodin_sizes[n])); i++)
        {
          this->setPxColor(pgm_read_byte(&(pole_hodin[n][i])), color);
        }
    }
  
}


void NeoPixel::drawCustom(){
  for(byte px = 0; px < NUMPIXELS; px++){
    this->setPxColor(px, this->timePxls[px] ? this->mainColor : this->customBG[px]);
  }
}

void NeoPixel::runRedrawEffect(){
  switch(rmode){
    default:
      this->redraw_frame = 0;
      this->redraw_done = true;
      break;
    case RMOD::FALL:
      for(byte px = 0; px < NUMPIXELS; px++){
        if(this->timePxls[px]){
          this->timePxls[px] = false;
          if(px + 11 < NUMPIXELS){
            this->timePxls[px + 11] = true;
          }
        }
      }
        this->redraw_frame++;
        if(this->redraw_frame == 11){
          this->redraw_frame = 0;
          this->redraw_done = true;
        }
      break;
  }
  if(this->redraw_done)
    {
      for(int i=0; i < NUMPIXELS; i++)
      {
        this->timePxls[i] = this->timePxlsNew[i];
      }
    }
}

void NeoPixel::drawSET(){
  this->ClearLED_MATRIX();
  for(uint8_t p=0; p<3;p++)
    {
      //this->setPxColor(px_SET[p], BARVY::BILA); jaaaaj 1.62 chyba po továrku!!
      this->setPxColor(pgm_read_byte(&(px_SET[p])), rgb{255,255,255});
    }
}


void NeoPixel::drawMatrix(bool v2){
  const rgb bColor = v2 ? rgb {22, 46, 22} : this->backColor;
  const rgb fColor = v2 ? rgb {55, 255, 15} : this->mainColor;
  const rgb cclrs[16]  = { //dimm(rgb color, uint8_t d)
                    {0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0},
                    this->dimm(bColor, 41),
                    this->dimm(bColor, 41),
                    this->dimm(bColor, 40),
                    this->dimm(bColor, 40),
                    this->dimm(bColor, 39),
                    this->dimm(bColor, 38),
                    this->dimm(bColor, 34),
                    this->dimm(bColor, 30),
                    this->dimm(bColor, 26),
                    this->dimm(bColor, 22),
                    this->dimm(bColor, 20),
                    this->dimm(bColor, 10),
                    this->dimm(bColor, 0)
    };
  if(!this->effect_slide){  // Generate Rain
    this->matrix_max_frames = 6;
    for(uint8_t c = 0; c < PX_COLS; c++)
      {
        this->matrix_rain_len[c] = random(0, 16);
        this->matrix_rain_pos[c] = random(-8, 0);

        if(this->matrix_rain_len[c] + this->matrix_rain_pos[c] > this->matrix_max_frames){
          this->matrix_max_frames = this->matrix_rain_len[c] + this->matrix_rain_pos[c];
        }
      }
  }
  for(uint8_t i=1; i < NUMPIXELS; i++)
  {
    this->fadePx(i, this->timePxls[i] && v2 ? 0 : 4);
  }
  for(uint8_t c = 0; c < PX_COLS; c++){
    this->matrix_rain_pos[c] += this->matrix_rain_pos[c] <  this->matrix_rain_len[c] ? random(1,2) : 0;
    map_colrow cc = col(c);
    for(uint8_t ccc = 0; ccc < this->matrix_rain_pos[c]; ccc++){
      int ci = (16 - this->matrix_rain_pos[c]) + ccc;
      if(!v2 && ccc < NUMPIXELS && !this->timePxls[cc.px[ccc]]){
        this->setPxColor(cc.px[ccc], cclrs[ci < 0 ? 0 : (ci > 15 ? 15 : ci)]);
      }else if(v2 && ccc < NUMPIXELS){
        this->setPxColor(cc.px[ccc], ci == 15 ? cclrs[15] : rgb {55, cclrs[ci < 0 ? 0 : (ci > 15 ? 15 : ci)].g + (64 - this->effect_slide) , 55});
      }
    }
  }

  this->effect_slide++;
  if(this->effect_slide >= this->matrix_max_frames){
    this->effect_slide = 0;
  }
  if(!v2){
    this->drawTime(true);
  }
}

void NeoPixel::drawFlame(bool v2){
  rgb bColor = v2 ? rgb {43, 33, 18} : this->backColor;
  rgb fColor = v2 ? rgb {255, 162, 56} : this->mainColor;
  rgb cclrs[16]  = { //dimm(rgb color, uint8_t d)
                    {0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0},
                    dimm(bColor, 41),
                    dimm(bColor, 41),
                    dimm(bColor, 40),
                    dimm(bColor, 40),
                    dimm(bColor, 39),
                    dimm(bColor, 38),
                    dimm(bColor, 34),
                    dimm(bColor, 30),
                    dimm(bColor, 26),
                    dimm(bColor, 22),
                    dimm(bColor, 20),
                    dimm(bColor, 10),
                    dimm(bColor, 0)
    };
  if(!effect_slide){  // Generate Rain
    matrix_max_frames = 6;
    for(uint8_t c = 0; c < PX_COLS; c++)
      {
        matrix_rain_len[c] = random(0, 16);
        matrix_rain_pos[c] = random(-16, 0);

        if(matrix_rain_len[c] + matrix_rain_pos[c] > matrix_max_frames){
          matrix_max_frames = matrix_rain_len[c] + matrix_rain_pos[c];
        }
      }
  }
  for(int i=1; i < NUMPIXELS; i++)
  {
    fadePx(i, /*!timePxls[i] ? 2 : 0*/2);
  }
  for(uint8_t c = 0; c < PX_COLS; c++){
    map_colrow cc = col(c);
    for(uint8_t ccc = 0; ccc < matrix_max_frames; ccc++){
      setPxColor(cc.px[ccc], cclrs[ccc]);
    }
  }
  effect_slide++;
  if(effect_slide >= matrix_max_frames){
    effect_slide = 0;
  }
  if(!v2){
    this->drawTime(true);
  }
}

void NeoPixel::drawHeart(){
  const uint8_t srdce1[26] = {38, 26, 14, 13, 12, 22, 33, 44, 55, 67, 79, 91, 103, 115, 105, 95, 85, 75, 65, 54, 43, 32, 20, 19, 18, 28};
  const uint8_t srdce2[20] = { 49, 39, 29, 30, 42, 53, 64, 74, 84, 94, 104, 92, 80, 68, 56, 45, 34, 24, 25, 37};
  const uint8_t srdce3[8] = { 60, 50, 62, 72, 82, 70, 58, 48};
  for(int i=1; i < NUMPIXELS; i++)
  {
    setPxColor(i, rgb{0,0,0});
  }

  if(effect_slide == 0)
  {
    for(int i=0; i < 26; i++)
    {
      setPxColor(srdce1[i], backColor);
    }
  }
  else
  {
    for(int i=0; i < 20; i++)
    {
      setPxColor(srdce2[i], backColor);
    }
  }
  effect_slide++;
  if(effect_slide == 2){
    effect_slide = 0;
  }
  this->drawTime(true);
}



uint8_t NeoPixel::timePxLastRow(uint8_t col){
  uint8_t l = 0;
  for(uint8_t r = col; r < NUMPIXELS; r += 11){
    if(this->timePxls[r]){
      l++;
    }
  }
  return l;
}

rgb NeoPixel::dimm(rgb color, uint8_t d){
  rgb newColor    = color;
      newColor.r -= newColor.r >= d ? d : newColor.r;
      newColor.g -= newColor.g >= d ? d : newColor.g;
      newColor.b -= newColor.b >= d ? d : newColor.b;
  return newColor;
}

void NeoPixel::setUpdate(uint32_t u){
  this->updateAt = u;
}

void NeoPixel::debug(){
  prntln("");
  prntln("Vizualizace:");
  prntln("||=============>"+ mytime.getTimeStr() + "<==========||");
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
