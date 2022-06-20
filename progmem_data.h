#ifndef progmem_data_h
#define progmem_data_h

#include "Arduino.h"
//  ================================================================================================== LED MATRIX
#ifdef LED_MATRIX_ZIGZAG
const uint8_t matrix_map[121] PROGMEM = {  // zapojení do "Z"
                                     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
                                    11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
                                    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
                                    33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
                                    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
                                    55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,
                                    66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
                                    77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
                                    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
                                    99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
                                   110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
                                     
};
#else
const uint8_t matrix_map[121] PROGMEM = {  // zapojení do "HADA"
                                     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
                                    21,  20,  19,  18,  17,  16,  15,  14,  13,  12,  11,
                                    22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
                                    43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,
                                    44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
                                    65,  64,  63,  62,  61,  60,  59,  58,  57,  56,  55,
                                    66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
                                    87,  86,  85,  84,  83,  82,  81,  80,  79,  78,  77,
                                    88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
                                   109, 108, 107, 106, 105, 104, 103, 102, 101, 100,  99,
                                   110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
                                     
};
#endif
const char paleta[]  PROGMEM = "WEBUDOUBUDEJEJSOUJEDNADEVETPETDVESEDMDVANACTDESETRISESTOSMJEDENACTCTYRIADESETDVACETRICETPATNACTNULAPECTYRICETMPADESATEPET";
// ================================================================================================== HODINY
const uint8_t je[]      PROGMEM = {11, 12};                                               //  JE
const uint8_t jsou[]    PROGMEM = {13, 14, 15, 16};                                       //  JSOU
const uint8_t bude[]    PROGMEM = {7, 8, 9, 10};                                          //  BUDE
const uint8_t budou[]   PROGMEM = {2, 3, 4, 5, 6};                                        //  BUDOU

const uint8_t pole_hodin_sizes[] PROGMEM = {7, 5, 3, 3, 5, 3, 4, 4, 3, 5, 5, 8, 7};
const uint8_t hodin1[]  PROGMEM = {17, 18, 19, 20, 21};                                   //  JEDNA
const uint8_t hodin2[]  PROGMEM = {30, 31, 32};                                           //  DVĚ
const uint8_t hodin3[]  PROGMEM = {48, 49, 50};                                           //  TŘI
const uint8_t hodin4[]  PROGMEM = {66, 67, 68, 69, 70};                                   //  ČTYŘI
const uint8_t hodin5[]  PROGMEM = {27, 28, 29};                                           //  PĚT
const uint8_t hodin6[]  PROGMEM = {51, 52, 53, 54};                                       //  ŠEST
const uint8_t hodin7[]  PROGMEM = {33, 34, 35, 36};                                       //  SEDM
const uint8_t hodin8[]  PROGMEM = {55, 56, 57};                                           //  OSM
const uint8_t hodin9[]  PROGMEM = {22, 23, 24, 25, 26};                                   //  DEVĚT
const uint8_t hodin10[] PROGMEM = {44, 45, 46, 47, 48};                                   //  DESET
const uint8_t hodin11[] PROGMEM = {58, 59, 60, 61, 62, 63, 64, 65};                       //  JEDENÁCT
const uint8_t hodin12[] PROGMEM = {37, 38, 39, 40, 41, 42, 43};                           //  DVANÁCT
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
const uint8_t w_set[]      PROGMEM = {74, 75, 76};                                               //  SET
// ================================================================================================== Kalendář
const char d_po_long[]  PROGMEM = "Pondělí";
const char d_ut_long[]  PROGMEM = "Úterý";
const char d_st_long[]  PROGMEM = "Středa";
const char d_ct_long[]  PROGMEM = "Čtvrtek";
const char d_pa_long[]  PROGMEM = "Pátek";
const char d_so_long[]  PROGMEM = "Sobota";
const char d_ne_long[]  PROGMEM = "Neděle";
const char* dow_long[] PROGMEM =  {  d_po_long, d_ut_long, d_st_long, d_ct_long, d_pa_long, d_so_long, d_ne_long};
const char d_po_short[]  PROGMEM = "Po";
const char d_ut_short[]  PROGMEM = "Út";
const char d_st_short[]  PROGMEM = "St";
const char d_ct_short[]  PROGMEM = "Čt";
const char d_pa_short[]  PROGMEM = "Pá";
const char d_so_short[]  PROGMEM = "So";
const char d_ne_short[]  PROGMEM = "Ne";
const char* dow_short[] PROGMEM =  {  d_po_short, d_ut_short, d_st_short, d_ct_short, d_pa_short, d_so_short, d_ne_short};

// ================================================================================================== 5x7 bignumbers
/**
  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,
  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,
  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,
  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,
  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,
  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,
  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,
  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,
  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,
  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120
*/
const uint8_t bignum_sizes[] PROGMEM = {16, 12, 15, 14, 14, 17, 17, 11, 17, 17}; 
const uint8_t bignum_0[] PROGMEM = {23, 24, 25, 33, 37, 44, 48, 55, 59, 66, 70, 77, 81, 89, 90, 91};      //  [0, ]
const uint8_t bignum_1[] PROGMEM = {24, 34, 35, 46, 57, 68, 79, 88, 89, 90, 91, 92};      //  [1, ]
const uint8_t bignum_2[] PROGMEM = {23, 24, 25, 33, 37, 44, 48, 58, 68, 78, 88, 89, 90, 91, 92};      //  [2, ]
const uint8_t bignum_3[] PROGMEM = {23, 24, 25, 33, 37, 48, 57, 58, 70, 81, 77, 89, 90, 91};      //  [3, ]
const uint8_t bignum_4[] PROGMEM = {25, 35, 36, 45, 47, 55, 58, 66, 67, 68, 69, 70, 80, 91};      //  [4, ]
const uint8_t bignum_5[] PROGMEM = {22, 23, 24, 25, 26, 33, 44, 45, 46, 47, 59, 70, 77, 81, 89, 90, 91};      //  [5, ]
const uint8_t bignum_6[] PROGMEM = {23, 24, 25, 33, 37, 44, 55, 56, 57, 58, 66, 70, 77, 81, 89, 90, 91};      //  [6, ]
const uint8_t bignum_7[] PROGMEM = {22, 23, 24, 25, 26, 37, 47, 57, 68, 79, 90};      //  [7, ]
const uint8_t bignum_8[] PROGMEM = {23, 24, 25, 33, 37, 44, 48, 56, 57, 58, 66, 70, 77, 81, 89, 90, 91};      //  [8, ]
const uint8_t bignum_9[] PROGMEM = {23, 24, 25, 33, 37, 44, 48, 56, 57, 58, 59, 70, 77, 81, 89, 90, 91};      //  [9, ]
const uint8_t* pole_bignum[] PROGMEM =  {bignum_0, bignum_1, bignum_2, bignum_3, bignum_4, bignum_5, bignum_6, bignum_7, bignum_8, bignum_9};

/*
const pxCoor pxNum_0[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}, {0, 5}, {4, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxNum_1[] PROGMEM = {{2, 2}, {1, 3}, {2, 3}, {2, 4}, {2, 5}, {2, 6}, {2, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}};
const pxCoor pxNum_2[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}, {3, 5}, {2, 6}, {1, 7}, {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}};
const pxCoor pxNum_3[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {4, 4}, {2, 5}, {3, 5}, {4, 6}, {4, 7}, {0, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxNum_4[] PROGMEM = {{3, 2}, {2, 3}, {3, 3}, {1, 4}, {3, 4}, {0, 5}, {3, 5}, {0, 6}, {1, 6}, {2, 6}, {3, 6}, {4, 6}, {3, 7}, {3, 8}};
const pxCoor pxNum_5[] PROGMEM = {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {0, 3}, {0, 4}, {1, 4}, {2, 4}, {3, 4}, {4, 5}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxNum_6[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {0, 4}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxNum_7[] PROGMEM = {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {4, 3}, {3, 4}, {2, 5}, {2, 6}, {2, 7}, {2, 8}};
const pxCoor pxNum_8[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}, {1, 5}, {2, 5}, {3, 5}, {0, 6}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxNum_9[] PROGMEM = {{1, 2}, {2, 2}, {3, 2}, {0, 3}, {4, 3}, {0, 4}, {4, 4}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {4, 6}, {0, 7}, {4, 7}, {1, 8}, {2, 8}, {3, 8}};
const pxCoor pxLetter_A[] PROGMEM = {};
const pxCoor pxLetter_B[] PROGMEM = {};
const pxCoor pxLetter_C[] PROGMEM = {};
const pxCoor pxLetter_D[] PROGMEM = {};
const pxCoor pxLetter_E[] PROGMEM = {};
const pxCoor pxLetter_F[] PROGMEM = {};
const pxCoor pxLetter_G[] PROGMEM = {};
const pxCoor pxLetter_H[] PROGMEM = {};
const pxCoor pxLetter_I[] PROGMEM = {};
const pxCoor pxLetter_J[] PROGMEM = {};
const pxCoor pxLetter_K[] PROGMEM = {};
const pxCoor pxLetter_L[] PROGMEM = {};
const pxCoor pxLetter_M[] PROGMEM = {};
const pxCoor pxLetter_N[] PROGMEM = {};
const pxCoor pxLetter_O[] PROGMEM = {};
const pxCoor pxLetter_P[] PROGMEM = {};
const pxCoor pxLetter_Q[] PROGMEM = {};
const pxCoor pxLetter_R[] PROGMEM = {};
const pxCoor pxLetter_S[] PROGMEM = {};
const pxCoor pxLetter_T[] PROGMEM = {};
const pxCoor pxLetter_U[] PROGMEM = {};
const pxCoor pxLetter_V[] PROGMEM = {};
const pxCoor pxLetter_W[] PROGMEM = {};
const pxCoor pxLetter_X[] PROGMEM = {};
const pxCoor pxLetter_Y[] PROGMEM = {};
const pxCoor pxLetter_Z[] PROGMEM = {};

const pxChar pxChar_0 PROGMEM = {16, pxNum_0};
const pxChar pxChar_1 PROGMEM = {12, pxNum_1};
const pxChar pxChar_2 PROGMEM = {15, pxNum_2};
const pxChar pxChar_3 PROGMEM = {14, pxNum_3};
const pxChar pxChar_4 PROGMEM = {14, pxNum_4};
const pxChar pxChar_5 PROGMEM = {17, pxNum_5};
const pxChar pxChar_6 PROGMEM = {17, pxNum_6};
const pxChar pxChar_7 PROGMEM = {11, pxNum_7};
const pxChar pxChar_8 PROGMEM = {17, pxNum_8};
const pxChar pxChar_9 PROGMEM = {17, pxNum_9};

const pxChar pxChar_A PROGMEM = {0, pxLetter_A};
const pxChar pxChar_B PROGMEM = {0, pxLetter_B};
const pxChar pxChar_C PROGMEM = {0, pxLetter_C};
const pxChar pxChar_D PROGMEM = {0, pxLetter_D};
const pxChar pxChar_E PROGMEM = {0, pxLetter_E};
const pxChar pxChar_F PROGMEM = {0, pxLetter_F};
const pxChar pxChar_G PROGMEM = {0, pxLetter_G};
const pxChar pxChar_H PROGMEM = {0, pxLetter_H};
const pxChar pxChar_I PROGMEM = {0, pxLetter_I};
const pxChar pxChar_J PROGMEM = {0, pxLetter_J};
const pxChar pxChar_K PROGMEM = {0, pxLetter_K};
const pxChar pxChar_L PROGMEM = {0, pxLetter_L};
const pxChar pxChar_M PROGMEM = {0, pxLetter_M};
const pxChar pxChar_N PROGMEM = {0, pxLetter_N};
const pxChar pxChar_O PROGMEM = {0, pxLetter_O};
const pxChar pxChar_P PROGMEM = {0, pxLetter_P};
const pxChar pxChar_Q PROGMEM = {0, pxLetter_Q};
const pxChar pxChar_R PROGMEM = {0, pxLetter_R};
const pxChar pxChar_S PROGMEM = {0, pxLetter_S};
const pxChar pxChar_T PROGMEM = {0, pxLetter_T};
const pxChar pxChar_U PROGMEM = {0, pxLetter_U};
const pxChar pxChar_V PROGMEM = {0, pxLetter_V};
const pxChar pxChar_W PROGMEM = {0, pxLetter_W};
const pxChar pxChar_X PROGMEM = {0, pxLetter_X};
const pxChar pxChar_Y PROGMEM = {0, pxLetter_Y};
const pxChar pxChar_Z PROGMEM = {0, pxLetter_Z};


const pxChar pxChars[] PROGMEM = {
  pxChar_0, pxChar_1, pxChar_2, pxChar_3, pxChar_4, pxChar_5, pxChar_6, pxChar_7, pxChar_8, pxChar_9, 
  pxChar_A, pxChar_B, pxChar_C, pxChar_D, pxChar_E, pxChar_F, pxChar_G, pxChar_H, pxChar_I, pxChar_J,
  pxChar_K, pxChar_L, pxChar_M, pxChar_N, pxChar_O, pxChar_P, pxChar_Q, pxChar_R, pxChar_S, pxChar_T,
  pxChar_U, pxChar_V, pxChar_W, pxChar_X, pxChar_Y, pxChar_Z
};
*/

#endif
