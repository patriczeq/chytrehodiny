#include "mytime.h"
MYTIME::MYTIME() {}

MYTIME::~MYTIME() {}

void MYTIME::setup(datetimeformat d){
  this->d = d.d;
  this->t = d.t;
  this->tz = cfg.getTimeZone();
  this->timeClient.setTimeOffset(this->tz * 3600);
}
void MYTIME::update(){
  if(currentTime - updateAt > 1000)
    {
      updateAt = currentTime;
      this->addSec();
    }
}
uint8_t MYTIME::getTZ(){
  return this->tz;
}
void MYTIME::setTZ(uint8_t h){
  this->tz = h;
  this->timeClient.setTimeOffset(this->tz * 3600);
}
void MYTIME::GetNtpTime(byte ntz) {
  logger("NTP", "Synchronizuji cas z internetu");
    uint8_t i = 0;
    if(ntz < 200){
      this->timeClient.setTimeOffset(ntz * 3600);
    }
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
      logger("NTP", timeClient.getFormattedDate());
      this->setDateTime(timeClient.getFormattedDate());

      logger("NTP", "Synchronizováno");
    }
}

void MYTIME::addSec(){
  this->t.s++;
  if(this->t.s > 59)
    {
      this->t.s = 0;
      this->t.m++;
    }
   if(this->t.m > 59)
    {
      this->t.m = 0;
      this->t.h++;
    }
   if(this->t.h > 23){
      this->t.h = 0;
      this->addDay();
   }

   // DST - přepnutí na zimní čas

   if(this->getDow() == 6 && this->d.m == 10 && this->d.d >= 25 && this->t.h == 3 && this->DST){
    this->t.h = 2;
    this->DST = false;
   }

   // DST - přepnutí na letní čas
   if(this->getDow() == 6 && this->d.m == 3 && this->d.d >= 25 && this->t.h == 2 && !this->DST){
    this->t.h = 3;
    this->DST = true;
   }
}

datetimeformat MYTIME::getDateTimeDST(){
  datetimeformat dt = {this->d, this->t};
  uint8_t monDays[13] = {0, 31, !(dt.d.y % 4) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  if(!this->isDST())
    { // mínus 1 hodina
      if(dt.t.h >= 1)
        {
          dt.t.h -= 1;
        } 
      else
        {
          dt.t.h = 23;
          if(dt.d.d > 1)
            {
              dt.d.d -= 1;
            }
          else
            {
              if(dt.d.m > 1)
                {
                  dt.d.m -= 1;
                }
              else
                {
                  dt.d.y-= 1;
                  dt.d.m = 12;
                  dt.d.d = 31;
                }
            }
            
        }
    }
  return dt;
}

bool MYTIME::isDST(){
  dateformat startDST;
  dateformat endDST;
  bool startFound = false;
  bool endFound = false;
  for(uint8_t s = 25; s < 32; s++)
    {
      if(this->getDow(dateformat {this->d.y, 3, s}) == 6){
        startDST = {this->d.y, 3, s};
        startFound = true;
        break;
      }
    }
  for(uint8_t s = 25; s < 32; s++)
    {
      if(this->getDow(dateformat {this->d.y, 10, s}) == 6){
        endDST = {this->d.y, 10, s};
        endFound = true;
        break;
      }
    }
  return (!startFound || !endFound) ? false : (
        (this->d.m > 3 && this->d.m < 10)                                       // jsme mezi dubnem a zářím
        || (this->d.m == 3 && this->d.d >= startDST.d && this->t.h >= 2)      // poslední neděle v březnu ve 2 ráno - do konce měsíce
        || (this->d.m == 10 && this->d.d <= endDST.d && this->t.h < 3)       // poslední neděle v říjnu před 3. ráno - od začátku měsíce
        );
}
dateformat MYTIME::addDay(dateformat d){
  uint8_t monDays[13] = {0, 31, !(d.y % 4) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  d.d++;
  if(d.d > monDays[d.m])
    {
      d.d = 1;
      d.m++;
    }
   if(d.m > 12)
    {
      d.m = 1;
      d.y++;
    }
}
void MYTIME::addDay(){
  this->d = this->addDay(this->d);
}

String MYTIME::strNum(uint16_t num){
  return num > 9 ? String(num) : String("0") + String(num);
}
void MYTIME::setTime(timeformat t){
  this->t = t;
  logger("TIME", "Nastaven čas " + this->getTimeStr());
}
void MYTIME::setDate(dateformat d){
  this->d = d;
  logger("TIME", "Nastaveno datum " + this->getDateStr());
  logger("TIME-DST", this->isDST() ? "Letní čas" : "Zimní čas");
}
void MYTIME::setDateTime(datetimeformat d){
  this->setDate(d.d);
  this->setTime(d.t);
}
void MYTIME::setDateTime(String d){
  this->setDateTime(
    datetimeformat {
      dateformat {
        d.substring(0, 4).toInt(),
        d.substring(5, 7).toInt(),
        d.substring(8, 10).toInt(),
      },
      timeformat {
        d.substring(11, 13).toInt(),
        d.substring(14, 16).toInt(),
        d.substring(17, 19).toInt()
      }
    }
  );
}

timeformat MYTIME::getTime(){
  return this->getDateTimeDST().t;
}
dateformat MYTIME::getDate(){
  return this->getDateTimeDST().d;
}
datetimeformat MYTIME::getDateTime(){
  return this->getDateTimeDST();
}

String MYTIME::getTimeStr(){
  timeformat tt = this->getDateTimeDST().t;
  return this->strNum(tt.h) + ":" + this->strNum(tt.m) + ":" + this->strNum(tt.s);
}
String MYTIME::getDateStr(){
  dateformat dd = this->getDateTimeDST().d;
  return String(dd.y) + "-" + this->strNum(dd.m) + "-" + this->strNum(dd.d);
}
String MYTIME::getDateTimeStr(){
  return this->getDateStr() + " " + this->getTimeStr();
}



uint8_t MYTIME::getDow(bool startmonday){
  return this->getDow(this->d, startmonday);
}

uint8_t MYTIME::getDow(dateformat dd, bool startmonday){
  static uint8_t t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  dd.y -= dd.m < 3;
  uint8_t _day = ((dd.y + dd.y / 4 - dd.y / 100 + dd.y / 400 + t[dd.m - 1] + dd.d) % 7);
  return !startmonday ? _day : ( _day > 1 ? _day - 1 : 6);
}

String  MYTIME::svatek(){
  switch ((this->d.m * 100) + this->d.d) {
    default: return String();
    case 1224: return String(F("Adam, Eva"));
    case 902: return String(F("Adela"));
    case 617: return String(F("Adolf"));
    case 626: return String(F("Adrian, Adriana"));
    case 1014: return String(F("Agata"));
    case 814: return String(F("Alan"));
    case 1121: return String(F("Albert"));
    case 1216: return String(F("Albina"));
    case 813: return String(F("Alena"));
    case 413: return String(F("Ales"));
    case 227: return String(F("Alexandr"));
    case 421: return String(F("Alexandra, Sasa"));
    case 503: return String(F("Alexej"));
    case 115: return String(F("Alice"));
    case 728: return String(F("Alina, Viktor"));
    case 621: return String(F("Alois"));
    case 1119: return String(F("Alzbeta"));
    case 710: return String(F("Amalie, Libuse"));
    case 1207: return String(F("Ambroz, Benjamin"));
    case 415: return String(F("Anastazie"));
    case 311: return String(F("Andela"));
    case 926: return String(F("Andrea"));
    case 1011: return String(F("Andrej"));
    case 517: return String(F("Aneta"));
    case 302: return String(F("Anezka"));
    case 726: return String(F("Anna"));
    case 613: return String(F("Anton, Antonin"));
    case 612: return String(F("Antonie"));
    case 209: return String(F("Apolena"));
    case 330: return String(F("Arnost"));
    case 1126: return String(F("Artur"));
    case 828: return String(F("Augustyn"));
    case 1204: return String(F("Barbara, Barbora"));
    case 824: return String(F("Bartolomej"));
    case 120: return String(F("Bastian, Ilona, Sebastian"));
    case 1025: return String(F("Beata"));
    case 729: return String(F("Beatrice, Marta"));
    case 301: return String(F("Bedrich"));
    case 121: return String(F("Bela"));
    case 1112: return String(F("Benedikt"));
    case 820: return String(F("Bernard"));
    case 923: return String(F("Berta"));
    case 1202: return String(F("Bibiana, Blanka"));
    case 430: return String(F("Blahoslav"));
    case 203: return String(F("Blazej"));
    case 510: return String(F("Blazena"));
    case 1109: return String(F("Bohdan"));
    case 111: return String(F("Bohdana"));
    case 1003: return String(F("Bohumil"));
    case 1228: return String(F("Bohumila"));
    case 1108: return String(F("Bohumir"));
    case 822: return String(F("Bohuslav"));
    case 707: return String(F("Bohuslava"));
    case 906: return String(F("Boleslav"));
    case 514: return String(F("Bonifac"));
    case 905: return String(F("Boris"));
    case 712: return String(F("Borek"));
    case 730: return String(F("Borivoj"));
    case 211: return String(F("Bozena"));
    case 1225: return String(F("Bozi hod vanocni"));
    case 1021: return String(F("Brigita"));
    case 903: return String(F("Bronislav"));
    case 611: return String(F("Bruno"));
    case 110: return String(F("Bretislav"));
    case 1122: return String(F("Cecilie"));
    case 509: return String(F("Ctibor"));
    case 116: return String(F("Ctirad"));
    case 719: return String(F("Cenek"));
    case 108: return String(F("Cestmir"));
    case 1220: return String(F("Dagmar"));
    case 604: return String(F("Dalibor"));
    case 105: return String(F("Dalimil"));
    case 1217: return String(F("Dan, Daniel"));
    case 1211: return String(F("Dana, Danuse"));
    case 126: return String(F("Danica, Jitrenka, Zora"));
    case 909: return String(F("Daniela"));
    case 922: return String(F("Darina"));
    case 410: return String(F("Darja"));
    case 1230: return String(F("David, David"));
    case 705: return String(F("Den Cyrila a Metodeje"));
    case 911: return String(F("Denisa"));
    case 508: return String(F("Den osvobozeni CSR"));
    case 104: return String(F("Diana"));
    case 327: return String(F("Dita"));
    case 205: return String(F("Dobromila"));
    case 605: return String(F("Dobroslav"));
    case 804: return String(F("Dominik, Dominika"));
    case 226: return String(F("Dorota"));
    case 119: return String(F("Doubravka"));
    case 718: return String(F("Drahomira"));
    case 117: return String(F("Drahoslav"));
    case 709: return String(F("Drahoslava"));
    case 409: return String(F("Dusan"));
    case 113: return String(F("Edita, Rastislav"));
    case 318: return String(F("Eduard"));
    case 316: return String(F("Elena, Herbert"));
    case 1005: return String(F("Eliska"));
    case 408: return String(F("Ema"));
    case 326: return String(F("Emanuel"));
    case 522: return String(F("Emil"));
    case 1124: return String(F("Emilie"));
    case 1026: return String(F("Erik"));
    case 402: return String(F("Erika"));
    case 1219: return String(F("Ester"));
    case 829: return String(F("Evelina"));
    case 1110: return String(F("Evzen"));
    case 422: return String(F("Evzenie"));
    case 1101: return String(F("Felix"));
    case 530: return String(F("Ferdinand"));
    case 526: return String(F("Filip"));
    case 1004: return String(F("Frantisek"));
    case 309: return String(F("Frantiska"));
    case 308: return String(F("Gabina, Gabriela"));
    case 324: return String(F("Gabriel"));
    case 610: return String(F("Gita"));
    case 218: return String(F("Gizela"));
    case 312: return String(F("Gregor, Rehor"));
    case 802: return String(F("Gustav"));
    case 815: return String(F("Hana, Hanka"));
    case 1006: return String(F("Hanus"));
    case 1016: return String(F("Havel"));
    case 1017: return String(F("Hedvika"));
    case 818: return String(F("Helena"));
    case 407: return String(F("Hermina, Herman"));
    case 1103: return String(F("Hubert"));
    case 401: return String(F("Hugo"));
    case 201: return String(F("Hynek"));
    case 315: return String(F("Ida"));
    case 731: return String(F("Ignac"));
    case 1001: return String(F("Igor"));
    case 720: return String(F("Ilja"));
    case 127: return String(F("Ingrid, Inka"));
    case 416: return String(F("Irena"));
    case 910: return String(F("Irma"));
    case 1201: return String(F("Iva"));
    case 625: return String(F("Ivan"));
    case 404: return String(F("Ivana"));
    case 607: return String(F("Iveta, Slavoj"));
    case 519: return String(F("Ivo, Ivos"));
    case 323: return String(F("Ivona"));
    case 411: return String(F("Izabela"));
    case 816: return String(F("Jachym"));
    case 725: return String(F("Jakub"));
    case 624: return String(F("Jan"));
    case 524: return String(F("Jana, Janka"));
    case 602: return String(F("Jarmil"));
    case 204: return String(F("Jarmila"));
    case 924: return String(F("Jaromir, Lubos"));
    case 427: return String(F("Jaroslav"));
    case 701: return String(F("Jaroslava"));
    case 930: return String(F("Jeronym"));
    case 715: return String(F("Jindrich"));
    case 904: return String(F("Jindra, Jindriska"));
    case 424: return String(F("Jiri, Juraj"));
    case 215: return String(F("Jirina"));
    case 1205: return String(F("Jitka"));
    case 821: return String(F("Johana"));
    case 915: return String(F("Jolana"));
    case 927: return String(F("Jonas"));
    case 319: return String(F("Josef, Jozef"));
    case 1229: return String(F("Judita"));
    case 1210: return String(F("Julie, Raduz"));
    case 412: return String(F("Julius"));
    case 1007: return String(F("Justyna"));
    case 303: return String(F("Kamil"));
    case 531: return String(F("Kamila"));
    case 1104: return String(F("Karel"));
    case 102: return String(F("Karina"));
    case 714: return String(F("Karolina"));
    case 1125: return String(F("Katarina, Katerina"));
    case 305: return String(F("Kazimir"));
    case 812: return String(F("Klara"));
    case 505: return String(F("Klaudie"));
    case 1123: return String(F("Klement"));
    case 805: return String(F("Kristian"));
    case 724: return String(F("Kristyna"));
    case 918: return String(F("Krystof"));
    case 620: return String(F("Kveta"));
    case 504: return String(F("Kvetoslav"));
    case 1208: return String(F("Kvetoslava"));
    case 331: return String(F("Kvido"));
    case 807: return String(F("Lada"));
    case 627: return String(F("Ladislav"));
    case 601: return String(F("Laura"));
    case 221: return String(F("Lena, Lenka"));
    case 322: return String(F("Leona"));
    case 1115: return String(F("Leopold"));
    case 619: return String(F("Leos"));
    case 1106: return String(F("Libena"));
    case 723: return String(F("Libor"));
    case 916: return String(F("Lidmila, Ludmila"));
    case 225: return String(F("Liliana"));
    case 901: return String(F("Linda, Samuel"));
    case 216: return String(F("Ljuba"));
    case 628: return String(F("Lubomir"));
    case 913: return String(F("Lubor"));
    case 716: return String(F("Lubos"));
    case 1213: return String(F("Lucia, Lucie"));
    case 826: return String(F("Ludek"));
    case 825: return String(F("Ludovit, Radim"));
    case 819: return String(F("Ludvik"));
    case 1018: return String(F("Lukas"));
    case 228: return String(F("Lumir"));
    case 1214: return String(F("Lydie"));
    case 722: return String(F("Magda, Magdalena"));
    case 1117: return String(F("Mahulena"));
    case 1012: return String(F("Marcel"));
    case 420: return String(F("Marcela"));
    case 425: return String(F("Marek"));
    case 325: return String(F("Marian"));
    case 908: return String(F("Mariana"));
    case 912: return String(F("Marie"));
    case 131: return String(F("Marika"));
    case 1010: return String(F("Marina"));
    case 713: return String(F("Marketa"));
    case 1111: return String(F("Martin"));
    case 717: return String(F("Martina"));
    case 224: return String(F("Matej"));
    case 921: return String(F("Matous"));
    case 314: return String(F("Matylda, Rut"));
    case 529: return String(F("Maxmilian"));
    case 608: return String(F("Medard"));
    case 929: return String(F("Michael, Michal"));
    case 1019: return String(F("Michaela, Michala"));
    case 1206: return String(F("Mikulas"));
    case 208: return String(F("Milada"));
    case 618: return String(F("Milan"));
    case 124: return String(F("Milena"));
    case 1218: return String(F("Miloslav"));
    case 217: return String(F("Miloslava"));
    case 125: return String(F("Milos"));
    case 803: return String(F("Miluse"));
    case 1105: return String(F("Miriam"));
    case 405: return String(F("Mirka, Miroslava"));
    case 306: return String(F("Miroslav"));
    case 210: return String(F("Mojmir"));
    case 521: return String(F("Monika"));
    case 917: return String(F("Nada, Nadezda"));
    case 1221: return String(F("Natalie"));
    case 518: return String(F("Natasa"));
    case 202: return String(F("Nela"));
    case 1120: return String(F("Nikol, Nikola"));
    case 1024: return String(F("Nina"));
    case 708: return String(F("Nora"));
    case 606: return String(F("Norbert"));
    case 101: return String(F("Novy rok"));
    case 220: return String(F("Oldrich"));
    case 806: return String(F("Oldriska"));
    case 920: return String(F("Oleg"));
    case 711: return String(F("Olga"));
    case 1002: return String(F("Oliver, Olivie"));
    case 1130: return String(F("Ondrej"));
    case 801: return String(F("Oskar"));
    case 827: return String(F("Otakar"));
    case 1116: return String(F("Otmar"));
    case 426: return String(F("Oto, Otto"));
    case 128: return String(F("Otylie"));
    case 1102: return String(F("Pamatka zesnulych"));
    case 512: return String(F("Pankrac"));
    case 702: return String(F("Patricie"));
    case 219: return String(F("Patrik"));
    case 629: return String(F("Petr, Pavel"));
    case 622: return String(F("Pavla"));
    case 831: return String(F("Pavlina"));
    case 817: return String(F("Petra"));
    case 112: return String(F("Pravoslav"));
    case 704: return String(F("Prokop"));
    case 516: return String(F("Premysl"));
    case 1215: return String(F("Radan, Radana"));
    case 321: return String(F("Radek"));
    case 914: return String(F("Radka"));
    case 103: return String(F("Radmila"));
    case 703: return String(F("Radomir, Radomira"));
    case 506: return String(F("Radoslav"));
    case 114: return String(F("Radovan"));
    case 907: return String(F("Regina"));
    case 1013: return String(F("Renata"));
    case 1128: return String(F("Rene"));
    case 403: return String(F("Richard"));
    case 429: return String(F("Robert"));
    case 130: return String(F("Robin"));
    case 614: return String(F("Roland"));
    case 809: return String(F("Roman"));
    case 1118: return String(F("Romana"));
    case 419: return String(F("Rostislav"));
    case 417: return String(F("Rudolf"));
    case 313: return String(F("Ruzena"));
    case 1022: return String(F("Sabina"));
    case 823: return String(F("Sandra"));
    case 1009: return String(F("Sara, Stefan"));
    case 1107: return String(F("Saskie"));
    case 1114: return String(F("Sava"));
    case 513: return String(F("Servac"));
    case 1231: return String(F("Silvestr"));
    case 1029: return String(F("Silvia, Silvie, Sylva, Sylvie"));
    case 1212: return String(F("Simona, Simona"));
    case 122: return String(F("Slavek, Slavo, Slavomir"));
    case 212: return String(F("Slavena, Slavka"));
    case 808: return String(F("Sobeslav"));
    case 328: return String(F("Sona"));
    case 507: return String(F("Stanislav"));
    case 609: return String(F("Stanislava"));
    case 304: return String(F("Stela"));
    case 511: return String(F("Svatava"));
    case 501: return String(F("Svatek prace"));
    case 223: return String(F("Svatopluk"));
    case 1203: return String(F("Svatoslav"));
    case 320: return String(F("Svetlana"));
    case 630: return String(F("Sarka"));
    case 1027: return String(F("Sarlota, Zoe"));
    case 1222: return String(F("Simon"));
    case 1226: return String(F("Stepan"));
    case 1031: return String(F("Stepanka"));
    case 1030: return String(F("Tadeas"));
    case 603: return String(F("Tamara"));
    case 329: return String(F("Tatiana, Tatana"));
    case 1023: return String(F("Tea, Teodor"));
    case 1015: return String(F("Tereza, Terezie"));
    case 1113: return String(F("Tibor"));
    case 307: return String(F("Tomas"));
    case 106: return String(F("Trikralove"));
    case 706: return String(F("Upaleni mistra Jana Husa"));
    case 527: return String(F("Valdemar"));
    case 214: return String(F("Valentyn"));
    case 418: return String(F("Valerie"));
    case 206: return String(F("Vanda"));
    case 810: return String(F("Vavrinec"));
    case 928: return String(F("Vaclav, Vaclava"));
    case 213: return String(F("Venceslav"));
    case 1020: return String(F("Vendelin"));
    case 406: return String(F("Vendula"));
    case 1008: return String(F("Vera"));
    case 207: return String(F("Veronika"));
    case 727: return String(F("Veroslav"));
    case 310: return String(F("Viktorie"));
    case 528: return String(F("Vilem"));
    case 107: return String(F("Vilma"));
    case 414: return String(F("Vincenc"));
    case 525: return String(F("Viola"));
    case 615: return String(F("Vit"));
    case 721: return String(F("Vitezslav"));
    case 109: return String(F("Vladan, Vladka"));
    case 830: return String(F("Vladena"));
    case 523: return String(F("Vladimir, Vladimira"));
    case 118: return String(F("Vladislav"));
    case 1223: return String(F("Vlasta"));
    case 317: return String(F("Vlastimil"));
    case 428: return String(F("Vlastislav"));
    case 423: return String(F("Vojta, Vojtech"));
    case 1209: return String(F("Vratislav"));
    case 1127: return String(F("Xenie"));
    case 1028: return String(F("Zalozeni CSR"));
    case 616: return String(F("Zbynek"));
    case 520: return String(F("Zbysek"));
    case 123: return String(F("Zdenek"));
    case 623: return String(F("Zdenka"));
    case 129: return String(F("Zdislava"));
    case 502: return String(F("Zikmund"));
    case 1129: return String(F("Zina"));
    case 919: return String(F("Zita"));
    case 925: return String(F("Zlata"));
    case 811: return String(F("Zuzana"));
    case 1227: return String(F("Zaneta"));
    case 515: return String(F("Zofie"));
  }
}
