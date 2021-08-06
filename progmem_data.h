#ifndef progmem_data_h
#define progmem_data_h

#include "Arduino.h"

const char paleta[]  PROGMEM = "WEBUDOUBUDEJEJSOUJEDNADEVETPETDVESEDMDVANACTDESETRISESTOSMJEDENACTCTYRIADESETDVACETRICETPATNACTNULAPECTYRICETMPADESATEPET";
// ================================================================================================== HODINY
const byte je[]      PROGMEM = {11, 12};                                               //  JE
const byte jsou[]    PROGMEM = {13, 14, 15, 16};                                       //  JSOU
const byte bude[]    PROGMEM = {7, 8, 9, 10};                                          //  BUDE
const byte budou[]   PROGMEM = {2, 3, 4, 5, 6};                                        //  BUDOU

const byte pole_hodin_sizes[] PROGMEM = {7, 5, 3, 3, 5, 3, 4, 4, 3, 5, 5, 8, 7};
const byte hodin1[]  PROGMEM = {17, 18, 19, 20, 21};                                   //  JEDNA
const byte hodin2[]  PROGMEM = {30, 31, 32};                                           //  DVĚ
const byte hodin3[]  PROGMEM = {48, 49, 50};                                           //  TŘI
const byte hodin4[]  PROGMEM = {66, 67, 68, 69, 70};                                   //  ČTYŘI
const byte hodin5[]  PROGMEM = {27, 28, 29};                                           //  PĚT
const byte hodin6[]  PROGMEM = {51, 52, 53, 54};                                       //  ŠEST
const byte hodin7[]  PROGMEM = {33, 34, 35, 36};                                       //  SEDM
const byte hodin8[]  PROGMEM = {55, 56, 57};                                           //  OSM
const byte hodin9[]  PROGMEM = {22, 23, 24, 25, 26};                                   //  DEVĚT
const byte hodin10[] PROGMEM = {44, 45, 46, 47, 48};                                   //  DESET
const byte hodin11[] PROGMEM = {58, 59, 60, 61, 62, 63, 64, 65};                       //  JEDENÁCT
const byte hodin12[] PROGMEM = {37, 38, 39, 40, 41, 42, 43};                           //  DVANÁCT
const byte* pole_hodin[] PROGMEM =  {
  hodin12,
  hodin1,  hodin2,  hodin3,  hodin4,  hodin5,  hodin6,
  hodin7,  hodin8,  hodin9,  hodin10,  hodin11,  hodin12,
};
const uint8_t pole_minut_sizes[] PROGMEM = {7, 5, 7, 6, 9, 6, 9, 8, 11, 7, 10};
const uint8_t minut05[] PROGMEM = {95, 96, 97, 98, 118, 119, 120};                         //  NULA PĚT
const uint8_t minut10[] PROGMEM = {72, 73, 74, 75, 76};                                    //  DESET
const uint8_t minut15[] PROGMEM = {88, 89, 90, 91, 92, 93, 94};                            //  PATNÁCT
const uint8_t minut20[] PROGMEM = {77, 78, 79, 80, 81, 82};                                //  DVACET
const uint8_t minut25[] PROGMEM = {77, 78, 79, 80, 81, 82, 118, 119, 120};                 //  DVACET PĚT
const uint8_t minut30[] PROGMEM = {82, 83, 84, 85, 86, 87};                                //  TŘICET
const uint8_t minut35[] PROGMEM = {82, 83, 84, 85, 86, 87, 118, 119, 120};                 //  TŘICET PĚT
const uint8_t minut40[] PROGMEM = {101, 102, 103, 104, 105, 106, 107, 108};                //  ČTYŘICET
const uint8_t minut45[] PROGMEM = {101, 102, 103, 104, 105, 106, 107, 108, 118, 119, 120}; //  ČTYŘICET PĚT
const uint8_t minut50[] PROGMEM = {110, 111, 112, 113, 114, 115, 116};                     //  PADESÁT
const uint8_t minut55[] PROGMEM = {110, 111, 112, 113, 114, 115, 116, 118, 119, 120};      //  PADESÁT PĚT

const uint8_t* pole_minut[] PROGMEM =  {
  minut05,  minut10,  minut15,  minut20,  minut25,  minut30,
  minut35,  minut40,  minut45,  minut50,  minut55
 };
// ================================================================================================== SET
const byte w_set[]      PROGMEM = {74, 75, 76};                                               //  SET


#endif
