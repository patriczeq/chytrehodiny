#include "neopixel.h"
#include "progmem_data.h"
NeoPixel::NeoPixel() {}

NeoPixel::~NeoPixel() {}

void NeoPixel::setup(){
  this->enable();
  this->setColor(cfg.getMainColor());
  this->setBgColor(cfg.getBgColor());
  this->setBright(cfg.getBright());
  for(uint8_t i = 0; i < NUMPIXELS; i++)
    {
      this->board[i] = this->mainColor;
    }
  logger("PIXELS", "READY!");
  this->setMode(cfg.getBoardMode());
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
  if(mode == MOD::WIFI_JEDE){
    this->setPxColor(0, {255,255,255}); // wifi
    this->setPxColor(58, {255,255,255}); // J
    this->setPxColor(59, {255,255,255}); // E
    this->setPxColor(60, {255,255,255}); // D
    this->setPxColor(61, {255,255,255}); // E
  }
  else if(!this->redraw_done && currentTime - this->redraw_loop >= this->updateAt){
    this->runRedrawEffect();
    this->redraw_loop = currentTime;
  }
  else if(currentTime - this->lastUpdate >= this->updateAt)
    {
      
      this->drawTime(true, true); // updateTimePxls
      switch(mode){
        case MOD::SET:
          this->updateAt = 500;
          this->drawSET();
          break;
        case MOD::HODINY:
          this->updateAt = 1000;
          this->drawTime();
          break;
        case MOD::SECONDS:
          this->updateAt = 250;
          this->drawMinOrSec(true);
          break;
        case MOD::MINUTES:
          this->updateAt = 250;
          this->drawMinOrSec(false);
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
        case MOD::CUSTOM:
          this->updateAt = 1000;
          this->drawCustom();
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
      }
            
      this->lastUpdate = currentTime;
      this->draw();
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
    for(uint8_t c = 0; c < NUMPIXELS; c++){
      this->setPxColor(c, {0,0,0});
    }
    this->effect_slide = 0;
    this->mode = m;
  }
void NeoPixel::setBright(uint8_t b){
  this->bright = b;
}
uint8_t NeoPixel::getBright(){
  return this->bright;
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
void NeoPixel::setColor(uint8_t r, uint8_t g, uint8_t b){
  this->mainColor.r = r;
  this->mainColor.g = g;
  this->mainColor.b = b;
  cfg.setMainColor(mainColor, false);
}
void NeoPixel::setColor(uint8_t predef){
  switch(predef)
    {
      case BARVY::CERNA:    this->setColor(  0,  0,  0);break;
      case BARVY::CERVENA:  this->setColor(255,  0,  0);break;
      case BARVY::ZELENA:   this->setColor(  0,255,  0);break;
      case BARVY::MODRA:    this->setColor(  0,  0,255);break;
      case BARVY::ZLUTA:    this->setColor(255,255,255);break;
      case BARVY::FIALOVA:  this->setColor(255,  0,255);break;
      case BARVY::AZUROVA:  this->setColor(  0,255,255);break;
      case BARVY::BILA:     this->setColor(255,255,255);break;
    }
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
  switch(predef)
    {
      case BARVY::CERNA:    this->setBgColor(  0,  0,  0);break;
      case BARVY::CERVENA:  this->setBgColor(255,  0,  0);break;
      case BARVY::ZELENA:   this->setBgColor(  0,255,  0);break;
      case BARVY::MODRA:    this->setBgColor(  0,  0,255);break;
      case BARVY::ZLUTA:    this->setBgColor(255,255,255);break;
      case BARVY::FIALOVA:  this->setBgColor(255,  0,255);break;
      case BARVY::AZUROVA:  this->setBgColor(  0,255,255);break;
      case BARVY::BILA:     this->setBgColor(255,255,255);break;
    }
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

void NeoPixel::setPxColor(pxcoor coor, rgb c){
  this->setPxColor(this->getPxID(coor.x, coor.y), c);
}
void NeoPixel::setPxColor(pxcoor coor, uint8_t r, uint8_t g, uint8_t b){
  this->setPxColor(this->getPxID(coor.x, coor.y), r, g, b);
}
void NeoPixel::drawCircle(uint8_t x0, uint8_t y0, uint8_t sz) {
  this->drawCircle(x0, y0, sz, this->mainColor);
}
void NeoPixel::drawCircle(uint8_t x0, uint8_t y0, uint8_t sz, rgb color) {
  for(uint8_t x = 0; x < sz/2; x ++){
    for(uint8_t y = 0; y < sz/2; y++){
      this->setPxColor(getPxID(x0 + x, y0 + y), color);
    }
  }
}

void NeoPixel::fillSquare(uint8_t x0, uint8_t y0, uint8_t sz) {
  this->drawCircle(x0, y0, sz, this->mainColor);
}
void NeoPixel::fillSquare(uint8_t x0, uint8_t y0, uint8_t sz, rgb color) {
  for(uint8_t x = 0; x < sz/2; x ++){
    for(uint8_t y = 0; y < sz/2; y++){
      this->setPxColor(getPxID(x0 + x, y0 + y), color);
    }
  }
}

void NeoPixel::setPxColor(uint8_t px, uint8_t predef){
  switch(predef)
    {
      case BARVY::CERNA:    this->setPxColor(px,  0,  0,  0);break;
      case BARVY::CERVENA:  this->setPxColor(px,255,  0,  0);break;
      case BARVY::ZELENA:   this->setPxColor(px,  0,255,  0);break;
      case BARVY::MODRA:    this->setPxColor(px,  0,  0,255);break;
      case BARVY::ZLUTA:    this->setPxColor(px,255,255,255);break;
      case BARVY::FIALOVA:  this->setPxColor(px,255,  0,255);break;
      case BARVY::AZUROVA:  this->setPxColor(px,  0,255,255);break;
      case BARVY::BILA:     this->setPxColor(px,255,255,255);break;
    }
}
void NeoPixel::setPxColor(uint8_t px, uint8_t r, uint8_t g, uint8_t b){
  if(px < NUMPIXELS){
    this->board[px].r = r;
    this->board[px].g = g;
    this->board[px].b = b;
  }
}
void NeoPixel::setPxColor(uint8_t px, rgb c){
  this->board[px] = c;
}
void NeoPixel::setPxColor(uint8_t px, String hex){
  this->board[px] = this->hexRgb(hex);
}
rgb NeoPixel::getColor(){
  return this->mainColor;
}
rgb NeoPixel::getBgColor(){
  return this->backColor;
}
rgb NeoPixel::getPxColor(uint8_t px){
  return this->board[px];
}
String NeoPixel::getPxColorHex(uint8_t px){
  return this->RgbHex(this->board[px]);
}
void NeoPixel::offPx(uint8_t px){
  this->board[px] = this->backColor;
}
bool NeoPixel::isPxActive(uint8_t px){
  return this->board[px].r || this->board[px].g || this->board[px].b;
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
  uint8_t m = mytime.getTime().m;
  
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

    // Aktivni px
    if(this->redraw_done){
      for(int i=0; i < NUMPIXELS; i++)
        {
          this->timePxlsNew[i] = px[i] ? true : false;
          // probehla zmena?
          if(this->timePxls[i] != this->timePxlsNew[i])
            {
              this->redraw_done = false;
            }
        }
    }

    for(int i=0; i < NUMPIXELS; i++)
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



void NeoPixel::drawBigNumber(uint8_t x, uint8_t y, uint8_t n, rgb color){
  if(n >= 0 && n < 10){
    for(int im=0; im < pgm_read_byte(&(bignum_sizes[n])); im++)
      {
        this->setPxColor(((pgm_read_byte(&(pole_bignum[n][im])) + x) - 22) + (y * 11), color);
      }
  }
}


void NeoPixel::drawMinOrSec(bool _sec){
  uint8_t n = _sec ? mytime.getTime().s : mytime.getTime().m;
  uint8_t f = n < 10 ? 0 : ((n - (n % 10))/10);
  uint8_t s = n % 10;

  for(int i=0; i < NUMPIXELS; i++)
  {
    this->setPxColor(i, rgb {0, 0, 0});
  }
  
  this->drawBigNumber(0, 2, f, this->backColor);
  this->drawBigNumber(6, 2, s, this->backColor);
  this->drawTime(true);

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
  for(int i=0; i < NUMPIXELS; i++)
    {
      setPxColor(i, backColor);
    }
  for(int i=0; i<3; i++)
  {
    setPxColor(pgm_read_byte(&(w_set[i])), mainColor);
   }
}


void NeoPixel::drawMatrix(bool v2){
  rgb bColor = v2 ? rgb {22, 46, 22} : this->backColor;
  rgb fColor = v2 ? rgb {55, 255, 15} : this->mainColor;
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
        matrix_rain_pos[c] = random(-8, 0);

        if(matrix_rain_len[c] + matrix_rain_pos[c] > matrix_max_frames){
          matrix_max_frames = matrix_rain_len[c] + matrix_rain_pos[c];
        }
      }
  }
  for(int i=1; i < NUMPIXELS; i++)
  {
    fadePx(i, timePxls[i] && v2 ? 0 : 2);
  }
  for(uint8_t c = 0; c < PX_COLS; c++){
    matrix_rain_pos[c] += matrix_rain_pos[c] <  matrix_rain_len[c] ? random(1,2) : 0;
    map_colrow cc = col(c);
    for(uint8_t ccc = 0; ccc < matrix_rain_pos[c]; ccc++){
      int ci = (16 - matrix_rain_pos[c]) + ccc;
      if(!timePxls[cc.px[ccc]]){
        setPxColor(cc.px[ccc], cclrs[ci < 0 ? 0 : (ci > 15 ? 15 : ci)]);
      }else if(v2){
        setPxColor(cc.px[ccc], ci == 15 ? cclrs[15] : rgb {55, cclrs[ci < 0 ? 0 : (ci > 15 ? 15 : ci)].g + (64 - effect_slide) , 55});
      }
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
  uint8_t srdce1[26] = {38, 26, 14, 13, 12, 22, 33, 44, 55, 67, 79, 91, 103, 115, 105, 95, 85, 75, 65, 54, 43, 32, 20, 19, 18, 28};
  uint8_t srdce2[20] = { 49, 39, 29, 30, 42, 53, 64, 74, 84, 94, 104, 92, 80, 68, 56, 45, 34, 24, 25, 37};
  uint8_t srdce3[8] = { 60, 50, 62, 72, 82, 70, 58, 48};
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
    if(timePxls[r]){
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
  updateAt = u;
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
