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
    this.daynames = {
          "Adam": 1224,
          "Adéla": 902,
          "Adolf": 617,
          "Adrian": 626,
          "Adriana": 626,
          "Agáta": 1014,
          "Alan": 814,
          "Albert": 1121,
          "Albína": 1216,
          "Alena": 813,
          "Aleš": 413,
          "Alexandr": 227,
          "Alexandra": 421,
          "Alexej": 503,
          "Alice": 115,
          "Alina": 728,
          "Alois": 621,
          "Alžběta": 1119,
          "Amálie": 710,
          "Ambrož": 1207,
          "Anastázie": 415,
          "Anděla": 311,
          "Andrea": 926,
          "Andrej": 1011,
          "Aneta": 517,
          "Anežka": 302,
          "Anna": 726,
          "Anton": 613,
          "Antonie": 612,
          "Antonín": 613,
          "Apolena": 209,
          "Arnošt": 330,
          "Artur": 1126,
          "Augustýn": 828,
          "Barbara": 1204,
          "Barbora": 1204,
          "Bartoloměj": 824,
          "Bastian": 120,
          "Beáta": 1025,
          "Beatrice": 729,
          "Bedřich": 301,
          "Běla": 121,
          "Benedikt": 1112,
          "Benjamín": 1207,
          "Bernard": 820,
          "Bernd": 820,
          "Bernhard": 820,
          "Berta": 923,
          "Bibiana": 1202,
          "Blahoslav": 430,
          "Blanka": 1202,
          "Blažej": 203,
          "Blažena": 510,
          "Bohdan": 1109,
          "Bohdana": 111,
          "Bohumil": 1003,
          "Bohumila": 1228,
          "Bohumír": 1108,
          "Bohuslav": 822,
          "Bohuslava": 707,
          "Boleslav": 906,
          "Bonifác": 514,
          "Boris": 905,
          "Bořek": 712,
          "Bořivoj": 730,
          "Božena": 211,
          "Boží hod vánoční": 1225,
          "Brigita": 1021,
          "Bronislav": 903,
          "Bruno": 611,
          "Břetislav": 110,
          "Cecílie": 1122,
          "Ctibor": 509,
          "Ctirad": 116,
          "Čeněk": 719,
          "Čestmír": 108,
          "Dagmar": 1220,
          "Dalibor": 604,
          "Dalimil": 105,
          "Dan": 1217,
          "Dana": 1211,
          "Danica": 126,
          "Daniel": 1217,
          "Daniela": 909,
          "Danuše": 1211,
          "Darina": 922,
          "Darja": 410,
          "David": 1230,
          "Dávid": 1230,
          "Den Cyrila a Metoděje": 705,
          "Denisa": 911,
          "Den osvobození ČSR": 508,
          "Diana": 104,
          "Dita": 327,
          "Dobromila": 205,
          "Dobroslav": 605,
          "Dominik": 804,
          "Dominika": 804,
          "Dorota": 226,
          "Doubravka": 119,
          "Drahomíra": 718,
          "Drahoslav": 117,
          "Drahoslava": 709,
          "Dušan": 409,
          "Edita": 113,
          "Eduard": 318,
          "Elena": 316,
          "Eliška": 1005,
          "Ema": 408,
          "Emanuel": 326,
          "Emil": 522,
          "Emílie": 1124,
          "Erik": 1026,
          "Erika": 402,
          "Ester": 1219,
          "Eva": 1224,
          "Evelína": 829,
          "Evžen": 1110,
          "Evženie": 422,
          "Felix": 1101,
          "Ferdinand": 530,
          "Filip": 526,
          "František": 1004,
          "Františka": 309,
          "Gábina": 308,
          "Gabriel": 324,
          "Gabriela": 308,
          "Gita": 610,
          "Gizela": 218,
          "Gregor": 312,
          "Gustav": 802,
          "Hana": 815,
          "Hanka": 815,
          "Hanuš": 1006,
          "Havel": 1016,
          "Hedvika": 1017,
          "Helena": 818,
          "Herbert": 316,
          "Hermína": 407,
          "Heřman": 407,
          "Hubert": 1103,
          "Hugo": 401,
          "Hynek": 201,
          "Ida": 315,
          "Ignác": 731,
          "Igor": 1001,
          "Ilja": 720,
          "Ilona": 120,
          "Ingrid": 127,
          "Inka": 127,
          "Irena": 416,
          "Irma": 910,
          "Iva": 1201,
          "Ivan": 625,
          "Ivana": 404,
          "Iveta": 607,
          "Ivo": 519,
          "Ivona": 323,
          "Ivoš": 519,
          "Izabela": 411,
          "Jáchym": 816,
          "Jakub": 725,
          "Jan": 624,
          "Ján": 624,
          "Jana": 524,
          "Janka": 524,
          "Jarmil": 602,
          "Jarmila": 204,
          "Jaromír": 924,
          "Jaroslav": 427,
          "Jaroslava": 701,
          "Jeroným": 930,
          "Jindřich": 715,
          "Jindra": 904,
          "Jindřiška": 904,
          "Jiří": 424,
          "Jiřina": 215,
          "Jitka": 1205,
          "Jitřenka": 126,
          "Johana": 821,
          "Jolana": 915,
          "Jonáš": 927,
          "Josef": 319,
          "Jozef": 319,
          "Judita": 1229,
          "Julie": 1210,
          "Julius": 412,
          "Juraj": 424,
          "Justýna": 1007,
          "Kamil": 303,
          "Kamila": 531,
          "Karel": 1104,
          "Karina": 102,
          "Karolína": 714,
          "Katarína": 1125,
          "Kateřina": 1125,
          "Kazimir": 305,
          "Klára": 812,
          "Klaudie": 505,
          "Klement": 1123,
          "Kristian": 805,
          "Kristýna": 724,
          "Kryštof": 918,
          "Květa": 620,
          "Květoslav": 504,
          "Květoslava": 1208,
          "Kvido": 331,
          "Lada": 807,
          "Ladislav": 627,
          "Laura": 601,
          "Lena": 221,
          "Lenka": 221,
          "Leona": 322,
          "Leopold": 1115,
          "Leoš": 619,
          "Liběna": 1106,
          "Libor": 723,
          "Libuše": 710,
          "Lidmila": 916,
          "Liliana": 225,
          "Linda": 901,
          "Ljuba": 216,
          "Lubomír": 628,
          "Lubor": 913,
          "Luboš": 716,
          "Ľuboš": 924,
          "Lucia": 1213,
          "Lucie": 1213,
          "Luděk": 826,
          "Ludmila": 916,
          "Ľudovít": 825,
          "Ludvík": 819,
          "Lukáš": 1018,
          "Lumír": 228,
          "Lýdie": 1214,
          "Magda": 722,
          "Magdaléna": 722,
          "Mahulena": 1117,
          "Marcel": 1012,
          "Marcela": 420,
          "Marek": 425,
          "Marian": 325,
          "Marián": 325,
          "Mariana": 908,
          "Marie": 912,
          "Marika": 131,
          "Marina": 1010,
          "Markéta": 713,
          "Maroš": 1111,
          "Marta": 729,
          "Martin": 1111,
          "Martina": 717,
          "Matěj": 224,
          "Matouš": 921,
          "Matylda": 314,
          "Maxmilián": 529,
          "Medard": 608,
          "Michael": 929,
          "Michaela": 1019,
          "Michal": 929,
          "Michala": 1019,
          "Mikuláš": 1206,
          "Milada": 208,
          "Milan": 618,
          "Milena": 124,
          "Miloslav": 1218,
          "Miloslava": 217,
          "Miloš": 125,
          "Miluše": 803,
          "Miriam": 1105,
          "Mirka": 405,
          "Miroslav": 306,
          "Miroslava": 405,
          "Mojmír": 210,
          "Monika": 521,
          "Naďa": 917,
          "Naděžda": 917,
          "Natálie": 1221,
          "Nataša": 518,
          "Nela": 202,
          "Nikol": 1120,
          "Nikola": 1120,
          "Nina": 1024,
          "Nora": 708,
          "Norbert": 606,
          "Novýrok": 101,
          "Oldřich": 220,
          "Oldřiška": 806,
          "Oleg": 920,
          "Olga": 711,
          "Oliver": 1002,
          "Olivie": 1002,
          "Ondřej": 1130,
          "Oskar": 801,
          "Otakar": 827,
          "Otmar": 1116,
          "Oto": 426,
          "Otto": 426,
          "Otýlie": 128,
          "Památka zesnulých": 1102,
          "Pankrác": 512,
          "Patricie": 702,
          "Patrik": 219,
          "Pavel": 629,
          "Pavla": 622,
          "Pavlína": 831,
          "Pavol": 629,
          "Peter": 629,
          "Petr": 629,
          "Petra": 817,
          "Pravoslav": 112,
          "Prokop": 704,
          "Přemysl": 516,
          "Radan": 1215,
          "Radana": 1215,
          "Radek": 321,
          "Radim": 825,
          "Radka": 914,
          "Radmila": 103,
          "Radomír": 703,
          "Radomíra": 703,
          "Radoslav": 506,
          "Radovan": 114,
          "Radúz": 1210,
          "Rastislav": 113,
          "Regina": 907,
          "Regína": 907,
          "Renata": 1013,
          "Renáta": 1013,
          "René": 1128,
          "Richard": 403,
          "Robert": 429,
          "Robin": 130,
          "Roland": 614,
          "Roman": 809,
          "Romana": 1118,
          "Rostislav": 419,
          "Rudolf": 417,
          "Růt": 314,
          "Růžena": 313,
          "Řehoř": 312,
          "Sabina": 1022,
          "Samuel": 901,
          "Sandra": 823,
          "Sára": 1009,
          "Saskie": 1107,
          "Saša": 421,
          "Sáva": 1114,
          "Sebastian": 120,
          "Servác": 513,
          "Silvestr": 1231,
          "Silvia": 1029,
          "Silvie": 1029,
          "Simona": 1212,
          "Slávek": 122,
          "Slavěna": 212,
          "Slávka": 212,
          "Slavo": 122,
          "Slavoj": 607,
          "Slavomír": 122,
          "Soběslav": 808,
          "Soňa": 328,
          "Stanislav": 507,
          "Stanislava": 609,
          "Stela": 304,
          "Svatava": 511,
          "Svátek práce": 501,
          "Svatopluk": 223,
          "Svatoslav": 1203,
          "Světlana": 320,
          "Sylva": 1029,
          "Sylvie": 1029,
          "Šárka": 630,
          "Šarlota": 1027,
          "Šimon": 1222,
          "Šimona": 1212,
          "Štefan": 1009,
          "Štěpán": 1226,
          "Štěpánka": 1031,
          "Tadeáš": 1030,
          "Tamara": 603,
          "Tatiana": 329,
          "Taťána": 329,
          "Tea": 1023,
          "Teodor": 1023,
          "Tereza": 1015,
          "Terezie": 1015,
          "Tibor": 1113,
          "Tomáš": 307,
          "Třikrálové": 106,
          "Upálení mistra Jana Husa": 706,
          "Valdemar": 527,
          "Valentýn": 214,
          "Valérie": 418,
          "Vanda": 206,
          "Vavřinec": 810,
          "Vašek": 928,
          "Václav": 928,
          "Václava": 928,
          "Věnceslav": 213,
          "Vendelín": 1020,
          "Vendula": 406,
          "Věra": 1008,
          "Veronika": 207,
          "Věroslav": 727,
          "Viktor": 728,
          "Viktorie": 310,
          "Vilém": 528,
          "Vilma": 107,
          "Vincenc": 414,
          "Viola": 525,
          "Vít": 615,
          "Vítězslav": 721,
          "Vladan": 109,
          "Vladěna": 830,
          "Vladimír": 523,
          "Vladimíra": 523,
          "Vladislav": 118,
          "Vlaďka": 109,
          "Vlasta": 1223,
          "Vlastimil": 317,
          "Vlastislav": 428,
          "Vojta": 423,
          "Vojtěch": 423,
          "Vratislav": 1209,
          "Xenie": 1127,
          "Založení ČSR": 1028,
          "Zbyněk": 616,
          "Zbyšek": 520,
          "Zdeněk": 123,
          "Zdeňka": 623,
          "Zdislava": 129,
          "Zikmund": 502,
          "Zina": 1129,
          "Zita": 919,
          "Zlata": 925,
          "Zoe": 1027,
          "Zora": 126,
          "Zuzana": 811,
          "Žaneta": 1227,
          "Žofie": 515
      };
    this.holidays = {
      '01-01': 'Den obnovy samostatného českého státu',
      '05-01': 'Svátek práce',
      '05-08': 'Den vítězství',
      '07-05': 'Den slovanských věrozvěstů Cyrila a Metoděje',
      '07-06': 'Den upálení mistra Jana Husa',
      '09-28': 'Den české státnosti',
      '10-28': 'Den vzniku samostatného československého státu',
      '11-17': 'Den boje za svobodu a demokracii',
      '12-24': 'Štědrý den',
      '12-25': '1. svátek vánoční',
      '12-26': '2. svátek vánoční'
    };
    this.cfg = {
        wifi_mode: 1,
        version: 0,
        remote_version: 0,
        sta_ssid: "",
        time: [0,0,0],
        date: [2000,1,1],
        timezone: 2,
        main_color: [255,255,255],
        bg_color: [0,0,0],
        bright: 0,
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

  getEasterSunday (year, formatted = true) {
    const f = Math.floor,
          G = year%19,
          C = f(year/100),
          H = (C-f(C/4)-f((8*C+13)/25)+19*G+15)%30,
          I = H-f(H/28)*(1-f(29/(H+1))*f((21-G)/11)),
          J = (year+f(year/4)+I+2-C+f(C/4))%7,
          L = I-J,
          month = 3+f((L+40)/44),
          day = L+28-31*f(month/4);
    return !formatted ? [year,month,day] : `${year}-${month<10?`0${month}`:month}-${day<10?`0${day}`:day}`;
  }
  setHolidays(year) {
    let n = {};
    for(let dt in this.holidays){
      n[`${year}-${dt}`] = this.holidays[dt];
    }
    const sunday_dt = this.getEasterSunday(year);
    n[this.addDays(sunday_dt, -2)] = 'Velký pátek';
    n[this.addDays(sunday_dt, 1)] = 'Velikonoční pondělí';
    this.holidays = n;
  }
  loadDayInfo(){
    this.setHolidays(this.cfg.date[0]);
    let str = "";
    str += `${this.dowStr} ${this.cfg.date[2]}. ${this.monStr}`;
    if(this.holidays.hasOwnProperty(this.strDate))
      {
        str += `, slavíme ${this.holidays[this.strDate]}.`
      }
    else
      {
        let svatek = "";
        const dateid = (this.cfg.date[1] * 100) + this.cfg.date[2];
        for(let name_ in this.daynames)
          {
            if(this.daynames[name_] == dateid)
              {
                svatek = name_;
                break;
              }
          }
        str += `, svátek slaví ${svatek}.`
      }
    this.qs(".dayinfo").innerHTML = str;
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
  get dowStr(){
    const dt = new Date(this.strDate),
          da = ["Neděle", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota"];
    return da[dt.getDay()];
  }
  get monStr(){
    const dt = new Date(this.strDate),
          mo = ["Ledna", "Února", "Března", "Dubna", "Května", "Června", "Července", "Srpna", "Září", "Října", "Listopadu", "Prosince"];
    return mo[dt.getMonth()];
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
  cfgHtmlUpdate(){
    this.qs("#status").innerHTML    = this.wifiStatus;
    this.qs("#colorselect").value   = this.rgbHex(this.cfg.main_color);
    this.qs("#modeselect").value    = this.cfg.board_mode;
    this.qs("#manualtime").value    = this.strTime;
    this.qs("#manualdate").value    = this.strDate;
    this.qs("#timezone").value      = this.cfg.timezone;
    this.qs("#bright").value        = this.cfg.bright;
    this.qs("#schedule").checked    = this.cfg.schedule.enabled;
    this.qs("#schedulebrightf").value = this.cfg.schedule.from;
    this.qs("#schedulebrightt").value = this.cfg.schedule.to;
    this.qs("#schedulebright").value = this.cfg.schedule.bright;

    
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
    });
    this.listen("#modeselect", "change", e => {
      that.cfg.board_mode = Number(e.target.value);
      this.qs(".customimg").style.display = that.cfg.board_mode == 200 ? "block" : "none";
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
