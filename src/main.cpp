//  ----------------------------------------------------------------------------
//  S H E L F   E D G E   C L O C K
//
//  Author:   CaptSnus
//  Date:     May 16, 2023
//  Update:   December 5, 2023
//  License:  MIT
//  ----------------------------------------------------------------------------
//
//  TABLE OF CONTENTS
//  - libraries, header files and board definitions
//  - pin assignments & global variables
//  - function declarations
//  - setup()
//  - loop()
//  - function definitions

//  ----------------------------------------------------------------------------
//  LIBRARIES, HEADER FILES & BOARD DEFINITIONS
//  ----------------------------------------------------------------------------

//  libraries
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <FastLED.h>
#include <NonBlockingRtttl.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <time.h>

//  ----------------------------------------------------------------------------
//  PIN ASSIGNMENTS & GLOBAL VARIABLES
//  ----------------------------------------------------------------------------

#define EDGE_PIN   36
#define EDGE_PPS   9
#define EDGE_COUNT 252
#define DOWN_PIN   35
#define DOWN_COUNT 14

// ==========   DEBUGGING   ====================================================
// For debugging purpose you may turn on additional serial output by changing
// the below parameter to true. Default: false
const bool DEBUG            = true;

//  general
const char projectTitle[]   = "S H E L F   E D G E   C L O C K";
const char projectVersion[] = "1.2.0";
const char projectAuthor[]  = "CaptSnus";
const char projectLink[]    = "https://github.com/CaptSnus/ShelfEdgeClock";
const char projectLicense[] = "MIT";

int displayMode;

TaskHandle_t   taskCore0;
Preferences    pref;
WiFiClient     wifi;
AsyncWebServer server( 80 );

//  time
struct tm ntpTime;                      // structure to save time
time_t    now;                          // this is the epoch
uint32_t  currMillisCore1   = millis(); // curr time core1
uint32_t  prevTimeSecCore1  = 0;        // prev time core1 (1 sec)
int       prevTimeSecCore0  = 0;        // prev time core0 (1 sec)
int       prevTimeMinCore0  = 0;        // prev time core0 (1 min)
int       prevTimeHourCore0 = 0;        // prev time core0 (1 hour)
int       prevTimeDayCore0  = 0;        // prev time core0 (1 day)
bool      edgeMinPassed     = true;     // color change (1 min)
bool      edgeHourPassed    = true;     // color change (1 hour)
bool      edgeDayPassed     = true;     // color change(1 day)
bool      downMinPassed     = true;     // color change (1 min)
bool      downHourPassed    = true;     // color change (1 hour)
bool      downDayPassed     = true;     // color change (1 day)

//  access point
bool      apConfig;
bool      apHide;                     // false = visible
String    apSSID, apPass;             // ssid & password
IPAddress apLocalIP;                  // ip address
int       apIP1, apIP2, apIP3, apIP4; // ...in blocks
IPAddress apGateway;                  // gateway address
int       apGW1, apGW2, apGW3, apGW4; // ...in blocks
IPAddress apSubnet;                   // subnet mask
int       apSN1, apSN2, apSN3, apSN4; // ...in blocks

// wifi client
bool      wifiConfig;
bool      wifiDHCP;                           // false = DHCP
String    wifiSSID, wifiPass;                 // ssid & password
IPAddress wifiLocalIP;                        // ip
int       wifiIP1, wifiIP2, wifiIP3, wifiIP4; // ...in blocks
IPAddress wifiGateway;                        // gateway
int       wifiGW1, wifiGW2, wifiGW3, wifiGW4; // ...in blocks
IPAddress wifiSubnet;                         // subnet
int       wifiSN1, wifiSN2, wifiSN3, wifiSN4; // ...in blocks

//  led
//  |- edge
CRGBArray<EDGE_COUNT> arrayEdge;
CRGBSet               segA( arrayEdge( EDGE_PPS * 0, EDGE_PPS - 1 + ( EDGE_PPS * 0 ) ) );
CRGBSet               segB( arrayEdge( EDGE_PPS * 1, EDGE_PPS - 1 + ( EDGE_PPS * 1 ) ) );
CRGBSet               segC( arrayEdge( EDGE_PPS * 2, EDGE_PPS - 1 + ( EDGE_PPS * 2 ) ) );
CRGBSet               segD( arrayEdge( EDGE_PPS * 3, EDGE_PPS - 1 + ( EDGE_PPS * 3 ) ) );
CRGBSet               segE( arrayEdge( EDGE_PPS * 4, EDGE_PPS - 1 + ( EDGE_PPS * 4 ) ) );
CRGBSet               segF( arrayEdge( EDGE_PPS * 5, EDGE_PPS - 1 + ( EDGE_PPS * 5 ) ) );
CRGBSet               segG( arrayEdge( EDGE_PPS * 6, EDGE_PPS - 1 + ( EDGE_PPS * 6 ) ) );
//  |- downlights
CRGBArray<DOWN_COUNT> arrayDown;

//  settings - globale einstellungen
int edgeBrightness = 0;            // 0 = auto, > 0 = manual
int edgeBrightnessMin;             // minimum value for edgeBrightnessOut
int edgeBrightnessMax;             // maximum value for edgeBrightnessOut
int edgeBrightnessOut;             // output to FastLED for edge lights
int edgeFrequency  = 0;            // 0 = sec, 1 = min, 2 = hour, 3 = day
int downUsage      = 0;            // 0 = off, 1 = on
int downBrightness = 0;            // 0 = auto, > 0 = manual
int downBrightnessMin;             // minimum value for downBrightnessOut
int downBrightnessMax;             // maximum value for downBrightnessOut
int downBrightnessOut;             // output to FastLED for downlights
int downFrequency             = 0; // 0 = sec, 1 = min, 2 = hour, 3 = day

// settings - knx
String     knxAddress         = "";   // ip address
int        knxPort            = 8080; // port number
String     knxBrightnessPath  = "";   // path to brightness item
long       knxBrightnessValue = 0;
long       knxBrightnessMin;
long       knxBrightnessMax;
String     knxTemperaturePath  = ""; // temperature item
double     knxTemperatureValue = 0;
String     knxHumidityPath     = ""; // humidity item
int        knxHumidityValue    = 0;
HttpClient knxClient           = HttpClient( wifi, knxAddress.c_str(), knxPort );

// settings - mode clock
String clkAddress              = "ch.pool.ntp.org";
String clkTimeZone             = "CET-1CEST,M3.5.0,M10.5.0/3";
int    clkFormat               = 1; // 0 = AM/PM, 1 = 24h
int    clkColor                = 0; // 0 = 2 def, 1 = 2 random
int    clkColorHR, clkColorHG, clkColorHB;
int    clkColorMR, clkColorMG, clkColorMB;

// settings - mode date
int datFormat = 1; // 0 = MMdd, 1 = ddMM
int datColor  = 0; // 0 = 2 def, 1 = 2 random
int datColorDR, datColorDG, datColorDB;
int datColorMR, datColorMG, datColorMB;

// settings - mode temperature
int temSymbol = 1; // 0 = Fahrenheit, 1 = Celsius

// settings - mode humidity
int humColor  = 0; // 0 = 3 def, 1 = 3 random
int humColorAR, humColorAG, humColorAB;
int humColorBR, humColorBG, humColorBB;
int humColorCR, humColorCG, humColorCB;

// settings - mode scoreboard
int scoLValue = 0;
int scoRValue = 0;
int scoColorLR, scoColorLG, scoColorLB;
int scoColorRR, scoColorRG, scoColorRB;

// settings - mode countdown
uint32_t couMillis    = 0;
uint32_t couMillisEnd = 0;
int      couColorHR, couColorHG, couColorHB;
int      couColorMR, couColorMG, couColorMB;
int      couColorSR, couColorSG, couColorSB;
int      couColorAR, couColorAG, couColorAB;

// settings - mode scroll
uint32_t scrMillis      = 0;
uint32_t scrMillisPrev  = 0;
int      scrDisplayMode = 0;
int      scrTime;

// downlights
int dowColor;
int dowColorR, dowColorG, dowColorB;
int dowColor0R, dowColor0G, dowColor0B;
int dowColor1R, dowColor1G, dowColor1B;
int dowColor2R, dowColor2G, dowColor2B;
int dowColor3R, dowColor3G, dowColor3B;
int dowColor4R, dowColor4G, dowColor4B;
int dowColor5R, dowColor5G, dowColor5B;
int dowColor6R, dowColor6G, dowColor6B;
int dowColor7R, dowColor7G, dowColor7B;
int dowColor8R, dowColor8G, dowColor8B;
int dowColor9R, dowColor9G, dowColor9B;
int dowColor10R, dowColor10G, dowColor10B;
int dowColor11R, dowColor11G, dowColor11B;
int dowColor12R, dowColor12G, dowColor12B;
int dowColor13R, dowColor13G, dowColor13B;

//  ----------------------------------------------------------------------------
//  FUNCTION DECLARATIONS
//  ----------------------------------------------------------------------------

void initPREFERENCES();                           // setup - initialize preferences and load values
void initWIFI();                                  // setup - configure wifi connections
void initSPIFFS();                                // setup - initialize SPIFFS and load data
void initNTP();                                   // setup - configure NTP and load current time
void initHANDLERS();                              // setup - load all web handlers
void initSERVER();                                // setup - configure and start webserver
void initLED();                                   // setup - define and initialize all led strands

void getAP();                                     // loop - restart the access point
void getWIFI();                                   // loop - reconnect the wifi client
void getNTP();                                    // loop - update current time
void getKNXbrightness();                          // loop - get the brightness value
void getKNXtemperature();                         // loop - get the temperature value
void getKNXhumidity();                            // loop - get the humidity value

void modeClk();                                   // display - mode CLOCK
void modeDat();                                   // display - mode DATE
void modeTem();                                   // display - mode TEMPERATURE
void modeHum();                                   // display - mode HUMIDITY
void modeSco();                                   // display - mode SCOREBOARD
void modeCou();                                   // display - mode COUNTDOWN
void modeScr();                                   // display - mode SCROLLING

void displayEdge( uint8_t d, uint8_t o, CHSV c ); // outout - edge
void displayDown();                               // output - downlights

void taskCore0code( void *pvParameters );         // task on core0

//  ----------------------------------------------------------------------------
//  SETUP
//  ----------------------------------------------------------------------------

void setup() {
  if ( DEBUG ) {
    Serial.begin( 115200 );                              // initialize serial
    delay( 5000 );                                       // wait a moment
    Serial.println( "\n\n" );                            // make some space
    Serial.printf( "PROJECT     %s\n", projectTitle );   // project title
    Serial.printf( "Version     %s\n", projectVersion ); // version
    Serial.printf( "Author      %s\n", projectAuthor );  // author
    Serial.printf( "Info        %s\n", projectLink );    // link
    Serial.printf( "License     %s\n", projectLicense ); // license
    Serial.println( "\nSETUP" );
  }

  initPREFERENCES();       // load all saved settings
  initWIFI();              // configure wifi
  initSPIFFS();            // initialize spiffs
  initNTP();               // get the time from ntp
  initHANDLERS();          // load all web handlers
  initSERVER();            // configure webserver
  initLED();               // init neopixels

  xTaskCreatePinnedToCore( // task running on core 0
    taskCore0code,         // function to implement the task
    "taskCore0",           // name of the task
    10000,                 // stack size in words
    NULL,                  // task input parameter
    0,                     // priority of the task
    &taskCore0,            // task handle
    0 );                   // core where the task should run
}

//  ----------------------------------------------------------------------------
//  LOOP
//  ----------------------------------------------------------------------------

void loop() {
  currMillisCore1 = millis();                             // update reference time
  ElegantOTA.loop();                                      // reboot after update

  if ( ( currMillisCore1 - prevTimeSecCore1 ) >= 1000 ) { // inside here every second
    prevTimeSecCore1 = currMillisCore1;                   // update previous reference time

    time( &now );                                         // read the current time
    localtime_r( &now, &ntpTime );                        // update ntpTime with the current time

    if ( displayMode == 0 ) {
      modeClk(); // call clock mode
    }
    if ( displayMode == 1 ) {
      modeDat(); // call date mode
    }
    if ( displayMode == 2 ) {
      modeTem(); // call temperature mode
    }
    if ( displayMode == 3 ) {
      modeHum(); // call humidity mode
    }
    if ( displayMode == 4 ) {
      modeSco(); // call scoreboard mode
    }
    if ( displayMode == 5 ) {
      modeCou(); // call countdown mode
    }
    if ( displayMode == 6 ) {
      modeScr(); // call scroll mode
    }

    if ( edgeBrightness == 0 ) {
      FastLED[ 0 ].showLeds( edgeBrightnessOut );
    } else {
      FastLED[ 0 ].showLeds( edgeBrightness );
    }
  }
}

//  ----------------------------------------------------------------------------
//  FUNCTION DEFINITIONS
//  ----------------------------------------------------------------------------

/**
 * @brief display mode - CLOCK
 * @details display the current time on the front of the shelf
 * @todo optimize and streamline the code
 * @todo check if CHSV clkHColor, clkMColor must be global
 * @since 1.0.0
 */
void modeClk() {
  // variables
  int  hour = ntpTime.tm_hour; // value of hours
  int  mins = ntpTime.tm_min;  // value of minutes
  CHSV clkHColor;              // color for hours
  CHSV clkMColor;              // color for minutes

  // adjust hours based on time format
  if ( !clkFormat ) {
    if ( hour > 12 ) {  // if hours > 12 ...
      hour = hour - 12; // ... turn 13:mm to 01:mm PM
    }
    if ( hour < 1 ) {   // if hours = 00 ...
      hour = hour + 12; // ... turn 00:mm to 12:mm AM
    }
  }

  // build digits
  int h1 = floor( hour / 10 ); // build h1
  int h2 = hour % 10;          // build h2
  int m1 = floor( mins / 10 ); // build m1
  int m2 = mins % 10;          // build m2

  // color settings
  // |- 2 user defined colors
  if ( clkColor == 0 ) {
    clkHColor = rgb2hsv_approximate( CRGB( clkColorHR, clkColorHG, clkColorHB ) );
    clkMColor = rgb2hsv_approximate( CRGB( clkColorMR, clkColorMG, clkColorMB ) );
  }
  // |- 2 random colors
  if ( clkColor == 1 && ( ( edgeFrequency == 0 ) ||                     // every sec
                          ( edgeFrequency == 1 && edgeMinPassed ) ||    // every min
                          ( edgeFrequency == 2 && edgeHourPassed ) ||   // every hour
                          ( edgeFrequency == 3 && edgeDayPassed ) ) ) { // every day
    clkHColor = CHSV( random8(), 255, 255 );
    clkMColor = CHSV( random8(), 255, 255 );
  }

  // display digits
  displayEdge( h1, 3, clkHColor ); // show first digit
  displayEdge( h2, 2, clkHColor ); // show second digit
  displayEdge( m1, 1, clkMColor ); // show third digit
  displayEdge( m2, 0, clkMColor ); // show fourth digit

  // clear flags
  edgeMinPassed  = false; // reset (1 min)
  edgeHourPassed = false; // reset (1 hour)
  edgeDayPassed  = false; // reset (1 day)
}

/**
 * @brief display mode - DATE
 * @details display the current date on the front of the shelf
 * @since 1.0.0
 */
void modeDat() {
  // variables
  CHSV datDCol; //
  CHSV datMCol; //

  // build digits
  int d1 = floor( ntpTime.tm_mday / 10 );        // build d1
  int d2 = ntpTime.tm_mday % 10;                 // build d2
  int m1 = floor( ( ntpTime.tm_mon + 1 ) / 10 ); // build m1
  int m2 = ( ntpTime.tm_mon + 1 ) % 10;          // build m2

  // color settings
  // |- 2 user defined colors
  if ( datColor == 0 ) {
    datDCol = rgb2hsv_approximate( CRGB( datColorDR, datColorDG, datColorDB ) );
    datMCol = rgb2hsv_approximate( CRGB( datColorMR, datColorMG, datColorMB ) );
  }
  // |- 2 random colors
  if ( datColor == 1 && ( ( edgeFrequency == 0 ) ||                     // every sec
                          ( edgeFrequency == 1 && edgeMinPassed ) ||    // every min
                          ( edgeFrequency == 2 && edgeHourPassed ) ||   // every hour
                          ( edgeFrequency == 3 && edgeDayPassed ) ) ) { // every day
    datDCol = CHSV( random8(), 255, 255 );
    datMCol = CHSV( random8(), 255, 255 );
  }

  // display digits
  if ( datFormat ) { // DDMM
    displayEdge( d1, 3, datDCol );
    displayEdge( d2, 2, datDCol );
    displayEdge( m1, 1, datMCol );
    displayEdge( m2, 0, datMCol );
  } else { // MMDD
    displayEdge( m1, 3, datMCol );
    displayEdge( m2, 2, datMCol );
    displayEdge( d1, 1, datDCol );
    displayEdge( d2, 0, datDCol );
  }

  // clear flags
  edgeMinPassed  = false; // reset (1 min)
  edgeHourPassed = false; // reset (1 hour)
  edgeDayPassed  = false; // reset (1 day)
}

/**
 * @brief TEMPERATURE mode
 * @details show the current temperature on the front of the shelf
 * @since 1.0.0
 */
void modeTem() {
  CHSV temColor;

  // build digits
  int temp = round( knxTemperatureValue ); // round
  int t1, t2, t3;                          // throwaway variable
  // |- Fahrenheit
  if ( temSymbol == 0 ) {
    if ( temp < 0 ) {
      t1 = 37;                  // show "-"
    } else {
      t1 = floor( temp / 100 ); // build t1
    }

    t2 = ( temp / 10 ) % 10; // build t2
    t3 = temp % 10;          // build t3

    if ( temp < 32 ) {
      CRGB temColor( 74, 88, 226 );
    }
    if ( temp >= 32 && temp <= 64 ) {
      CRGB temColor( 69, 223, 208 );
    }
    if ( temp > 64 && temp <= 75 ) {
      CRGB temColor( 63, 227, 93 );
    }
    if ( temp > 75 && temp <= 86 ) {
      CRGB temColor( 216, 228, 55 );
    }
    if ( temp > 86 && temp <= 97 ) {
      CRGB temColor( 231, 169, 49 );
    }
    if ( temp > 97 ) {
      CRGB temColor( 233, 36, 32 );
    }
  }
  // |- Celsius
  if ( temSymbol == 1 ) {
    if ( temp <= ( -10 ) ) {                   // below -10°C
      t1 = 10;                                 // show "-"
      t2 = floor( abs( temp ) / 10 );          // build t2
      t3 = abs( temp ) % 10;                   // build t3
    } else if ( temp > ( -10 ) && temp < 0 ) { // above -10°C and below 0°C
      t1 = 0;                                  // set to 0 (hidden)
      t2 = 10;                                 // show "-"
      t3 = abs( temp ) % 10;                   // build t3
    } else {                                   // above 0°C
      t1 = floor( temp / 100 );                // build t1
      t2 = floor( temp / 10 );                 // build t2
      t3 = temp % 10;                          // build t3
    }
    // color settings
    // |- below 0
    if ( temp < 0 ) {
      temColor = CHSV( 160, 255, 255 );
    }
    // |- between 0 and 18
    if ( temp >= 0 && temp <= 18 ) {
      temColor = CHSV( 128, 255, 255 );
    }
    // |- between 18 and 24
    if ( temp > 18 && temp <= 24 ) {
      temColor = CHSV( 96, 255, 255 );
    }
    // |- between 24 and 30
    if ( temp > 24 && temp <= 30 ) {
      temColor = CHSV( 64, 255, 255 );
    }
    // |- between 30 and 36
    if ( temp > 30 && temp <= 36 ) {
      temColor = CHSV( 32, 255, 255 );
    }
    // |- above 36
    if ( temp > 36 ) {
      temColor = CHSV( 0, 255, 255 );
    }
  }

  // display the digits
  if ( t1 != 0 ) {
    displayEdge( t1, 3, temColor );
  } else {
    displayEdge( t1, 3, CHSV( 0, 0, 0 ) );
  }
  if ( t2 != 0 ) {
    displayEdge( t2, 2, temColor );
  } else {
    displayEdge( t2, 2, CHSV( 0, 0, 0 ) );
  }
  displayEdge( t3, 1, temColor );
  displayEdge( 11, 0, temColor );
}

/**
 * @brief HUMIDITY mode
 * @details show the current humidity on the front of the shelf
 * @since 1.0.0
 */
void modeHum() {
  CHSV humAColor;
  CHSV humBColor;
  CHSV humCColor;

  // build digits
  int h1 = floor( knxHumidityValue / 100 ); // build h1
  int h2 = ( knxHumidityValue / 10 ) % 10;  // build h2
  int h3 = knxHumidityValue % 10;           // build h3

  // color settings
  // |- 3 user defined colors
  if ( humColor == 0 ) {
    humAColor = rgb2hsv_approximate( CRGB( humColorAR, humColorAG, humColorAB ) );
    humBColor = rgb2hsv_approximate( CRGB( humColorBR, humColorBG, humColorBB ) );
    humCColor = rgb2hsv_approximate( CRGB( humColorCR, humColorCG, humColorCB ) );
  }
  // |- 3 random colors
  if ( humColor == 1 && ( ( edgeFrequency == 0 ) ||                     // every second
                          ( edgeFrequency == 1 && edgeMinPassed ) ||    // every minute
                          ( edgeFrequency == 2 && edgeHourPassed ) ||   // every hour
                          ( edgeFrequency == 3 && edgeDayPassed ) ) ) { // every day
    humAColor = CHSV( random8(), 255, 255 );
    humBColor = CHSV( random8(), 255, 255 );
    humCColor = CHSV( random8(), 255, 255 );
  }

  // display the digits
  displayEdge( h1, 3, humAColor ); // show first digit
  displayEdge( h2, 2, humAColor ); // show second digit
  displayEdge( h3, 1, humBColor ); // show third digit
  displayEdge( 14, 0, humCColor ); // show icon

  // clear flags
  edgeMinPassed  = false; // reset (1 min)
  edgeHourPassed = false; // reset (1 hour)
  edgeDayPassed  = false; // reset (1 day)
}

/**
 * @brief MODE - scoreboard
 * @details show a scoreboard on the front of the shelf
 * @since 1.0.0
 */
void modeSco() {
  CHSV scoHColor;
  CHSV scoAColor;

  // build the digits
  int s1    = floor( scoLValue / 10 ); // build s1
  int s2    = scoLValue % 10;          // build s2
  int s3    = floor( scoRValue / 10 ); // build s3
  int s4    = scoRValue % 10;          // build s4

  // set the colors
  scoHColor = rgb2hsv_approximate( CRGB( scoColorLR, scoColorLG, scoColorLB ) );
  scoAColor = rgb2hsv_approximate( CRGB( scoColorRR, scoColorRG, scoColorRB ) );

  // display the digits
  displayEdge( s1, 3, scoHColor ); // show first digit
  displayEdge( s2, 2, scoHColor ); // show second digit
  displayEdge( s3, 1, scoAColor ); // show third digit
  displayEdge( s4, 0, scoAColor ); // show fourth digit
}

/**
 * @brief MODE - countdown
 * @details show a countdown on the front of the shelf
 * @todo "EndE" not correctly displayed
 * @since 1.0.0
 */
void modeCou() {
  CHSV couHColor;
  CHSV couMColor;
  CHSV couSColor;
  CHSV couAColor;

  if ( couMillis == 0 ) {        // abort empty countdown
    if ( edgeBrightness == 0 ) { // value = 0 -> automatic mode
      FastLED[ 0 ].showLeds( edgeBrightnessOut );
    } else {
      FastLED[ 0 ].showLeds( edgeBrightness / 2 );
    }
    displayMode = 0;
    pref.putInt( "displayMode", 0 );
    return;
  }

  // build the digits
  uint32_t restMillis = couMillisEnd - millis();             // continously build the remaining milliseconds and ...
  uint32_t hours      = ( ( restMillis / 1000 ) / 60 ) / 60; // ... create the countdown in hours
  uint32_t minutes    = ( restMillis / 1000 ) / 60;          // ... create the countdown in minutes
  uint32_t seconds    = restMillis / 1000;                   // ... create the countdown in seconds
  int      remM       = minutes - ( hours * 60 );            // build the minutes between 0 and 59
  int      remS       = seconds - ( minutes * 60 );          // build the seconds between 0 and 59

  int h1              = hours / 10;                          // build h1
  int h2              = hours % 10;                          // build h2
  int m1              = remM / 10;                           // build m1
  int m2              = remM % 10;                           // build m2
  int s1              = remS / 10;                           // build s1
  int s2              = remS % 10;                           // build s2

  // set the colors
  if ( restMillis > 10000 ) { // three defined colors
    couHColor = rgb2hsv_approximate( CRGB( couColorHR, couColorHG, couColorHB ) );
    couMColor = rgb2hsv_approximate( CRGB( couColorMR, couColorMG, couColorMB ) );
    couSColor = rgb2hsv_approximate( CRGB( couColorSR, couColorSG, couColorSB ) );
  } else { // change color for the last 10 seconds
    couMColor = rgb2hsv_approximate( CRGB( couColorAR, couColorAG, couColorAB ) );
    couSColor = rgb2hsv_approximate( CRGB( couColorAR, couColorAG, couColorAB ) );
  }

  // display the digits
  if ( hours > 0 ) {                 // if more than 60 minutes remaining on the countdown
    displayEdge( h1, 3, couHColor ); // show hours and minutes
    displayEdge( h2, 2, couHColor );
    displayEdge( m1, 1, couMColor );
    displayEdge( m2, 0, couMColor );
  } else {                           // if less than 60 minutes remaining on the countdown
    displayEdge( m1, 3, couMColor ); // show minutes and seconds
    displayEdge( m2, 2, couMColor );
    displayEdge( s1, 1, couSColor );
    displayEdge( s2, 0, couSColor );
  }

  // display "EndE"
  if ( hours <= 0 && remM <= 0 && remS <= 0 ) {
    //  ledClock.setBrightness( 150 );
    couAColor = rgb2hsv_approximate( CRGB( couColorAR, couColorAG, couColorAB ) );
    for ( int i = 0; i < 5; i++ ) {
      displayEdge( 13, 3, couAColor );
      displayEdge( 15, 2, couAColor );
      displayEdge( 12, 1, couAColor );
      displayEdge( 13, 0, couAColor );
      FastLED[ 1 ].showLeds( 150 );
      delay( 500 );
      //      fill_solid( arrayEdge, EDGE_COUNT, CRGB::Black );
      FastLED[ 1 ].showLeds( 0 );
      delay( 300 );
    }
    //  ledClock.clear();
    delay( 5000 );
    if ( edgeBrightness == 0 ) { // value = 0 -> automatic mode
                                 //    ledClock.setBrightness( briEdge );
    } else {
      //    ledClock.setBrightness( edgeBrightness / 2 );
    }
    displayMode = 0;
    pref.putInt( "displayMode", 0 );
    couMillis    = 0;
    couMillisEnd = 0;
    return;
  }
}

/**
 * @brief MODE - scrolling
 * @details show multiple values on the front of the shelf
 * @since 1.0.0
 */
void modeScr() {
  scrMillis = millis();                                        // update current reference time

  if ( ( scrMillis - scrMillisPrev ) >= ( scrTime * 1000 ) ) { // run everything inside here every x seconds
    scrMillisPrev = scrMillis;                                 // update previous reference time

    if ( scrDisplayMode < 2 ) {                                // if scrDisplayMode is smaller than 2 ...
      scrDisplayMode = scrDisplayMode + 1;                     // ... increase scrDisplayMode by 1
    } else {                                                   // ... else ...
      scrDisplayMode = 0;                                      // ... restart with the first content
    }
  }

  if ( scrDisplayMode == 0 ) {
    modeClk(); // call clock mode
  }
  if ( scrDisplayMode == 1 ) {
    modeDat(); // call date mode
  }
  if ( scrDisplayMode == 2 ) {
    modeTem(); // call temperature mode
  }
}

/**
 * @brief SETUP - flash memory
 * @details Create preferences array in read/write mode and load all saved data.
 * @since 1.0.0
 */
void initPREFERENCES() {
  if ( DEBUG ) {
    Serial.println( "- Preferences" );
  }

  pref.begin( "shelfclock", false );
  // last mode
  displayMode        = pref.getInt( "displayMode", 0 );
  // wifi
  apConfig           = pref.getInt( "apConfig", 0 );
  apSSID             = pref.getString( "apSSID", "Shelf Edge Clock" );
  apPass             = pref.getString( "apPass", "shelfEdgeClock" );
  apHide             = pref.getInt( "apHide", 0 );
  apIP1              = pref.getInt( "apIP1", 192 );
  apIP2              = pref.getInt( "apIP2", 168 );
  apIP3              = pref.getInt( "apIP3", 100 );
  apIP4              = pref.getInt( "apIP4", 1 );
  apGW1              = pref.getInt( "apGW1", 192 );
  apGW2              = pref.getInt( "apGW2", 168 );
  apGW3              = pref.getInt( "apGW3", 100 );
  apGW4              = pref.getInt( "apGW4", 1 );
  apSN1              = pref.getInt( "apSN1", 255 );
  apSN2              = pref.getInt( "apSN2", 255 );
  apSN3              = pref.getInt( "apSN3", 255 );
  apSN4              = pref.getInt( "apSN4", 0 );
  wifiConfig         = pref.getInt( "wifiConfig", 0 );
  wifiSSID           = pref.getString( "wifiSSID", "ENTER_SSID" );
  wifiPass           = pref.getString( "wifiPass", "ENTER_PASSWORD" );
  wifiDHCP           = pref.getInt( "wifiDHCP", 0 );
  wifiIP1            = pref.getInt( "wifiIP1" );
  wifiIP2            = pref.getInt( "wifiIP2" );
  wifiIP3            = pref.getInt( "wifiIP3" );
  wifiIP4            = pref.getInt( "wifiIP4" );
  wifiGW1            = pref.getInt( "wifiGW1" );
  wifiGW2            = pref.getInt( "wifiGW2" );
  wifiGW3            = pref.getInt( "wifiGW3" );
  wifiGW4            = pref.getInt( "wifiGW4" );
  wifiSN1            = pref.getInt( "wifiSN1" );
  wifiSN2            = pref.getInt( "wifiSN2" );
  wifiSN3            = pref.getInt( "wifiSN3" );
  wifiSN4            = pref.getInt( "wifiSN4" );
  // global settings
  edgeBrightness     = pref.getInt( "edgBright", edgeBrightness );
  edgeBrightnessMin  = pref.getInt( "edgBrightMin", 5 );
  edgeBrightnessMax  = pref.getInt( "edgBrightMax", 125 );
  edgeBrightnessOut  = pref.getInt( "edgBrightOut", 25 );
  edgeFrequency      = pref.getInt( "edgColFreq", edgeFrequency );
  downUsage          = pref.getInt( "dowUsage", downUsage );
  downBrightness     = pref.getInt( "dowBright", downBrightness );
  downBrightnessMin  = pref.getInt( "dowBrightMin ", 10 );
  downBrightnessMax  = pref.getInt( "dowBrightMax", 250 );
  downBrightnessOut  = pref.getInt( "dowBrightOut", 50 );
  downFrequency      = pref.getInt( "dowColFreq", downFrequency );
  // knx connection
  knxAddress         = pref.getString( "knxAddr", "0.0.0.0" );
  knxPort            = pref.getInt( "knxPort", 8080 );
  knxBrightnessPath  = pref.getString( "knxBrightPath", "/rest/items/{item}/state" );
  knxBrightnessMin   = pref.getLong( "knxBrightMin", 0 );
  knxBrightnessMax   = pref.getLong( "knxBrightMax", 150000 );
  knxTemperaturePath = pref.getString( "knxTempPath", "/rest/items/{item}/state" );
  knxHumidityPath    = pref.getString( "knxHumPath", "/rest/items/{item}/state" );
  // downlights
  dowColor           = pref.getInt( "dowColor", 0 );
  dowColorR          = pref.getInt( "dowColorR", 255 );
  dowColorG          = pref.getInt( "dowColorG", 255 );
  dowColorB          = pref.getInt( "dowColorB", 255 );
  dowColor0R         = pref.getInt( "dowColor0R", 255 );
  dowColor0G         = pref.getInt( "dowColor0G", 255 );
  dowColor0B         = pref.getInt( "dowColor0B", 255 );
  dowColor1R         = pref.getInt( "dowColor1R", 255 );
  dowColor1G         = pref.getInt( "dowColor1G", 255 );
  dowColor1B         = pref.getInt( "dowColor1B", 255 );
  dowColor2R         = pref.getInt( "dowColor2R", 255 );
  dowColor2G         = pref.getInt( "dowColor2G", 255 );
  dowColor2B         = pref.getInt( "dowColor2B", 255 );
  dowColor3R         = pref.getInt( "dowColor3R", 255 );
  dowColor3G         = pref.getInt( "dowColor3G", 255 );
  dowColor3B         = pref.getInt( "dowColor3B", 255 );
  dowColor4R         = pref.getInt( "dowColor4R", 255 );
  dowColor4G         = pref.getInt( "dowColor4G", 255 );
  dowColor4B         = pref.getInt( "dowColor4B", 255 );
  dowColor5R         = pref.getInt( "dowColor5R", 255 );
  dowColor5G         = pref.getInt( "dowColor5G", 255 );
  dowColor5B         = pref.getInt( "dowColor5B", 255 );
  dowColor6R         = pref.getInt( "dowColor6R", 255 );
  dowColor6G         = pref.getInt( "dowColor6G", 255 );
  dowColor6B         = pref.getInt( "dowColor6B", 255 );
  dowColor7R         = pref.getInt( "dowColor7R", 255 );
  dowColor7G         = pref.getInt( "dowColor7G", 255 );
  dowColor7B         = pref.getInt( "dowColor7B", 255 );
  dowColor8R         = pref.getInt( "dowColor8R", 255 );
  dowColor8G         = pref.getInt( "dowColor8G", 255 );
  dowColor8B         = pref.getInt( "dowColor8B", 255 );
  dowColor9R         = pref.getInt( "dowColor9R", 255 );
  dowColor9G         = pref.getInt( "dowColor9G", 255 );
  dowColor9B         = pref.getInt( "dowColor9B", 255 );
  dowColor10R        = pref.getInt( "dowColor10R", 255 );
  dowColor10G        = pref.getInt( "dowColor10G", 255 );
  dowColor10B        = pref.getInt( "dowColor10B", 255 );
  dowColor11R        = pref.getInt( "dowColor11R", 255 );
  dowColor11G        = pref.getInt( "dowColor11G", 255 );
  dowColor11B        = pref.getInt( "dowColor11B", 255 );
  dowColor12R        = pref.getInt( "dowColor12R", 255 );
  dowColor12G        = pref.getInt( "dowColor12G", 255 );
  dowColor12B        = pref.getInt( "dowColor12B", 255 );
  dowColor13R        = pref.getInt( "dowColor13R", 255 );
  dowColor13G        = pref.getInt( "dowColor13G", 255 );
  dowColor13B        = pref.getInt( "dowColor13B", 255 );
  // mode clock
  clkAddress         = pref.getString( "clkAddr", clkAddress );
  clkTimeZone        = pref.getString( "clkZone", clkTimeZone );
  clkFormat          = pref.getInt( "clkFormat", 1 );
  clkColor           = pref.getInt( "clkColor", 0 );
  clkColorHR         = pref.getInt( "clkColHR", 85 );
  clkColorHG         = pref.getInt( "clkColHG", 26 );
  clkColorHB         = pref.getInt( "clkColHB", 139 );
  clkColorMR         = pref.getInt( "clkColMR", 255 );
  clkColorMG         = pref.getInt( "clkColMG", 0 );
  clkColorMB         = pref.getInt( "clkColMB", 132 );
  // mode date
  datFormat          = pref.getInt( "datFormat", 1 );
  datColor           = pref.getInt( "datColor", 0 );
  datColorDR         = pref.getInt( "datColorDR", 255 );
  datColorDG         = pref.getInt( "datColorDG", 215 );
  datColorDB         = pref.getInt( "datColorDB", 0 );
  datColorMR         = pref.getInt( "datColorMR", 255 );
  datColorMG         = pref.getInt( "datColorMG", 69 );
  datColorMB         = pref.getInt( "datColorMB", 0 );
  // mode temperature
  temSymbol          = pref.getInt( "temSymbol", 1 );
  // mode humidity
  humColor           = pref.getInt( "humColor", 0 );
  humColorAR         = pref.getInt( "humColorAR", 255 );
  humColorAG         = pref.getInt( "humColorAG", 255 );
  humColorAB         = pref.getInt( "humColorAB", 0 );
  humColorBR         = pref.getInt( "humColorBR", 0 );
  humColorBG         = pref.getInt( "humColorBG", 255 );
  humColorBB         = pref.getInt( "humColorBB", 255 );
  humColorCR         = pref.getInt( "humColorCR", 255 );
  humColorCG         = pref.getInt( "humColorCG", 0 );
  humColorCB         = pref.getInt( "humColorCB", 255 );
  // mode scoreboard
  scoColorLR         = pref.getInt( "scoLColR", 0 );
  scoColorLG         = pref.getInt( "scoLColG", 0 );
  scoColorLB         = pref.getInt( "scoLColB", 255 );
  scoColorRR         = pref.getInt( "scoRColR", 0 );
  scoColorRG         = pref.getInt( "scoRColG", 255 );
  scoColorRB         = pref.getInt( "scoRColB", 255 );
  // mode countdown
  couColorHR         = pref.getInt( "couColorHR", 255 );
  couColorHG         = pref.getInt( "couColorHG", 140 );
  couColorHB         = pref.getInt( "couColorHB", 0 );
  couColorMR         = pref.getInt( "couColorMR", 255 );
  couColorMG         = pref.getInt( "couColorMG", 165 );
  couColorMB         = pref.getInt( "couColorMB", 0 );
  couColorSR         = pref.getInt( "couColorSR", 255 );
  couColorSG         = pref.getInt( "couColorSG", 215 );
  couColorSB         = pref.getInt( "couColorSB", 0 );
  couColorAR         = pref.getInt( "couColorAR", 255 );
  couColorAG         = pref.getInt( "couColorAG", 0 );
  couColorAB         = pref.getInt( "couColorAB", 0 );
  // mode scroll
  scrTime            = pref.getInt( "scrTime", 4 );
}

/**
 * @brief SETUP - wifi
 * @details initialize the access point and wifi client
 * @since 1.0.0
 */
void initWIFI() {
  WiFi.mode( WIFI_AP_STA ); // set mode: access point & wifi client
  // access point (ap)
  apLocalIP = IPAddress( apIP1, apIP2, apIP3, apIP4 );      // ip
  apGateway = IPAddress( apGW1, apGW2, apGW3, apGW4 );      // gateway
  apSubnet  = IPAddress( apSN1, apSN2, apSN3, apSN4 );      // subnet
  WiFi.softAPConfig( apLocalIP, apGateway, apSubnet );      // config ap
  WiFi.softAP( apSSID.c_str(), apPass.c_str(), 1, apHide ); // start ap

  if ( DEBUG ) {
    Serial.println( "- Access point" );
    Serial.printf( "  SSID:       %s\n", WiFi.softAPSSID().c_str() );
    Serial.printf( "  Pass:       %s\n", apPass.c_str() );
    Serial.printf( "  IP:         %s\n", WiFi.softAPIP().toString().c_str() );
  }

  //  wifi client
  if ( wifiDHCP ) {                                                // if fixed ip used
    wifiLocalIP = IPAddress( wifiIP1, wifiIP2, wifiIP3, wifiIP4 ); // ip
    wifiGateway = IPAddress( wifiGW1, wifiGW2, wifiGW3, wifiGW4 ); // gateway
    wifiSubnet  = IPAddress( wifiSN1, wifiSN2, wifiSN3, wifiSN4 ); // subnet
    WiFi.config( wifiLocalIP, wifiGateway, wifiSubnet );           // config wifi
  }
  WiFi.begin( wifiSSID.c_str(), wifiPass.c_str() );                // connect to wifi
  for ( int i = 0; i < 10; i++ ) {                                 // loop 10 times
    delay( 200 );                                                  // wait before each check
    if ( WiFi.status() == WL_CONNECTED ) {                         // check if connected
      wifiIP1 = WiFi.localIP()[ 0 ];
      wifiIP2 = WiFi.localIP()[ 1 ];
      wifiIP3 = WiFi.localIP()[ 2 ];
      wifiIP4 = WiFi.localIP()[ 3 ];
      wifiGW1 = WiFi.gatewayIP()[ 0 ];
      wifiGW2 = WiFi.gatewayIP()[ 1 ];
      wifiGW3 = WiFi.gatewayIP()[ 2 ];
      wifiGW4 = WiFi.gatewayIP()[ 3 ];
      wifiSN1 = WiFi.subnetMask()[ 0 ];
      wifiSN2 = WiFi.subnetMask()[ 1 ];
      wifiSN3 = WiFi.subnetMask()[ 2 ];
      wifiSN4 = WiFi.subnetMask()[ 3 ];
      if ( DEBUG ) {
        Serial.println( "- WiFi client" );
        Serial.printf( "  SSID:       %s\n", WiFi.SSID().c_str() );
        Serial.printf( "  IP:         %s\n", WiFi.localIP().toString().c_str() );
      }
      break; // if connected, leave the cycle
    }
  }
  if ( WiFi.status() != WL_CONNECTED ) { // if not connected
    WiFi.disconnect();                   // turn off wifi
  }
}

/**
 * @brief SETUP - spiffs
 * @details initialize the spiffs storage for the webserver
 * @since 1.0.0
 */
void initSPIFFS() {
  if ( SPIFFS.begin( true ) ) {
    if ( DEBUG ) {
      Serial.println( "- SPIFFS" );
    }
  } else {
    ESP.restart();
  }
}

/**
 * @brief SETUP - ntp
 * @details configure the ntp settings and get the time
 * @since 1.0.0
 */
void initNTP() {
  if ( WiFi.status() == WL_CONNECTED ) {
    // connect NTP (0 TZ offset)
    configTime( 0, 0, clkAddress.c_str() );
    // overwrite TZ
    setenv( "TZ", clkTimeZone.c_str(), 1 );
    // adjust the TZ
    tzset();

    if ( !getLocalTime( &ntpTime ) ) {
      ESP.restart();
    }
  }
}

/**
 * @brief SETUP - web handlers
 * @details configure all the necessary web requests
 * @since 1.0.0
 */
void initHANDLERS() {
  // --> don't touch the following code
  // clang-format off

  // default
  server.on( "/",                               HTTP_GET, []( AsyncWebServerRequest *request ) { request->send( SPIFFS, "/index.html", "text/html", false ); } );

  //  index.html
  server.on( "/updateDisplayMode",              HTTP_POST, []( AsyncWebServerRequest *request ) { displayMode = request->arg( "mode" ).toInt(); pref.putInt( "displayMode", displayMode ); if ( displayMode == 4 ) { scoLValue = request->arg( "data1" ).toInt(); scoRValue = request->arg( "data2" ).toInt(); } if (displayMode == 5 ) { couMillis = request->arg( "data1" ).toInt(); couMillisEnd = millis() + couMillis; } request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlight",                HTTP_POST, []( AsyncWebServerRequest *request ) { downUsage = request->arg( "downlightUsage" ).toInt(); pref.putInt( "dowUsage", downUsage ); dowColor = request->arg( "downlightColor" ).toInt(); pref.putInt( "dowColor", dowColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  settings.html
  //  |-  send
  //  |   |-  general settings
  server.on( "/getEdgeBrightness",              []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightness ) ); } );
  server.on( "/getEdgeBrightnessMin",           []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightnessMin ) ); } );
  server.on( "/getEdgeBrightnessMax",           []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightnessMax ) ); } );
  server.on( "/getEdgeFrequency",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeFrequency ) ); } );
  server.on( "/getDownlightUsage",              []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downUsage ) ); } );
  server.on( "/getDownlightBrightness",         []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightness ) ); } );
  server.on( "/getDownlightBrightnessMin",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightnessMin ) ); } );
  server.on( "/getDownlightBrightnessMax",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightnessMax ) );  } );
  server.on( "/getDownlightFrequency",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downFrequency ) ); } );
  //  |   |-  knx settings
  server.on( "/getKNXAddress",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxAddress ) ); } );
  server.on( "/getKNXPort",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxPort ) ); } );
  server.on( "/getKNXBrightnessPath",           []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightnessPath ) ); } );
  server.on( "/getKNXBrightnessMin",            []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightnessMin ) ); } );
  server.on( "/getKNXBrightnessMax",            []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightnessMax ) ); } );
  server.on( "/getKNXTemperaturePath",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxTemperaturePath ) ); } );
  server.on( "/getKNXHumidityPath",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxHumidityPath ) ); } );
  //  |   |-  mode clock
  server.on( "/getClkAddress",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkAddress ) ); } );
  server.on( "/getClkTimeZone",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkTimeZone ) ); } );
  server.on( "/getClkFormat",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkFormat ) ); } );
  server.on( "/getClkColor",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkColor ) ); } );
  server.on( "/getClkColorH",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", clkColorHR, clkColorHG, clkColorHB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getClkColorM",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", clkColorMR, clkColorMG, clkColorMB ); request->send( 200, "text/plain", tempcolor ); } );
  //  |   |-  mode date
  server.on( "/getDatFormat",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datFormat ) ); } );
  server.on( "/getDatColor",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datColor ) ); } );
  server.on( "/getDatColorD",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", datColorDR, datColorDG, datColorDB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDatColorM",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", datColorMR, datColorMG, datColorMB ); request->send( 200, "text/plain", tempcolor ); } );
  //  |   |-  mode temperature
  server.on( "/getTemSymbol",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( temSymbol ) ); } );
  //  |   |-  mode humidity
  server.on( "/getHumColor",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( humColor ) ); } );
  server.on( "/getHumColorA",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorAR, humColorAG, humColorAB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getHumColorB",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorBR, humColorBG, humColorBB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getHumColorC",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorCR, humColorCG, humColorCB ); request->send( 200, "text/plain", tempcolor ); } );
  //  |   |-  mode scoreboard
  server.on( "/getScoColorL",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", scoColorLR, scoColorLG, scoColorLB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getScoColorR",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", scoColorRR, scoColorRG, scoColorRB ); request->send( 200, "text/plain", tempcolor ); } );
  //  |   |-  mode countdown
  server.on( "/getCouColorH",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorHR, couColorHG, couColorHB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorM",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorMR, couColorMG, couColorMB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorS",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorSR, couColorSG, couColorSB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorA",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorAR, couColorAG, couColorAB ); request->send( 200, "text/plain", tempcolor ); } );
  //  |   |-  mode scrolling
  server.on( "/getScrTime",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( scrTime ) ); } );
  //  |   |-  downlights
  server.on( "/getDowColor",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( dowColor ) ); } );
  server.on( "/getDowColor0",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor0R, dowColor0G, dowColor0B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor1",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor1R, dowColor1G, dowColor1B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor2",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor2R, dowColor2G, dowColor2B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor3",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor3R, dowColor3G, dowColor3B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor4",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor4R, dowColor4G, dowColor4B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor5",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor5R, dowColor5G, dowColor5B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor6",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor6R, dowColor6G, dowColor6B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor7",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor7R, dowColor7G, dowColor7B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor8",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor8R, dowColor8G, dowColor8B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor9",                   []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor9R, dowColor9G, dowColor9B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor10",                  []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor10R, dowColor10G, dowColor10B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor11",                  []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor11R, dowColor11G, dowColor11B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor12",                  []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor12R, dowColor12G, dowColor12B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDowColor13",                  []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", dowColor13R, dowColor13G, dowColor13B ); request->send( 200, "text/plain", tempcolor ); } );
  //  |-  receive
  //  |   |-  general settings
  server.on( "/updateEdgeBrightness",           HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightness = request->arg( "edgeBrightness" ).toInt(); pref.putInt( "edgBright", edgeBrightness ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateEdgeBrightnessMin",        HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightnessMin = request->arg( "edgeBrightnessMin" ).toInt(); pref.putInt( "edgBrightMin", edgeBrightnessMin ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateEdgeBrightnessMax",        HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightnessMax = request->arg( "edgeBrightnessMax" ).toInt(); pref.putInt( "edgBrightMax", edgeBrightnessMax ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateEdgeFrequency",            HTTP_POST, []( AsyncWebServerRequest *request ) { edgeFrequency = request->arg( "edgeFrequency" ).toInt(); pref.putInt( "edgColFreq", edgeFrequency ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlightUsage",           HTTP_POST, []( AsyncWebServerRequest *request ) { downUsage = request->arg( "downUsage" ).toInt(); pref.putInt( "dowUsage", downUsage ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlightBrightness",      HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightness = request->arg( "downBrightness" ).toInt(); pref.putInt( "dowBright", downBrightness ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlightBrightnessMin",   HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightnessMin = request->arg( "downBrightnessMin" ).toInt(); pref.putInt( "dowBrightMin", downBrightnessMin ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlightBrightnessMax",   HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightnessMax = request->arg( "downBrightnessMax" ).toInt(); pref.putInt( "dowBrightMax", downBrightnessMax ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDownlightFrequency",       HTTP_POST, []( AsyncWebServerRequest *request ) { downFrequency = request->arg( "downFrequency" ).toInt(); pref.putInt( "dowColFreq", downFrequency ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  knx settings
  server.on( "/updateKNXAddress",               HTTP_POST, []( AsyncWebServerRequest *request ) { knxAddress = request->arg( "knxAddress" ); pref.putString( "knxAddr", knxAddress ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateKNXPort",                  HTTP_POST, []( AsyncWebServerRequest *request ) { knxPort = request->arg( "knxPort" ).toInt(); pref.putInt( "knxPort", knxPort ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateKNXBrightnessPath",        HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightnessPath = request->arg( "knxBrightnessPath" ); pref.putString( "knxBrightPath", knxBrightnessPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/updateKNXBrightnessMin",         HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightnessMin = request->arg( "knxBrightnessMin" ).toInt(); pref.putInt( "knxBrightMin", knxBrightnessMin ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/updateKNXBrightnessMax",         HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightnessMax = request->arg( "knxBrightnessMax" ).toInt(); pref.putInt( "knxBrightMax", knxBrightnessMax ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/updateKNXTemperaturePath",       HTTP_POST, []( AsyncWebServerRequest *request ) { knxTemperaturePath = request->arg( "knxTemperaturePath" ); pref.putString( "knxTempPath", knxTemperaturePath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXtemperature(); } );
  server.on( "/updateKNXHumidityPath",          HTTP_POST, []( AsyncWebServerRequest *request ) { knxHumidityPath = request->arg( "knxHumidityPath" ); pref.putString( "knxHumPath", knxHumidityPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXhumidity(); } );
  //  |   |-  mode clock
  server.on( "/updateClkAddress",               HTTP_POST, []( AsyncWebServerRequest *request ) { clkAddress = request->arg( "clkAddress" ); pref.putString( "clkAddr", clkAddress ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateClkTimeZone",              HTTP_POST, []( AsyncWebServerRequest *request ) { clkTimeZone = request->arg( "clkTimeZone" ); pref.putString( "clkZone", clkTimeZone ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateClkFormat",                HTTP_POST, []( AsyncWebServerRequest *request ) { clkFormat = request->arg( "clkFormat" ).toInt(); pref.putInt( "clkFormat", clkFormat ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateClkColor",                 HTTP_POST, []( AsyncWebServerRequest *request ) { clkColor = request->arg( "clkColor" ).toInt(); pref.putInt( "clkColor", clkColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateClkColorH",                HTTP_POST, []( AsyncWebServerRequest *request ) { clkColorHR = request->arg( "r" ).toInt(); pref.putInt( "clkColorHR", clkColorHR ); clkColorHG = request->arg( "g" ).toInt(); pref.putInt( "clkColorHG", clkColorHG ); clkColorHB = request->arg( "b" ).toInt(); pref.putInt( "clkColorHB", clkColorHB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateClkColorM",                HTTP_POST, []( AsyncWebServerRequest *request ) { clkColorMR = request->arg( "r" ).toInt(); pref.putInt( "clkColorMR", clkColorMR ); clkColorMG = request->arg( "g" ).toInt(); pref.putInt( "clkColorMG", clkColorMG ); clkColorMB = request->arg( "b" ).toInt(); pref.putInt( "clkColorMB", clkColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode date
  server.on( "/updateDatFormat",                HTTP_POST, []( AsyncWebServerRequest *request ) { datFormat = request->arg( "datFormat" ).toInt(); pref.putInt( "datFormat", datFormat ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDatColor",                 HTTP_POST, []( AsyncWebServerRequest *request ) { datColor = request->arg( "datColor" ).toInt(); pref.putInt( "datColor", datColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDatColorD",                HTTP_POST, []( AsyncWebServerRequest *request ) { datColorDR = request->arg( "r" ).toInt(); pref.putInt( "datColorDR", datColorDR ); datColorDG = request->arg( "g" ).toInt(); pref.putInt( "datColorDG", datColorDG ); datColorDB = request->arg( "b" ).toInt(); pref.putInt( "datColorDB", datColorDB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDatColorM",                HTTP_POST, []( AsyncWebServerRequest *request ) { datColorMR = request->arg( "r" ).toInt(); pref.putInt( "datColorMR", datColorMR ); datColorMG = request->arg( "g" ).toInt(); pref.putInt( "datColorMG", datColorMG ); datColorMB = request->arg( "b" ).toInt(); pref.putInt( "datColorMB", datColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode temperature
  server.on( "/updateTemSymbol",                HTTP_POST, []( AsyncWebServerRequest *request ) { temSymbol = request->arg( "temSymbol" ).toInt(); pref.putInt( "temSymbol", temSymbol ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode humidity
  server.on( "/updateHumColor",                 HTTP_POST, []( AsyncWebServerRequest *request ) { humColor = request->arg( "humColor" ).toInt(); pref.putInt( "humColor", humColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateHumColorA",                HTTP_POST, []( AsyncWebServerRequest *request ) { humColorAR = request->arg( "r" ).toInt(); pref.putInt( "humColorAR", humColorAR ); humColorAG = request->arg( "g" ).toInt(); pref.putInt( "humColorAG", humColorAG ); humColorAB = request->arg( "b" ).toInt(); pref.putInt( "humColorAB", humColorAB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateHumColorB",                HTTP_POST, []( AsyncWebServerRequest *request ) { humColorBR = request->arg( "r" ).toInt(); pref.putInt( "humColorBR", humColorBR ); humColorBG = request->arg( "g" ).toInt(); pref.putInt( "humColorBG", humColorBG ); humColorBB = request->arg( "b" ).toInt(); pref.putInt( "humColorBB", humColorBB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateHumColorC",                HTTP_POST, []( AsyncWebServerRequest *request ) { humColorCR = request->arg( "r" ).toInt(); pref.putInt( "humColorCR", humColorCR ); humColorCG = request->arg( "g" ).toInt(); pref.putInt( "humColorCG", humColorCG ); humColorCB = request->arg( "b" ).toInt(); pref.putInt( "humColorCB", humColorCB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode scoreboard
  server.on( "/updateScoColorL",                HTTP_POST, []( AsyncWebServerRequest *request ) { scoColorLR = request->arg( "r" ).toInt(); pref.putInt( "scoColorLR", scoColorLR ); scoColorLG = request->arg( "g" ).toInt(); pref.putInt( "scoColorLG", scoColorLG ); scoColorLB = request->arg( "b" ).toInt(); pref.putInt( "scoColorLB", scoColorLB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateScoColorR",                HTTP_POST, []( AsyncWebServerRequest *request ) { scoColorRR = request->arg( "r" ).toInt(); pref.putInt( "scoColorRR", scoColorRR ); scoColorRG = request->arg( "g" ).toInt(); pref.putInt( "scoColorRG", scoColorRG ); scoColorRB = request->arg( "b" ).toInt(); pref.putInt( "scoColorRB", scoColorRB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode countdown
  server.on( "/updateCouColorH",                HTTP_POST, []( AsyncWebServerRequest *request ) { couColorHR = request->arg( "r" ).toInt(); pref.putInt( "couColorHR", couColorHR ); couColorHG = request->arg( "g" ).toInt(); pref.putInt( "couColorHG", couColorHG ); couColorHB = request->arg( "b" ).toInt(); pref.putInt( "couColorHB", couColorHB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateCouColorM",                HTTP_POST, []( AsyncWebServerRequest *request ) { couColorMR = request->arg( "r" ).toInt(); pref.putInt( "couColorMR", couColorMR ); couColorMG = request->arg( "g" ).toInt(); pref.putInt( "couColorMG", couColorMG ); couColorMB = request->arg( "b" ).toInt(); pref.putInt( "couColorMB", couColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateCouColorS",                HTTP_POST, []( AsyncWebServerRequest *request ) { couColorSR = request->arg( "r" ).toInt(); pref.putInt( "couColorSR", couColorSR ); couColorSG = request->arg( "g" ).toInt(); pref.putInt( "couColorSG", couColorSG ); couColorSB = request->arg( "b" ).toInt(); pref.putInt( "couColorSB", couColorSB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateCouColorA",                HTTP_POST, []( AsyncWebServerRequest *request ) { couColorAR = request->arg( "r" ).toInt(); pref.putInt( "couColorAR", couColorAR ); couColorAG = request->arg( "g" ).toInt(); pref.putInt( "couColorAG", couColorAG ); couColorAB = request->arg( "b" ).toInt(); pref.putInt( "couColorAB", couColorAB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  mode scrolling
  server.on( "/updateScrTime",                  HTTP_POST, []( AsyncWebServerRequest *request ) { scrTime = request->arg( "scrTime" ).toInt(); pref.putInt( "scrTime", scrTime ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  downlights
  server.on( "/updateDowColor",                 HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor = request->arg( "dowColor" ).toInt(); pref.putInt( "dowColor", dowColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor",                 HTTP_POST, []( AsyncWebServerRequest *request ) { dowColorR = request->arg( "r" ).toInt(); pref.putInt( "dowColorR", dowColorR ); dowColorG = request->arg( "g" ).toInt(); pref.putInt( "dowColorG", dowColorG ); dowColorB = request->arg( "b" ).toInt(); pref.putInt( "dowColorB", dowColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor0",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor0R = request->arg( "r" ).toInt(); pref.putInt( "dowColor0R", dowColor0R ); dowColor0G = request->arg( "g" ).toInt(); pref.putInt( "dowColor0G", dowColor0G ); dowColor0B = request->arg( "b" ).toInt(); pref.putInt( "dowColor0B", dowColor0B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor1",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor1R = request->arg( "r" ).toInt(); pref.putInt( "dowColor1R", dowColor1R ); dowColor1G = request->arg( "g" ).toInt(); pref.putInt( "dowColor1G", dowColor1G ); dowColor1B = request->arg( "b" ).toInt(); pref.putInt( "dowColor1B", dowColor1B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor2",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor2R = request->arg( "r" ).toInt(); pref.putInt( "dowColor2R", dowColor2R ); dowColor2G = request->arg( "g" ).toInt(); pref.putInt( "dowColor2G", dowColor2G ); dowColor2B = request->arg( "b" ).toInt(); pref.putInt( "dowColor2B", dowColor2B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor3",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor3R = request->arg( "r" ).toInt(); pref.putInt( "dowColor3R", dowColor3R ); dowColor3G = request->arg( "g" ).toInt(); pref.putInt( "dowColor3G", dowColor3G ); dowColor3B = request->arg( "b" ).toInt(); pref.putInt( "dowColor3B", dowColor3B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor4",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor4R = request->arg( "r" ).toInt(); pref.putInt( "dowColor4R", dowColor4R ); dowColor4G = request->arg( "g" ).toInt(); pref.putInt( "dowColor4G", dowColor4G ); dowColor4B = request->arg( "b" ).toInt(); pref.putInt( "dowColor4B", dowColor4B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor5",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor5R = request->arg( "r" ).toInt(); pref.putInt( "dowColor5R", dowColor5R ); dowColor5G = request->arg( "g" ).toInt(); pref.putInt( "dowColor5G", dowColor5G ); dowColor5B = request->arg( "b" ).toInt(); pref.putInt( "dowColor5B", dowColor5B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor6",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor6R = request->arg( "r" ).toInt(); pref.putInt( "dowColor6R", dowColor6R ); dowColor6G = request->arg( "g" ).toInt(); pref.putInt( "dowColor6G", dowColor6G ); dowColor6B = request->arg( "b" ).toInt(); pref.putInt( "dowColor6B", dowColor6B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor7",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor7R = request->arg( "r" ).toInt(); pref.putInt( "dowColor7R", dowColor7R ); dowColor7G = request->arg( "g" ).toInt(); pref.putInt( "dowColor7G", dowColor7G ); dowColor7B = request->arg( "b" ).toInt(); pref.putInt( "dowColor7B", dowColor7B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor8",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor8R = request->arg( "r" ).toInt(); pref.putInt( "dowColor8R", dowColor8R ); dowColor8G = request->arg( "g" ).toInt(); pref.putInt( "dowColor8G", dowColor8G ); dowColor8B = request->arg( "b" ).toInt(); pref.putInt( "dowColor8B", dowColor8B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor9",                HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor9R = request->arg( "r" ).toInt(); pref.putInt( "dowColor9R", dowColor9R ); dowColor9G = request->arg( "g" ).toInt(); pref.putInt( "dowColor9G", dowColor9G ); dowColor9B = request->arg( "b" ).toInt(); pref.putInt( "dowColor9B", dowColor9B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor10",               HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor10R = request->arg( "r" ).toInt(); pref.putInt( "dowColor10R", dowColor10R ); dowColor10G = request->arg( "g" ).toInt(); pref.putInt( "dowColor10G", dowColor10G ); dowColor10B = request->arg( "b" ).toInt(); pref.putInt( "dowColor10B", dowColor10B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor11",               HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor11R = request->arg( "r" ).toInt(); pref.putInt( "dowColor11R", dowColor11R ); dowColor11G = request->arg( "g" ).toInt(); pref.putInt( "dowColor11G", dowColor11G ); dowColor11B = request->arg( "b" ).toInt(); pref.putInt( "dowColor11B", dowColor11B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor12",               HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor12R = request->arg( "r" ).toInt(); pref.putInt( "dowColor12R", dowColor12R ); dowColor12G = request->arg( "g" ).toInt(); pref.putInt( "dowColor12G", dowColor12G ); dowColor12B = request->arg( "b" ).toInt(); pref.putInt( "dowColor12B", dowColor12B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateDowColor13",               HTTP_POST, []( AsyncWebServerRequest *request ) { dowColor13R = request->arg( "r" ).toInt(); pref.putInt( "dowColor13R", dowColor13R ); dowColor13G = request->arg( "g" ).toInt(); pref.putInt( "dowColor13G", dowColor13G ); dowColor13B = request->arg( "b" ).toInt(); pref.putInt( "dowColor13B", dowColor13B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  wifi.html
  //  |   send
  //  |   |-  access point
  server.on( "/getAPConfig",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apConfig ) ); } );
  server.on( "/getAPSSID",                      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSSID ) ); } );
  server.on( "/getAPPass",                      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apPass ) ); } );
  server.on( "/getAPHide",                      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apHide ) ); } );
  server.on( "/getAPIP1",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP1 ) ); } );
  server.on( "/getAPIP2",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP2 ) ); } );
  server.on( "/getAPIP3",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP3 ) ); } );
  server.on( "/getAPIP4",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP4 ) ); } );
  server.on( "/getAPGW1",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW1 ) ); } );
  server.on( "/getAPGW2",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW2 ) ); } );
  server.on( "/getAPGW3",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW3 ) ); } );
  server.on( "/getAPGW4",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW4 ) ); } );
  server.on( "/getAPSN1",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN1 ) ); } );
  server.on( "/getAPSN2",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN2 ) ); } );
  server.on( "/getAPSN3",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN3 ) ); } );
  server.on( "/getAPSN4",                       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN4 ) ); } );
  //  |   |-  wifi client
  server.on( "/getWifiConfig",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiConfig ) ); } );
  server.on( "/getWifiSSID",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSSID ) ); } );
  server.on( "/getWifiPass",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiPass ) ); } );
  server.on( "/getWifiDHCP",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiDHCP ) ); } );
  server.on( "/getWifiIP1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP1 ) ); } );
  server.on( "/getWifiIP2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP2 ) ); } );
  server.on( "/getWifiIP3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP3 ) ); } );
  server.on( "/getWifiIP4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP4 ) ); } );
  server.on( "/getWifiGW1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW1 ) ); } );
  server.on( "/getWifiGW2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW2 ) ); } );
  server.on( "/getWifiGW3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW3 ) ); } );
  server.on( "/getWifiGW4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW4 ) ); } );
  server.on( "/getWifiSN1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN1 ) ); } );
  server.on( "/getWifiSN2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN2 ) ); } );
  server.on( "/getWifiSN3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN3 ) ); } );
  server.on( "/getWifiSN4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN4 ) ); } );
  //  |   receive
  //  |   |-  access point
  server.on( "/updateAPConfig",                 HTTP_POST, []( AsyncWebServerRequest *request ) { apConfig = request->arg( "apConfig" ); pref.putInt( "apConfig", apConfig ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPSSID",                   HTTP_POST, []( AsyncWebServerRequest *request ) { apSSID = request->arg( "apSSID" ); pref.putString( "apSSID", apSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPPass",                   HTTP_POST, []( AsyncWebServerRequest *request ) { apPass = request->arg( "apPass" ); pref.putString( "apPass", apPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPHide",                   HTTP_POST, []( AsyncWebServerRequest *request ) { apHide = request->arg( "apHide" ).toInt(); pref.putInt( "apHide", apHide ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPIP1",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apIP1 = request->arg( "apIP1" ).toInt(); pref.putInt( "apIP1", apIP1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPIP2",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apIP2 = request->arg( "apIP2" ).toInt(); pref.putInt( "apIP2", apIP2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPIP3",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apIP3 = request->arg( "apIP3" ).toInt(); pref.putInt( "apIP3", apIP3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPIP4",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apIP4 = request->arg( "apIP4" ).toInt(); pref.putInt( "apIP4", apIP4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPGW1",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apGW1 = request->arg( "apGW1" ).toInt(); pref.putInt( "apGW1", apGW1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPGW2",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apGW2 = request->arg( "apGW2" ).toInt(); pref.putInt( "apGW2", apGW2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPGW3",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apGW3 = request->arg( "apGW3" ).toInt(); pref.putInt( "apGW3", apGW3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPGW4",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apGW4 = request->arg( "apGW4" ).toInt(); pref.putInt( "apGW4", apGW4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPSN1",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apSN1 = request->arg( "apSN1" ).toInt(); pref.putInt( "apSN1", apSN1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPSN2",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apSN2 = request->arg( "apSN2" ).toInt(); pref.putInt( "apSN2", apSN2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPSN3",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apSN3 = request->arg( "apSN3" ).toInt(); pref.putInt( "apSN3", apSN3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPSN4",                    HTTP_POST, []( AsyncWebServerRequest *request ) { apSN4 = request->arg( "apSN4" ).toInt(); pref.putInt( "apSN4", apSN4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateAPrestart",                HTTP_POST, []( AsyncWebServerRequest *request ) { getAP(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |   |-  wifi client
  server.on( "/updateWifiConfig",               HTTP_POST, []( AsyncWebServerRequest *request ) { wifiConfig = request->arg( "wifiConfig" ); pref.putInt( "wifiConfig", wifiConfig ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiSSID",                 HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSSID = request->arg( "wifiSSID" ); pref.putString( "wifiSSID", wifiSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiPass",                 HTTP_POST, []( AsyncWebServerRequest *request ) { wifiPass = request->arg( "wifiPass" ); pref.putString( "wifiPass", wifiPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiDHCP",                 HTTP_POST, []( AsyncWebServerRequest *request ) { wifiDHCP = request->arg( "wifiDHCP" ).toInt(); pref.putInt( "wifiDHCP", wifiDHCP ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiIP1",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiIP1 = request->arg( "wifiIP1" ).toInt(); pref.putInt( "wifiIP1", wifiIP1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiIP2",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiIP2 = request->arg( "wifiIP2" ).toInt(); pref.putInt( "wifiIP2", wifiIP2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiIP3",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiIP3 = request->arg( "wifiIP3" ).toInt(); pref.putInt( "wifiIP3", wifiIP3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiIP4",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiIP4 = request->arg( "wifiIP4" ).toInt(); pref.putInt( "wifiIP4", wifiIP4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiGW1",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiGW1 = request->arg( "wifiGW1" ).toInt(); pref.putInt( "wifiGW1", wifiGW1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiGW2",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiGW2 = request->arg( "wifiGW2" ).toInt(); pref.putInt( "wifiGW2", wifiGW2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiGW3",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiGW3 = request->arg( "wifiGW3" ).toInt(); pref.putInt( "wifiGW3", wifiGW3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiGW4",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiGW4 = request->arg( "wifiGW4" ).toInt(); pref.putInt( "wifiGW4", wifiGW4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiSN1",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSN1 = request->arg( "wifiSN1" ).toInt(); pref.putInt( "wifiSN1", wifiSN1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiSN2",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSN2 = request->arg( "wifiSN2" ).toInt(); pref.putInt( "wifiSN2", wifiSN2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiSN3",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSN3 = request->arg( "wifiSN3" ).toInt(); pref.putInt( "wifiSN3", wifiSN3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiSN4",                  HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSN4 = request->arg( "wifiSN4" ).toInt(); pref.putInt( "wifiSN4", wifiSN4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updateWifiRestart",              HTTP_POST, []( AsyncWebServerRequest *request ) { getWIFI(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  help.html
  //  |- date & time
  server.on( "/updateDateTime",                 HTTP_POST, []( AsyncWebServerRequest *request ) {
    int yeararg         = request->arg( "year" ).toInt();
    int montharg        = request->arg( "month" ).toInt();
    int dayarg          = request->arg( "day" ).toInt();
    int hourarg         = request->arg( "hour" ).toInt();
    int minarg          = request->arg( "min" ).toInt();
    int secarg          = request->arg( "sec" ).toInt();
    ntpTime.tm_year     = yeararg - 1900;
    ntpTime.tm_mon      = montharg - 1;
    ntpTime.tm_mday     = dayarg;
    ntpTime.tm_hour     = hourarg;
    ntpTime.tm_min      = minarg;
    ntpTime.tm_sec      = secarg;
    time_t         t    = mktime( &ntpTime );
    struct timeval now1 = { .tv_sec = t };
    settimeofday( &now1, NULL );
    request->send( 200, "text/json", "{ \"result\":\"ok\" }" );
  } );
  //  |- resets
  server.on( "/resetFlash", HTTP_POST, []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); pref.clear(); ESP.restart(); } );
  server.on( "/resetESP", HTTP_POST, []( AsyncWebServerRequest *request ) {
    request->send( 200, "text/json", "{ \"result\":\"ok\" }" );
    ESP.restart();
  } );
  // clang-format on
}

/**
 * @brief SETUP - webserver
 * @details initialize the webserver and ota
 * @since 1.0.0
 */
void initSERVER() {
  server.serveStatic( "/", SPIFFS, "/" ); // set default path
  ElegantOTA.begin( &server );            // start ElegantOTA
  server.begin();                         // start webserver
}

/**
 * @brief SETUP - led
 * @details start the neopixel arrays
 * @since 1.0.0
 */
void initLED() {
  // edge
  FastLED.addLeds<NEOPIXEL, EDGE_PIN>( arrayEdge, EDGE_COUNT );
  FastLED[ 0 ].setCorrection( TypicalLEDStrip );
  FastLED[ 0 ].showLeds( edgeBrightnessOut );
  // down
  FastLED.addLeds<NEOPIXEL, DOWN_PIN>( arrayDown, DOWN_COUNT );
  FastLED[ 1 ].setCorrection( TypicalLEDStrip );
  FastLED[ 1 ].showLeds( downBrightnessOut );

  if ( DEBUG ) {
    Serial.println( "- LED" );
    Serial.printf( "  Edge:       %i\n", edgeBrightnessOut );
    Serial.printf( "  Down:       %i\n", downBrightnessOut );
  }
}

/**
 * @brief LOOP - access point
 * @details restart the access point
 * @since 1.2.0
 */
void getAP() {
  WiFi.softAPdisconnect();                                  // close access point
  WiFi.softAPConfig( apLocalIP, apGateway, apSubnet );      // load new data
  WiFi.softAP( apSSID.c_str(), apPass.c_str(), 1, apHide ); // restart access point
}

/**
 * @brief LOOP - wifi
 * @details reconnect the wifi client
 * @since 1.2.0
 */
void getWIFI() {
  WiFi.disconnect();                                     // close wifi client
  if ( wifiDHCP ) {                                      // if fixed ip adress configured
    WiFi.config( wifiLocalIP, wifiGateway, wifiSubnet ); // load new data
  }
  WiFi.begin( wifiSSID.c_str(), wifiPass.c_str() );      // restart wifi client
  for ( int i = 0; i < 10; i++ ) {
    delay( 200 );
    if ( WiFi.status() == WL_CONNECTED ) {
      wifiIP1 = WiFi.localIP()[ 0 ];
      wifiIP2 = WiFi.localIP()[ 1 ];
      wifiIP3 = WiFi.localIP()[ 2 ];
      wifiIP4 = WiFi.localIP()[ 3 ];
      wifiGW1 = WiFi.gatewayIP()[ 0 ];
      wifiGW2 = WiFi.gatewayIP()[ 1 ];
      wifiGW3 = WiFi.gatewayIP()[ 2 ];
      wifiGW4 = WiFi.gatewayIP()[ 3 ];
      wifiSN1 = WiFi.subnetMask()[ 0 ];
      wifiSN2 = WiFi.subnetMask()[ 1 ];
      wifiSN3 = WiFi.subnetMask()[ 2 ];
      wifiSN4 = WiFi.subnetMask()[ 3 ];
      break;
    }
  }
  if ( WiFi.status() != WL_CONNECTED ) {
    WiFi.disconnect();
  }
}

/**
 * @brief LOOP - ntp
 * @details Call an NTP server, get the current time and update the local time.
 * @since 1.0.0
 */
void getNTP() {
  if ( WiFi.status() == WL_CONNECTED ) {
    getLocalTime( &ntpTime );
  }
}

/**
 * @brief LOOP - knx brightness
 * @details
 * @since 1.2.0
 */
void getKNXbrightness() {
  if ( WiFi.status() == WL_CONNECTED ) {                     // if wifi connected
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxBrightnessPath != "" ) ) {
      knxClient.get( knxBrightnessPath );                    // call knx for value
      knxBrightnessValue = knxClient.responseBody().toInt(); // ...and write answer
      // edge
      edgeBrightnessOut =
        map( knxBrightnessValue, knxBrightnessMin, knxBrightnessMax, edgeBrightnessMin, edgeBrightnessMax );
      pref.putInt( "edgBrightOut", edgeBrightnessOut );
      // downlight
      downBrightnessOut =
        map( knxBrightnessValue, knxBrightnessMin, knxBrightnessMax, downBrightnessMin, downBrightnessMax );
      pref.putInt( "dowBrightOut", downBrightnessOut );

      if ( DEBUG ) {
        Serial.println( "- knxBrightness" );
        Serial.printf( "  knxValue:   %i\n", knxBrightnessValue );
        Serial.printf( "  edgeOut:    %i\n", edgeBrightnessOut );
        Serial.printf( "  downOut:    %i\n", downBrightnessOut );
      }
    }
  } else {     // if wifi not connected
    getWIFI(); // reconnect wifi first
  }
}

/**
 * @brief LOOP - knx temperature
 * @details
 * @since 1.2.0
 */
void getKNXtemperature() {
  if ( WiFi.status() == WL_CONNECTED ) {
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxTemperaturePath != "" ) ) {
      knxClient.get( knxTemperaturePath );                           // call knx for value
      knxTemperatureValue = ( knxClient.responseBody().toDouble() ); // ...and write answer
      if ( DEBUG ) {
        Serial.println( "- knxTemperature" );
        Serial.printf( "  knxValue:   %i\n", knxTemperatureValue );
      }
    }
  } else {     // if wifi not connected
    getWIFI(); // reconnect wifi first
  }
}

/**
 * @brief LOOP - knx humidity
 * @details fetch values from the home automation system
 * @since 1.2.0
 */
void getKNXhumidity() {
  if ( WiFi.status() == WL_CONNECTED ) {
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxHumidityPath != "" ) ) {
      knxClient.get( knxHumidityPath );                                       // call knx for value
      knxHumidityValue = int( ( knxClient.responseBody().toDouble() ) * 10 ); // ...and write answer
      if ( DEBUG ) {
        Serial.println( "- knxHumidity" );
        Serial.printf( "  knxValue:   %d\n", knxHumidityValue );
      }
    }
  } else {
    getWIFI();
  }
}

/**
 * @brief LOOP - display edge
 * @details tbd
 * @param[in] d digit to display
 * @param[in] o offset: 0 = right, 1 = center right, 2 = center left, 3 = left
 * @param[in] c color as HSV value
 * @since 1.2.0
 */
void displayEdge( uint8_t d, uint8_t o, CHSV c ) {
  // initially set segments off
  segA = segB = segC = segD = segE = segF = segG = CRGB::Black;

  // turn respective segments on
  if ( d == 0 ) {
    segA = segB = segC = segE = segF = segG = c;
  }
  if ( d == 1 ) {
    segA = segE = c;
  }
  if ( d == 2 ) {
    segA = segB = segD = segF = segG = c;
  }
  if ( d == 3 ) {
    segA = segB = segD = segE = segF = c;
  }
  if ( d == 4 ) {
    segA = segC = segD = segE = c;
  }
  if ( d == 5 ) {
    segB = segC = segD = segE = segF = c;
  }
  if ( d == 6 ) {
    segB = segC = segD = segE = segF = segG = c;
  }
  if ( d == 7 ) {
    segA = segB = segE = c;
  }
  if ( d == 8 ) {
    segA = segB = segC = segD = segE = segF = segG = c;
  }
  if ( d == 9 ) {
    segA = segB = segC = segD = segE = segF = c;
  }
  if ( d == 10 ) { // -
    segD = c;
  }
  if ( d == 11 ) { // °
    segA = segB = segC = segD = c;
  }
  if ( d == 12 ) { // d
    segA = segD = segE = segF = segG = c;
  }
  if ( d == 13 ) { // E
    segB = segC = segD = segF = segG = c;
  }
  if ( d == 14 ) { // H
    segA = segC = segD = segE = segG = c;
  }
  if ( d == 15 ) { // n
    segD = segE = segG = c;
  }

  // shift segments to correct spot
  if ( o == 1 ) {
    for ( uint8_t p = 0; p < ( 7 * EDGE_PPS ); p++ ) {
      arrayEdge[ p + ( 7 * EDGE_PPS ) ] = arrayEdge[ p ];
    }
  }

  if ( o == 2 ) {
    for ( uint8_t p = 0; p < ( 7 * EDGE_PPS ); p++ ) {
      arrayEdge[ p + ( 2 * 7 * EDGE_PPS ) ] = arrayEdge[ p ];
    }
  }

  if ( o == 3 ) {
    for ( uint8_t p = 0; p < ( 7 * EDGE_PPS ); p++ ) {
      arrayEdge[ p + ( 3 * 7 * EDGE_PPS ) ] = arrayEdge[ p ];
    }
  }
}

/**
 * @brief LOOP - display downlights
 * @details Control the downlights, e.g. colors and change frequency.
 * @since 1.0.0
 */
void displayDown() {
  // downlights - off
  if ( downUsage == 0 ) {
    fill_solid( arrayDown, DOWN_COUNT, CRGB::Black );
  }
  // downlights - on
  if ( downUsage == 1 ) {
    // user defined colors
    if ( dowColor == 0 ) {
      arrayDown[ 0 ].setRGB( dowColor0R, dowColor0G, dowColor0B );
      arrayDown[ 1 ].setRGB( dowColor1R, dowColor1G, dowColor1B );
      arrayDown[ 2 ].setRGB( dowColor2R, dowColor2G, dowColor2B );
      arrayDown[ 3 ].setRGB( dowColor3R, dowColor3G, dowColor3B );
      arrayDown[ 4 ].setRGB( dowColor4R, dowColor4G, dowColor4B );
      arrayDown[ 5 ].setRGB( dowColor5R, dowColor5G, dowColor5B );
      arrayDown[ 6 ].setRGB( dowColor6R, dowColor6G, dowColor6B );
      arrayDown[ 7 ].setRGB( dowColor7R, dowColor7G, dowColor7B );
      arrayDown[ 8 ].setRGB( dowColor8R, dowColor8G, dowColor8B );
      arrayDown[ 9 ].setRGB( dowColor9R, dowColor9G, dowColor9B );
      arrayDown[ 10 ].setRGB( dowColor10R, dowColor10G, dowColor10B );
      arrayDown[ 11 ].setRGB( dowColor11R, dowColor11G, dowColor11B );
      arrayDown[ 12 ].setRGB( dowColor12R, dowColor12G, dowColor12B );
      arrayDown[ 13 ].setRGB( dowColor13R, dowColor13G, dowColor13B );
      FastLED[ 1 ].showLeds( downBrightnessOut );
    }
    // 1 random color
    if ( dowColor == 1 && ( ( downFrequency == 0 ) || ( downFrequency == 1 && downMinPassed ) ||
                            ( downFrequency == 2 && downHourPassed ) || ( downFrequency == 3 && downDayPassed ) ) ) {
      CHSV dowColorRandom = CHSV( random8(), 255, 255 );
      for ( int i = 0; i < DOWN_COUNT; i++ ) {
        arrayDown[ i ] = dowColorRandom;
      }
      FastLED[ 1 ].showLeds( downBrightnessOut );
    }
    // 14 random colors
    if ( dowColor == 2 && ( ( downFrequency == 0 ) || ( downFrequency == 1 && downMinPassed ) ||
                            ( downFrequency == 2 && downHourPassed ) || ( downFrequency == 3 && downDayPassed ) ) ) {
      for ( int i = 0; i < DOWN_COUNT; i++ ) {
        arrayDown[ i ] = CHSV( random8(), 255, 255 );
      }
    }
  }

  if ( downBrightness == 0 ) {
    FastLED[ 1 ].showLeds( downBrightnessOut );
  } else {
    FastLED[ 1 ].showLeds( downBrightness );
  }

  downMinPassed  = false; // reset the color change flag (1 minute)
  downHourPassed = false; // reset the color change flag (1 hour)
  downDayPassed  = false; // reset the color change flag (1 day)
}

//  task1 on core0
void taskCore0code( void *pvParameters ) {
  for ( ;; ) { // required to keep the task running

    // time-passage trackers
    int currTsec  = ntpTime.tm_sec;
    int currTmin  = ntpTime.tm_min;
    int currThour = ntpTime.tm_hour;
    int currTday  = ntpTime.tm_mday;
    int m1        = currTmin / 10;
    int m2        = currTmin % 10;

    if ( abs( currTsec - prevTimeSecCore0 ) >= 1 ) { // every second
      prevTimeSecCore0 = currTsec;                   // set reference
      displayDown();
      if ( WiFi.status() != WL_CONNECTED ) {         // NO wifi
        getWIFI();                                   // -> reconnect
      }
    }

    if ( abs( currTmin - prevTimeMinCore0 ) >= 1 ) { // every minute
      prevTimeMinCore0 = currTmin;                   // set reference
      edgeMinPassed    = true;                       // set flag
      downMinPassed    = true;                       // set flag
      getKNXbrightness();                            // brightness
    }

    if ( ( m2 == 0 || m2 == 5 ) && ( currTsec == 0 ) ) { // every 5 minutes
      getKNXtemperature();
      getKNXhumidity();
    }

    if ( abs( currThour - prevTimeHourCore0 ) >= 1 ) { // every hour
      prevTimeHourCore0 = currThour;                   // set reference
      edgeHourPassed    = true;                        // set flag
      downHourPassed    = true;                        // set flag
      getNTP();
    }

    if ( abs( currTday - prevTimeDayCore0 ) >= 1 ) { // every day
      prevTimeDayCore0 = currTday;                   // set reference
      edgeDayPassed    = true;                       // set flag
      downDayPassed    = true;                       // set flag
    }
  }
}
