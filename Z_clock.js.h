const char file_clock_js[] PROGMEM = R"=====(
/*
 * 
 * Hodiny
 * patrikeder.cz 2021
 * 
 */
export default class {
  constructor(clock = null){
    this.clock = this.qs(clock);
    this.pixely = [];
    for(let i = 0; i < 121; i ++){
      this.pixely.push("#000");
    }
    this._notify = {
      shown: 0,
      timeout: 5000,
      style: "info", // info, success, danger
      visible: false,
      message: ""
    };
    this.drawColor = "#fff";
    this.custom = [];
    for(let i = 0; i < 121; i ++){
      this.custom.push("#000");
    }
    
    this.cfg = {
        wifi_mode: 1,
        version: 0,
        remote_version: 0,
        sta_ssid: "",
        time: [0,0,0],
        date: [2000,1,1],
        my_msg: "",
        timezone: 2,
        main_color: [255,255,255],
        bg_color: [0,0,0],
        msg: "",
        day_info: "",
        bright: 0,
        speed: 100,
        board_mode: 0
    };
    this.activePixels = [];
    this.prepareHTML();
    this.interval = setInterval(() => {
      this.vykresli();
      this.checkNotify();
    }, 500);
    this.lastUpdate = 0;
    this.networks = [];
    this.socketid = -1;
    this.ws;
    //this.WSinit();
    this.init();
    this.loaded = false;

  }
  addDays(date, days){
    const ndate = new Date(Number(new Date(date)) + (days * 86400000))
    return `${ndate.getFullYear()}-${ndate.getMonth()+1 < 10 ? '0' + (ndate.getMonth()+1) : ndate.getMonth()+1}-${ndate.getDate()+1 < 10 ? '0' + (ndate.getDate()+1) : ndate.getDate()+1}`;
  }
  loadDayInfo(){
    this.qs(".dayinfo").innerHTML = this.cfg.day_info;
  }
  checkNotify(){
    if(Number(new Date()) - this._notify.shown >= this._notify.timeout && this._notify.visible){
      this._notify.visible = false;
      this.qs("#notify").classList.remove("visible");
    }
  }
  notify(type = "info", msg = "", timeout = 5000){
    this._notify.timeout = timeout;
    this._notify.style = type;
    this._notify.message = msg;
    this._notify.visible = true;
    this.qs("#notify").classList.add("visible");
    this._notify.shown = Number(new Date());
    this.qs("#notify").classList.remove("info");
    this.qs("#notify").classList.remove("success");
    this.qs("#notify").classList.remove("danger");
    this.qs("#notify").classList.add(this._notify.style);
    this.qs("#notify").innerHTML = this._notify.message;
  }
  init(){
    this.fetch("cfg.json")
    .then(data => {
      this.cfg = data;
      this.qs("#verison").innerHTML = this.cfg.version
      this.cfgHtmlUpdate();
      this.qs("body").classList.add(`mode-${this.cfg.wifi_mode == 2 ? 'sta' : 'ap'}`);
      this.loadDayInfo();
      if(this.cfg.wifi_mode == 2){ // sta
        this.WSinit();
        this.qs("body").classList.remove("loading");
      }else{
        this.loaded = true;
        this.notify("info","AP mode", 3000);
      }
    });
  }
  
  WSinit(){
    if('hodiny' in window){
      window.hodiny.notify("info","Připojuji se...", 10000);
      window.hodiny.lastUpdate = Number(new Date());
      window.hodiny.ws = new WebSocket(`ws://${window.location.hostname}/socket`);
      window.hodiny.ws.onopen = e => {
        window.hodiny.lastUpdate = Number(new Date());
        window.hodiny.loaded = true;
        window.hodiny.notify("success","Připojeno", 1000);
        window.hodiny.qs("body").classList.remove("loading");
        setInterval(()=>{
          if(window.hodiny.loaded && Number(new Date()) - window.hodiny.lastUpdate >= 5000){
            window.hodiny.notify("warning","Ztráta spojení...", 10000);
            window.hodiny.qs("body").classList.add("loading");
            setTimeout(()=>{
              window.hodiny.ws.close();
            }, 1000);
          }
        },500);
      };
      window.hodiny.ws.onclose = e => {
        window.hodiny.notify("danger","Odpojeno", 10000);
        window.hodiny.qs("body").classList.add("loading");
        setTimeout(window.hodiny.WSinit, 1000);
      };
      window.hodiny.ws.onmessage = e => {
        const json = JSON.parse(e.data);
        if('px' in json){
          window.hodiny.activePixels = json.px.split("x");
        }
        if('error' in json){
          console.error(`Socket error: ${json.error}`);
        }
        if('color' in json){
          window.hodiny.cfg.main_color = window.hodiny.HexRgb(json.color);
          window.hodiny.cfgHtmlUpdate();
        }
        if('bgcolor' in json){
          window.hodiny.cfg.bg_color = window.hodiny.HexRgb(json.bgcolor);
          window.hodiny.cfgHtmlUpdate();
        }
        if('bright' in json){
          window.hodiny.cfg.bright = window.hodiny.HexRgb(json.bright);
          window.hodiny.cfgHtmlUpdate();
        }
        if('speed' in json){
          window.hodiny.cfg.speed = window.hodiny.HexRgb(json.speed);
          window.hodiny.cfgHtmlUpdate();
        }
        if('board_mode' in json){
          window.hodiny.cfg.board_mode = json.board_mode;
          window.hodiny.cfgHtmlUpdate();
        }
        if('fw_update' in json && json.fw_update == "start"){
          this.notify("danger","Probíhá aktualizace Firmware!", 60000);
        }
        if('ntp_update' in json){
          if(json.ntp_update)
            {
              this.notify("success", "Synchronizace byla úspěšná!", 2000);
            }
           else
            {
              this.notify("danger", "Chyba aktualizace!", 2000);
            }
        }
        if('time' in json){
          this.notify("success", "Čas byl nastaven...", 2000);
        }
        if('display_apply' in json){
          this.notify("success", "Nastavení zobrazení uloženo", 2000);
        }
        if('reboot' in json && json.reboot == 'OK'){
          this.notify("success", "Restartuji...", 2000);
        }
        
        window.hodiny.lastUpdate = Number(new Date());
      };
    }
  }
  wsRequest(msg = ''){
    return this.ws.send(msg);
  }
  wsSett(json = {}){
    return this.ws.send(JSON.stringify(json));
  }
  get rgbMainColor(){
    return `rgb(${this.cfg.main_color[0]},${this.cfg.main_color[1]},${this.cfg.main_color[2]})`;
  }
  get wifiStatus(){
    return this.cfg.wifi_mode == 1 ? "Režim AP" : `Připojeno k síti ${this.cfg.sta_ssid}`;
  }
  get strTime(){
    return `${this.cfg.time[0] < 10 ? "0" + this.cfg.time[0] : this.cfg.time[0]}:${this.cfg.time[1] < 10 ? "0" + this.cfg.time[1] : this.cfg.time[1]}:${this.cfg.time[2] < 10 ? "0" + this.cfg.time[2] : this.cfg.time[2]}`;
  }
  get strDate(){
    return `${this.cfg.date[0]}-${this.cfg.date[1] < 10 ? "0" + this.cfg.date[1] : this.cfg.date[1]}-${this.cfg.date[2] < 10 ? "0" + this.cfg.date[2] : this.cfg.date[2]}`;
  }
  cfgLoad(){
    this.fetch("cfg.json")
        .then(data => {
          this.cfg = data;
          this.loadDayInfo();
          this.cfgHtmlUpdate();
          this.notify("success","Připojeno", 1000);
          this.loaded = true;
        });
  }
  get scan(){
    return this.fetch("scan.json");
  }
  toHex(d) {
    return  ("0"+(Number(d).toString(16))).slice(-2).toUpperCase();
  }
  qs(el = "body"){
    return document.querySelector(el);
  }
  ce(el = "span"){
    return document.createElement(el);
  }
  listen(el = "body", ev = "click", ef = ()=>{}){
    if(this.qs(el) === null){
      return false;
    }
    this.qs(el).addEventListener(ev, ef);
    return true;
  }
  rgbHex(rgbarr = [0,0,0]){
    return `#${this.toHex(rgbarr[0])}${this.toHex(rgbarr[1])}${this.toHex(rgbarr[2])}`;
  }
  HexRgb(hex = "#000000"){
    hex = typeof hex == "string" && hex.length == 4 ? hex + hex.substring(1) : hex;
    hex = typeof hex == "string" ? ((hex.length == 7 ? hex.substring(1) : hex).toUpperCase()) : "#000000";
    return [
      parseInt(hex.substring(0,2), 16),
      parseInt(hex.substring(2,4), 16),
      parseInt(hex.substring(4,6), 16)
    ];
  }
  arrToTime(arr){
    let nArr = [];
    for(let t of arr){
      nArr.push(t<10 ? "0" + String(t) : String(t));
    }
    return nArr.join(":");
  }
  cfgHtmlUpdate(){
    this.qs("#status").innerHTML    = this.wifiStatus;
    this.qs("#colorselect").value   = this.rgbHex(this.cfg.main_color);
    this.qs("#bgcolorselect").value = this.rgbHex(this.cfg.bg_color);
    this.qs("#modeselect").value    = this.cfg.board_mode;
    this.qs("#manualtime").value    = this.strTime;
    this.qs("#manualdate").value    = this.strDate;
    this.qs("#timezone").value      = this.cfg.timezone;
    this.qs("#bright").value        = this.cfg.bright;
    this.qs("#speed").value         = this.cfg.speed;
    this.qs("#msgvalue").value      = this.cfg.msg;
    this.qs("#schedule").checked    = this.cfg.schedule.enabled;
    this.qs("#schedulebrightf").value = this.arrToTime(this.cfg.schedule.from);
    this.qs("#schedulebrightt").value = this.arrToTime(this.cfg.schedule.to);
    this.qs("#schedulebright").value = this.cfg.schedule.bright;

    if(this.cfg.board_mode == 20 || this.cfg.board_mode == 21){
      this.qs(".custommsg").style.display = "block";
    }
    if(this.cfg.board_mode == 200){
      this.qs(".customimg").style.display = "block";
    }
    if(this.cfg.wifi_mode !== 2){
      this.qs("#section_display").style.display = "none";
      this.qs("#section_time").style.display = "none";
      this.qs("#section_ntp").style.display = "none";
    }
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
    let card    = this.ce("div"),
        title   = this.ce("h4"),
        content = this.ce("div");
        card.classList.add("sett-card");
        content.classList.add("sett-card-content");
        title.innerHTML = t;
        content.appendChild(c);
        card.appendChild(title);
        card.appendChild(content);
    
    return card;
  }
  prepareHTML()
    {
      let that = this;      
        new myPicker("colorselect", color => {
          that.cfg.main_color = that.HexRgb(color);
          that.wsSett({color: color});
        },color => {
          that.cfg.main_color = that.HexRgb(color);
          that.wsSett({color: color});
        }, false);
        new myPicker("bgcolorselect",color => {
          that.cfg.bg_color = that.HexRgb(color);
          that.wsSett({bgcolor: color});
        },color => {
          that.cfg.bg_color = that.HexRgb(color);
          that.wsSett({bgcolor: color});
        }, false);
        new myPicker("pencolor",color => {
          that.drawColor = color;
        },color => {
          that.drawColor = color;
        }, false);       
        this.listeners();
    }
  applySingle(key = "", value = ""){
    this.fetch(`apply?${key}=${value}`)
      .then(data => {
      });
  }
  strtimeFix(input){
    if(input.length == 2){
      return `${input}:00:00`;
    }else if(input.length == 5){
      return `${input}:00`;
    }else{
      return input;
    }
  }
  drawCust(i){
    this.custom[i] = this.custom[i] == this.drawColor ? "#000" : this.drawColor;
    this.reDrawCust();
    const rgb = this.HexRgb(this.custom[i]);
    this.wsSett({custom_bg: i, cust_r: rgb[0], cust_g: rgb[1], cust_b: rgb[2]});
  }
  reDrawCust(){
    const width = 256,
        height = 256,
        canvas = document.getElementById("drawboard"),
        ctx = canvas.getContext("2d");
   let x = 0,
       y = 0;
       ctx.clearRect(0, 0, canvas.width, canvas.height);
        for(let p in this.custom)
          {
            if(this.custom[p] !== "#000" && this.custom[p] !== "#000000"){
              ctx.fillStyle = this.custom[p];
              ctx.fillRect((width/11) * x, (height/11) * y, width / 11, height / 11);
            }
            x++;
            if(x > 10){
              x = 0;
              y++;
            }
          }
  }
  get custData(){
    let pxx = [];
    for(let px in this.custom)
      {
        if(this.custom[px] !== "#000" && this.custom[px] !== "#000000")
          {
            let color = this.HexRgb(this.custom[px]);
            pxx.push([px, color[0], color[1], color[2]]);
          }
      }
    return pxx;
  }
  CharDisplay(r){
       return r.replace(new RegExp(/[àáâãäå]/g), "a")
                .replace(new RegExp(/[çč]/g), "c")
                .replace(new RegExp(/[ď]/g), "d")
                .replace(new RegExp(/[èéêëě]/g), "e")
                .replace(new RegExp(/[ìíîï]/g), "i")
                .replace(new RegExp(/[ñň]/g), "n")
                .replace(new RegExp(/[òóôõö]/g), "o")
                .replace(new RegExp(/[œ]/g), "oe")
                .replace(new RegExp(/[ř]/g), "r")
                .replace(new RegExp(/[š]/g), "s")
                .replace(new RegExp(/[ť]/g), "t")
                .replace(new RegExp(/[ůùúûü]/g), "u")
                .replace(new RegExp(/[ýÿ]/g), "y")
                .replace(new RegExp(/[ž]/g), "z")
                .replace(new RegExp(/[ÀÁÂÃÄÅ]/g), "A")
                .replace(new RegExp(/[ÇČ]/g), "C")
                .replace(new RegExp(/[Ď]/g), "D")
                .replace(new RegExp(/[ÈÉÊËĚ]/g), "E")
                .replace(new RegExp(/[ÌÍÎÏ]/g), "I")
                .replace(new RegExp(/[ÑŇ]/g), "N")
                .replace(new RegExp(/[ÒÓÔÕÖ]/g), "O")
                .replace(new RegExp(/[Œ]/g), "OE")
                .replace(new RegExp(/[Ř]/g), "R")
                .replace(new RegExp(/[Š]/g), "S")
                .replace(new RegExp(/[Ť]/g), "T")
                .replace(new RegExp(/[ŮÙÚÛÜ]/g), "U")
                .replace(new RegExp(/[ÝŸ]/g), "Y")
                .replace(new RegExp(/[Ž]/g), "Z");
  }
  listeners(){
    let that = this;
    this.listen(".noarea", "click", e => {
      const bcls = this.qs("body").classList;
      bcls[bcls.contains("sett") ? "remove" : "add"]("sett");
    });

    this.listen(".boardClick", "click", e => {
      const w = e.target.offsetWidth,
            h = e.target.offsetHeight,
            t = e.layerY - 8,
            l = e.layerX - 8,
            r = Math.round(t / (h / 11)),
            c = Math.round(l / (w / 11)),
            p = (r * 11) + c;
         that.drawCust(p);
    });
    
    this.listen("#cancel", "click", e => {
      that.qs("body").classList.remove("sett");
    });
    this.listen("#colorselect", "input", e => {
      that.cfg.main_color = that.HexRgb(e.target.value);
      that.wsSett({color: e.target.value});
    });
    this.listen("#bgcolorselect", "input", e => {
      that.cfg.bg_color = that.HexRgb(e.target.value);
      that.wsSett({bgcolor: e.target.value});
    });
    this.listen("#modeselect", "input", e => {
      that.cfg.board_mode = Number(e.target.value);
      this.qs(".customimg").style.display = that.cfg.board_mode == 200 ? "block" : "none";
      this.qs(".custommsg").style.display = that.cfg.board_mode == 20 || that.cfg.board_mode == 21 ? "block" : "none";
    });
    this.listen("#modeselect", "change", e => {
      that.cfg.board_mode = Number(e.target.value);
      this.qs(".customimg").style.display = that.cfg.board_mode == 200 ? "block" : "none";
      this.qs(".custommsg").style.display = that.cfg.board_mode == 20 || that.cfg.board_mode == 21 ? "block" : "none";
      that.wsSett({board_mode: e.target.value});
    });
    this.listen("#bright", "input", e => {
      that.cfg.bright = Number(e.target.value);
       that.wsSett({bright: Number(e.target.value)});
    });
    this.listen("#bright", "change", e => {
      that.cfg.bright = Number(e.target.value);
        that.wsSett({bright: Number(e.target.value)});
    });
    this.listen("#speed", "input", e => {
      that.cfg.speed = Number(e.target.value);
       that.wsSett({speed: Number(e.target.value)});
    });
    this.listen("#speed", "change", e => {
      that.cfg.speed = Number(e.target.value);
       that.wsSett({speed: Number(e.target.value)});
    });
    this.listen("#schedule", "change", e => {
        that.cfg.schedule.enabled = e.target.checked;
        that.wsSett({schedule_enable: e.target.checked});
    });
    this.listen("#schedulesave", "click", e => {
        that.cfg.schedule = {
          enabled: that.qs("#schedule").checked,
          from: that.qs("#schedulebrightf").value,
          to: that.qs("#schedulebrightt").value,
          bright: Number(that.qs("#schedulebright").value)
        };
        that.wsSett({
          schedule_from: that.strtimeFix(that.cfg.schedule.from),
          schedule_to: that.strtimeFix(that.cfg.schedule.to),
          schedule_bright: that.cfg.schedule.bright
        });
    });
    this.listen("#saveDisplay", "click", e => {
      that.wsRequest("saveDisplay");
    });
    this.listen("#msgvalue", "input", e => {
      that.qs("#msgvalue").value = that.CharDisplay(that.qs("#msgvalue").value);
    });
    this.listen("#msgvalue", "keyup", e => {
      that.qs("#msgvalue").value = that.CharDisplay(that.qs("#msgvalue").value);
    });

    this.listen("#sendMSG", "click", e => {
      that.wsSett({
          msg: that.CharDisplay(that.qs("#msgvalue").value)
        });
    });
    // cas z prohlizece
    this.listen("#timebutton", "click", e => {
      const dt = new Date();
            that.cfg.time[0] = dt.getHours();
            that.cfg.time[1] = dt.getMinutes();
            that.cfg.time[2] = dt.getSeconds();
            that.cfg.date[0] = dt.getFullYear();
            that.cfg.date[1] = dt.getMonth() + 1;
            that.cfg.date[2] = dt.getDate();
            this.qs("#manualtime").value = that.strTime;
            this.qs("#manualdate").value = that.strDate;
    });
    this.listen("#timezone", "change", e => {
      that.cfg.timezone = Number(e.target.value);
      that.wsSett({"timezone": e.target.value});
    });
    this.listen("#ntpbutton", "click", e => {
      this.wsRequest("getNTP");
    });
    this.listen("#manualtime", "input", e => {
        let t = e.target.value.split(":");
        that.cfg.time[0] = Number(t[0]);
        that.cfg.time[1] = Number(t[1]);
        that.cfg.time[2] = Number(t[2]);
      });
    this.listen("#manualdate", "input", e => {
        let t = e.target.value.split("-");
        that.cfg.date[0] = Number(t[0]);
        that.cfg.date[1] = Number(t[1]);
        that.cfg.date[2] = Number(t[2]);
      });
    // scan siti
    this.listen("#scanbutton", "click", e => {
      for(let opt of this.qs("#networks").querySelectorAll("option")){opt.remove();}
      let option = this.ce("option");
          option.setAttribute("disabled","");
          option.innerHTML = "Probíhá vyhledávání sítí";
          this.qs("#networks").appendChild(option);
          this.qs("#networks").classList.add("empty");
      this.scan.then(data => {
        this.networks = data;
        for(let opt of this.qs("#networks").querySelectorAll("option")){opt.remove();}
        for(let i in data)
          {
                option = this.ce("option");
                option.value = i;
                option.innerHTML = data[i][0];
            this.qs("#networks").appendChild(option);
          }
        if(!data.length)
          {
            option = this.ce("option");
            option.setAttribute("disabled");
            option.innerHTML = "Nebyla nalezena žádná síť!";
            this.qs("#networks").appendChild(option);
          }
        else
          {
            this.qs("#networks").classList.remove("empty");
          }
      });
    });
    // pripojit k wifi
    this.listen("#connect", "click", e => {
      let ssid = that.networks[Number(this.qs("#networks").value)][0],
          pwd = this.qs("#pwd").value;
          that.fetch(`connect?ssid=${ssid}&pwd=${pwd}`).
            then(data => {
              console.log(data);
            });
    });
    this.listen("#applyTime", "click", e => {
      that.wsSett({"time": that.strTime, "date": that.strDate}); // that.cfg.time[0]
    });

    this.listen("#defaults", "click", e => {
      if(confirm("Opravdu chcete provést tovární nastavení?"))
        {
          this.wsRequest("factoryReset");
        }
    });
    this.listen("#reboot", "click", e => {
      this.wsRequest("reboot");
    });
  }

  vykresli()
    {
      for(let p in this.pixely)
        {
          this.pixely[p] = "#000";
        }
      for(let p of this.activePixels)
        {
          this.pixely[parseInt(p.substring(0,2), 16)] = `#${p.substring(2)}`;
        }
      // canvas
      const width = 256,
            height = 256,
            canvas = document.getElementById("board"),
            ctx = canvas.getContext("2d");
       let x = 0,
           y = 0;
           ctx.clearRect(0, 0, canvas.width, canvas.height);
      for(let p in this.pixely)
        {
          if(this.pixely[p] !== "#000" && this.pixely[p] !== "#000000"){
            ctx.fillStyle = this.pixely[p];
            ctx.fillRect((width/11) * x, (height/11) * y, width / 11, height / 11);
          }
          x++;
          if(x > 10){
            x = 0;
            y++;
          }
        }
    }
}
)=====";
