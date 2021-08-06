const char file_clock_js[] PROGMEM = R"=====(
/*
 * WORD CLOCK WEBOVÉ ROZHRANÍ
 * Česká verze, WiFi ESP8266, WS2412B NEOPIXEL
 *
 *  by Patrik Eder
 *  patrikeder.cz 2021
 *
 *  ESP Firmware: project.eders.cz/neoclock
 */
export default class {
  constructor(clock = null){
    this.clock = document.querySelector(clock);
    this.paleta = [
                    [   "W", "E", "B", "U", "D", "O", "U", "B", "U", "D", "E" ], //  00,  01,  02,  03,  04,  05,  06,  07,  08,  09,  10
                    [   "J", "E", "J", "S", "O", "U", "J", "E", "D", "N", "A" ], //  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21
                    [   "D", "E", "V", "Ě", "T", "P", "Ě", "T", "D", "V", "Ě" ], //  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32
                    [   "S", "E", "D", "M", "D", "V", "A", "N", "Á", "C", "T" ], //  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43
                    [   "D", "E", "S", "E", "T", "Ř", "I", "Š", "E", "S", "T" ], //  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54
                    [   "O", "S", "M", "J", "E", "D", "E", "N", "Á", "C", "T" ], //  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65
                    [   "Č", "T", "Y", "Ř", "I", "A", "D", "E", "S", "E", "T" ], //  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76
                    [   "D", "V", "A", "C", "E", "T", "Ř", "I", "C", "E", "T" ], //  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87
                    [   "P", "A", "T", "N", "Á", "C", "T", "N", "U", "L", "A" ], //  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98
                    [   "P", "E", "Č", "T", "Y", "Ř", "I", "C", "E", "T", "M" ], //  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109
                    [   "P", "A", "D", "E", "S", "Á", "T", "E", "P", "Ě", "T" ]  // 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
                  ];
    this.pixely = [
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000',
                    '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000', '#000'
                  ];
    this.activePixels = [
      [99, 255, 255, 255],
      [100, 255, 255, 255]
    ];
    this.vytvorHTML();
    this.interval = setInterval(() => {
      this.vykresli();
    }, 500);
    this.networks = [];
    this.cfg = {
        wifi_mode: 1,
        sta_ssid: "",
        time: [0,0,0],
        timezone: 2,
        main_color: [255,255,255],
        bg_color: [0,0,0],
        board_mode: 0
    };
    this.cfgLoad();
    this.pxloader = setInterval(() => {
      this.fetch("pixels.json")
          .then(data => {
            if('px' in data)
              {
                this.activePixels = data.px;
              }
          });
    }, 2000);
  }
  
  get rgbMainColor(){
    return `rgb(${this.cfg.main_color[0]},${this.cfg.main_color[1]},${this.cfg.main_color[2]})`;
  }
  get wifiStatus(){
    return this.cfg.wifi_mode == 1 ? "Režim AP" : `Připojeno k síti ${this.cfg.sta_ssid}`;
  }
  get strTime(){
    return `${this.cfg.time[0] < 10 ? "0" + this.cfg.time[0] : this.cfg.time[0]}:${this.cfg.time[1] < 10 ? "0" + this.cfg.time[1] : this.cfg.time[2]}:${this.cfg.time[2] < 10 ? "0" + this.cfg.time[2] : this.cfg.time[2]}`;
  }
  cfgLoad(){
    this.fetch("cfg.json")
        .then(data => {
          this.cfg = data;
          this.cfgHtmlUpdate();
        });
  }
  get scan(){
    return this.fetch("scan.json");
  }
  toHex(d) {
    return  ("0"+(Number(d).toString(16))).slice(-2).toUpperCase();
  }
  rgbHex(rgbarr = [0,0,0]){
    return `#${this.toHex(rgbarr[0])}${this.toHex(rgbarr[1])}${this.toHex(rgbarr[2])}`;
  }
  HexRgb(hex = "#000000"){
    hex = (hex.length == 7 ? hex.substring(1) : hex).toUpperCase();
    
    return [
      parseInt(hex.substring(0,2), 16),
      parseInt(hex.substring(2,4), 16),
      parseInt(hex.substring(4,6), 16)
    ];
  }
  cfgHtmlUpdate(){
    document.querySelector("#status").innerHTML   = this.wifiStatus;
    document.querySelector("#colorselect").value  = this.rgbHex(this.cfg.main_color);
    document.querySelector("#modeselect").value   = this.cfg.board_mode;
    document.querySelector("#manualtime").value   = this.strTime;
    document.querySelector("#timezone").value     = this.cfg.timezone;
  }
  
  async fetch(file = "cfg.json", post = false){
    return new Promise((resolve, reject) => {
          let request = new XMLHttpRequest();
              request.open(post ? 'POST' : 'GET', file, true);
              request.timeout = 10000;
              request.ontimeout = reject;
              request.onerror = reject;
              request.onreadystatechange = function(){
                if(this.readyState == 4 && this.status == 200){
                  let data = this.responseText;
                  if(data === null || data === '')
                    {
                      data = '{}';
                    }
                  let rObj = {};
                      try {
                          rObj = JSON.parse(data);
                      } catch (e) {
                          reject(e);
                      }
                      resolve(rObj);
                }
              };
              if(post)
                {
                  request.send(encodeURI(post));
                }
              else
                {
                  request.send();
                }
        });
  }
  settingsCard(t = "", c = null){
    let card    = document.createElement("div"),
        title   = document.createElement("h4"),
        content = document.createElement("div");
        card.classList.add("sett-card");
        content.classList.add("sett-card-content");
        title.innerHTML = t;
        content.appendChild(c);
        card.appendChild(title);
        card.appendChild(content);
    
    return card;
  }
  vytvorHTML()
    {
      let hodiny    = document.createElement("div");
          hodiny.classList.add("hodiny");
      let cid = 0,
          that = this;
      for(let row of this.paleta)
        {
          let rowel = document.createElement("div");
              rowel.classList.add("row");
          for(let cell of row)
            {
              let cellel = document.createElement("div");
                  cellel.classList.add("cell");
                  if(cell == "W")
                    {
                      cellel.classList.add("wifi");
                    }
                  cellel.setAttribute("cellid", cid);
              for(let letter of cell)
                {
                  let letterEl = document.createElement("div");
                      letterEl.classList.add("letter");
                      letterEl.setAttribute("char", letter);
                      cellel.appendChild(letterEl);
                }
                rowel.appendChild(cellel);
                cid++;
            }
            hodiny.appendChild(rowel);
        }
        this.clock.appendChild(hodiny);
        this.listeners();
    }
  listeners(){
    let that = this;
    // nastaveni toggle
    document.querySelector(".hodiny").addEventListener("click", e => {
          if(document.querySelector("body").classList.contains("sett"))
            {
              document.querySelector("body").classList.remove("sett");
            }
          else
            {
              document.querySelector("body").classList.add("sett");
            }
        });
    document.querySelector("#cancel").addEventListener("click", e => {
          document.querySelector("body").classList.remove("sett");
        });
        
    // vychozi barva
    document.querySelector("#colorselect").addEventListener("input", e => {
          that.cfg.main_color = that.HexRgb(e.target.value);
        });
    // bg barva
    document.querySelector("#bgcolorselect").addEventListener("input", e => {
          that.cfg.bg_color = that.HexRgb(e.target.value);
        });
    // cas z prohlizece
      document.querySelector("#timebutton").addEventListener("click", e => {
        const dt = new Date();
              that.cfg.time[0] = dt.getHours();
              that.cfg.time[1] = dt.getMinutes();
              that.cfg.time[2] = dt.getSeconds();
              console.log(this.cfg.time);
              document.querySelector("#manualtime").value = that.strTime;
      });
    // ntp cas
      document.querySelector("#ntpbutton").addEventListener("click", e => {
        if(that.cfg.wifi_mode == 2)
          {
            that.fetch("ntp").then(data => {
              that.cfgLoad();
            });
          }
      });
    
    // rucni uprava
      document.querySelector("#manualtime").addEventListener("input", e => {
        let t = e.target.value.split(":");
        that.cfg.time[0] = Number(t[0]);
        that.cfg.time[1] = Number(t[1]);
        that.cfg.time[2] = Number(t[2]);
      });
    // scan siti
    document.querySelector("#scanbutton").addEventListener("click", e => {
          for(let opt of document.querySelector("#networks").querySelectorAll("option")){opt.remove();}
          let option = document.createElement("option");
                    option.innerHTML = "Probíhá vyhledávání sítí";
                document.querySelector("#networks").appendChild(option);
          this.scan.then(data => {
            this.networks = data;
            for(let opt of document.querySelector("#networks").querySelectorAll("option")){opt.remove();}
            for(let i in data)
              {
                    option = document.createElement("option");
                    option.value = i;
                    option.innerHTML = data[i][0];
                document.querySelector("#networks").appendChild(option);
              }
          });
        });
        
      // pripojit k wifi
    document.querySelector("#connect").addEventListener("click", e => {
          let ssid = that.networks[Number(document.querySelector("#networks").value)][0],
              pass = document.querySelector("#pwd").value;
              that.fetch(`apply?ssid=${ssid}&pwd=${pass}`).
                then(data => {
                  console.log(data);
                });
              
        });
      // aplikovat
    document.querySelector("#applyall").addEventListener("click", e => {
          let cfg = {
                time_h: that.cfg.time[0],
                time_m: that.cfg.time[1],
                time_s: that.cfg.time[2],
                timezone: that.cfg.timezone,
                color_r: that.cfg.main_color[0],
                color_g: that.cfg.main_color[1],
                color_b: that.cfg.main_color[2]
              },
              query = [];
          for(let key in cfg)
            {
              query.push(`${key}=${cfg[key]}`);
            }
          
          that.fetch(`apply?${query.join("&")}`)
            .then(data => {
              console.log(data);
            });
        });
        
      // tovarko
      document.querySelector("#defaults").addEventListener("click", e => {
        if(confirm("Opravdu chcete provést tovární nastavení?"))
          {
            that.fetch("apply?factoryReset")
              .then(data => {
                console.log(data);
              });
          }
      });
  }
  
  vykresli()
    {
      // vypnout pixely
      for(let p in this.pixely)
        {
          this.pixely[p] = "#000";
        }
      // zapnout pixely
      for(let p of this.activePixels)
        {
          this.pixely[p[0]] = `rgb(${p[1]},${p[2]},${p[3]})`;
        }
      // vykreslit
      for(let p in this.pixely)
        {
          document.querySelector(`[cellid="${p}"]`).style["text-shadow"] = this.pixely[p] === "#000" ? "none": "0 0 calc( var(--cell-size) / 4)";
          document.querySelector(`[cellid="${p}"]`).style.color = this.pixely[p];
        }
    }
}
)=====";
