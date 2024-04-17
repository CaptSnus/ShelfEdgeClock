//  ----------------------------------------------------------------------------
const char projectTitle[]   = "S H E L F   E D G E   C L O C K";

const char projectAuthor[]  = "CaptSnus";
const char projectVersion[] = "2.0.0";
const char projectLink[]    = "https://github.com/CaptSnus/ShelfEdgeClock";
const char projectLicense[] = "MIT";
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
#define EDGE_PIN    36                         // pin of the edge leds
#define EDGE_DIGITS 4                          // number of digits
#define EDGE_PPS    9                          // number of leds per segment
#define EDGE_COUNT  EDGE_DIGITS * 7 * EDGE_PPS // number of leds in total
#define DOWN_PIN    35                         // pin of the downlight leds
#define DOWN_BOXES  14                         // number of downlight boxes
#define DOWN_PPS    1                          // number of leds per box
#define DOWN_COUNT  DOWN_BOXES *DOWN_PPS       // number of leds in total

//  General  -------------------------------------------------------------------
int displayMode;     // current display mode
int displayModePrev; // previous display mode

Preferences    pref;
WiFiClient     wifi;
AsyncWebServer server( 80 );

//  Time  ----------------------------------------------------------------------
struct tm ntpTime;                      // structure to save time
time_t    now;                          // this is the epoch
uint32_t  currMillisCore1   = millis(); // curr time core1
uint32_t  prevTimeSecCore1  = 0;        // prev time core1 (1 sec)
int       prevTimeSecCore0  = 0;        // prev time core0 (1 sec)
int       prevTimeMinCore0  = 0;        // prev time core0 (1 min)
int       prevTimeHourCore0 = 0;        // prev time core0 (1 hour)
int       prevTimeDayCore0  = 0;        // prev time core0 (1 day)

TaskHandle_t taskCore0;

//  mode CLOCK  ----------------------------------------------------------------
String clkAddress  = "ch.pool.ntp.org";
String clkTimeZone = "CET-1CEST,M3.5.0,M10.5.0/3";
int    clkFormat   = 1; // 0 = AM/PM, 1 = 24h
int    clkColor    = 0; // 0 = 2 def, 1 = 2 random

int clkColorHR, clkColorHG, clkColorHB;
int clkColorMR, clkColorMG, clkColorMB;

//  mode DATE  -----------------------------------------------------------------
int datFormat = 1; // 0 = MMdd, 1 = ddMM
int datColor  = 0; // 0 = 2 def, 1 = 2 random

int datColorDR, datColorDG, datColorDB; // color for days
int datColorMR, datColorMG, datColorMB; // color for months

//  mode TEMPERATURE  ----------------------------------------------------------
int temSymbol = 1; // 0 = Fahrenheit, 1 = Celsius

//  mode HUMIDITY  -------------------------------------------------------------
int humColor  = 0; // 0 = 3 def, 1 = 3 random

int humColorAR, humColorAG, humColorAB; // color for digits
int humColorBR, humColorBG, humColorBB; // color for comma value
int humColorCR, humColorCG, humColorCB; // color for sign

//  mode SCOREBOARD  -----------------------------------------------------------
int scoL = 0; // left value to display
int scoR = 0; // right value to display

int scoColorLR, scoColorLG, scoColorLB; // color for left value
int scoColorRR, scoColorRG, scoColorRB; // color for right value

//  mode COUNTDOWN  ------------------------------------------------------------
uint32_t couMillis    = 0;
uint32_t couMillisEnd = 0;

int couColorHR, couColorHG, couColorHB;
int couColorMR, couColorMG, couColorMB;
int couColorSR, couColorSG, couColorSB;
int couColorAR, couColorAG, couColorAB;

//  mode SCROLLING  ------------------------------------------------------------
uint32_t scrMillis      = 0;
uint32_t scrMillisPrev  = 0;
int      scrDisplayMode = 0;
int      scrTime;

//  wifi  ----------------------------------------------------------------------
bool      apSwitch = 1;        // ap: 0 = off, 1 = on
bool      apHidden = 0;        // ap: 0 = visible, 1 = hidden
String    apSSID, apPass;      // ap: ssid & password
IPAddress apIPLocal;           // ap: ip address
IPAddress apGateway;           // ap: gateway address
IPAddress apSubnet;            // ap: subnet mask
int       apIP1, apIP2, apIP3; // ap: ip address in blocks (apIP4 = 1)
bool      wifiSwitch = 0;      // wifi: 0 = off, 1 = on
String    wifiSSID, wifiPass;  // wifi: ssid & password

//  rest api  ------------------------------------------------------------------
String knxAddress    = "";     // ip address
int    knxPort       = 8080;   // port
String knxBrightPath = "";     // path to brightness item
long   knxBrightIn   = 0;      // current brightness value
long   knxBrightMin  = 0;      // min value of brightness item
long   knxBrightMax  = 150000; // max value of brightness item
String knxTempPath   = "";     // path to temperature item
double knxTempIn     = 0;      // current temperature value
String knxHumidPath  = "";     // path to humidity item
int    knxHumidIn    = 0;      // current humidity value

HttpClient knxClient = HttpClient( wifi, knxAddress.c_str(), knxPort );

//  edgelights  ----------------------------------------------------------------
bool edgeMinPassed   = true; // color change (1 min)
bool edgeHourPassed  = true; // color change (1 hour)
bool edgeDayPassed   = true; // color change (1 day)
int  edgeBrightIn    = 0;    // 0 = auto, > 0 = manual
int  edgeBrightMin   = 5;    // minimum value
int  edgeBrightMax   = 125;  // maximum value
int  edgeBrightOut   = 25;   // effective value
int  edgeFrequency   = 0;    // 0 = sec, 1 = min, 2 = hour, 3 = day

CRGBArray<EDGE_COUNT> arrayEdge;                                                          // edge array
CRGBSet               segA( arrayEdge( EDGE_PPS * 0, EDGE_PPS - 1 + ( EDGE_PPS * 0 ) ) ); //    B B
CRGBSet               segB( arrayEdge( EDGE_PPS * 1, EDGE_PPS - 1 + ( EDGE_PPS * 1 ) ) ); //  C     A
CRGBSet               segC( arrayEdge( EDGE_PPS * 2, EDGE_PPS - 1 + ( EDGE_PPS * 2 ) ) ); //  C     A
CRGBSet               segD( arrayEdge( EDGE_PPS * 3, EDGE_PPS - 1 + ( EDGE_PPS * 3 ) ) ); //    D D
CRGBSet               segE( arrayEdge( EDGE_PPS * 4, EDGE_PPS - 1 + ( EDGE_PPS * 4 ) ) ); //  G     E
CRGBSet               segF( arrayEdge( EDGE_PPS * 5, EDGE_PPS - 1 + ( EDGE_PPS * 5 ) ) ); //  G     E
CRGBSet               segG( arrayEdge( EDGE_PPS * 6, EDGE_PPS - 1 + ( EDGE_PPS * 6 ) ) ); //    F F

//  downlights  ----------------------------------------------------------------
bool downMinPassed  = true; // color change (1 min)
bool downHourPassed = true; // color change (1 hour)
bool downDayPassed  = true; // color change (1 day)
bool downSwitch     = 0;    // values: 0 = off, 1 = on
int  downBrightIn   = 0;    // values: 0 = auto, > 0 = manual
int  downBrightMin  = 10;   // minimum value
int  downBrightMax  = 250;  // maximum value
int  downBrightOut;         // effective value
int  downFrequency = 0;     // values: 0 = sec, 1 = min, 2 = hour, 3 = day
int  downColorMode;         // values: 0, 1, 2, 3

int downColorR00, downColorG00, downColorB00; // mode: 0 -> color
int downColorR01, downColorG01, downColorB01; // mode: 1 -> color 1
int downColorR02, downColorG02, downColorB02; // mode: 1 -> color 2
int downColorR03, downColorG03, downColorB03; // mode: 1 -> color 3
int downColorR04, downColorG04, downColorB04; // mode: 1 -> color 4
int downColorR05, downColorG05, downColorB05; // mode: 1 -> color 5
int downColorR06, downColorG06, downColorB06; // mode: 1 -> color 6
int downColorR07, downColorG07, downColorB07; // mode: 1 -> color 7
int downColorR08, downColorG08, downColorB08; // mode: 1 -> color 8
int downColorR09, downColorG09, downColorB09; // mode: 1 -> color 9
int downColorR10, downColorG10, downColorB10; // mode: 1 -> color 10
int downColorR11, downColorG11, downColorB11; // mode: 1 -> color 11
int downColorR12, downColorG12, downColorB12; // mode: 1 -> color 12
int downColorR13, downColorG13, downColorB13; // mode: 1 -> color 13
int downColorR14, downColorG14, downColorB14; // mode: 1 -> color 14

CRGBArray<DOWN_COUNT> arrayDown;

//  ----------------------------------------------------------------------------
//  FUNCTION DECLARATIONS
//  ----------------------------------------------------------------------------
void initPREFERENCES(); // setup - initialize preferences and load values
void initWIFI();        // setup - configure wifi connections
void initSPIFFS();      // setup - initialize SPIFFS and load data
void initNTP();         // setup - configure NTP and fetch current time
void initHANDLERS();    // setup - load all web handlers
void initSERVER();      // setup - configure and start webserver
void initLED();         // setup - define and initialize all led strands

void getNTP();            // loop - update current time
void getKNXbrightness();  // loop - get the brightness value
void getKNXtemperature(); // loop - get the temperature value
void getKNXhumidity();    // loop - get the humidity value
void getPing();           // TBD

void modeClk(); // display - mode CLOCK
void modeDat(); // display - mode DATE
void modeTem(); // display - mode TEMPERATURE
void modeHum(); // display - mode HUMIDITY
void modeSco(); // display - mode SCOREBOARD
void modeCou(); // display - mode COUNTDOWN
void modeScr(); // display - mode SCROLLING

void displayEdge( uint8_t d, uint8_t o, CHSV c ); // outout - edgelights
void displayDown();                               // output - downlights
void taskCore0code( void *pvParameters );         // task on core0

//  ----------------------------------------------------------------------------
//  SETUP
//  ----------------------------------------------------------------------------
void setup() {
  Serial.begin( 115200 ); // initialize serial
  delay( 5000 );          // wait a moment

  Serial.println( "\n\n" );                            // make some space
  Serial.printf( "PROJECT     %s\n", projectTitle );   // project title
  Serial.printf( "Version     %s\n", projectVersion ); // version
  Serial.printf( "Author      %s\n", projectAuthor );  // author
  Serial.printf( "Info        %s\n", projectLink );    // link
  Serial.printf( "License     %s\n", projectLicense ); // license
  Serial.println( "\nSETUP" );

  initPREFERENCES(); // load all saved settings
  initWIFI();        // configure wifi
  initSPIFFS();      // initialize spiffs
  initNTP();         // get the time from ntp
  initHANDLERS();    // load all web handlers
  initSERVER();      // configure webserver
  initLED();         // init neopixels

  if ( WiFi.status() == WL_CONNECTED ) { // if wifi up ...
    getNTP();                            // ... first read of date and time
    getKNXbrightness();                  // ... first read of brightness
    getKNXtemperature();                 // ... first read of temperature
    getKNXhumidity();                    // ... first read of humidity
  }

  displayModePrev = displayMode; // prefill the previous display mode

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

    if ( edgeBrightIn == 0 ) {
      FastLED[ 0 ].showLeds( edgeBrightOut );
    } else {
      FastLED[ 0 ].showLeds( edgeBrightIn );
    }
  }
}

//  ----------------------------------------------------------------------------
//  FUNCTION DEFINITIONS
//  ----------------------------------------------------------------------------
/**
 * @brief display mode - CLOCK
 * @details display the current time on the front of the shelf
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
  int temp = round( knxTempIn ); // round
  int t1, t2, t3;                // throwaway variable
  // |- Fahrenheit
  if ( temSymbol == 0 ) {
    if ( temp < 0 ) {
      t1 = 37; // show "-"
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
  int h1 = floor( knxHumidIn / 100 ); // build h1
  int h2 = ( knxHumidIn / 10 ) % 10;  // build h2
  int h3 = knxHumidIn % 10;           // build h3

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
  int s1    = floor( scoL / 10 ); // build s1
  int s2    = scoL % 10;          // build s2
  int s3    = floor( scoR / 10 ); // build s3
  int s4    = scoR % 10;          // build s4

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

  // abort empty countdown
  if ( couMillis == 0 ) {
    displayMode = 0;
    pref.putInt( "displayMode", 0 );
    return;
  }

  // build the values
  uint32_t restMillis = couMillisEnd - millis();             // build the remaining milliseconds and ...
  uint32_t hours      = ( ( restMillis / 1000 ) / 60 ) / 60; // ... the countdown in hours
  uint32_t minutes    = ( restMillis / 1000 ) / 60;          // ... the countdown in minutes
  uint32_t seconds    = restMillis / 1000;                   // ... the countdown in seconds
  int      remM       = minutes - ( hours * 60 );            // ... the minutes between 0 and 59
  int      remS       = seconds - ( minutes * 60 );          // ... the seconds between 0 and 59

  // build the digits
  int h1              = hours / 10; // build h1
  int h2              = hours % 10; // build h2
  int m1              = remM / 10;  // build m1
  int m2              = remM % 10;  // build m2
  int s1              = remS / 10;  // build s1
  int s2              = remS % 10;  // build s2

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

    // blank edge leds
    FastLED[ 0 ].clearLedData();
    FastLED[ 0 ].showLeds( 250 );
    delay( 500 );

    // set the color
    couAColor = rgb2hsv_approximate( CRGB( couColorAR, couColorAG, couColorAB ) );

    // blink EndE
    for ( int i = 0; i < 5; i++ ) {
      displayEdge( 13, 3, couAColor );
      displayEdge( 15, 2, couAColor );
      displayEdge( 12, 1, couAColor );
      displayEdge( 13, 0, couAColor );
      FastLED[ 0 ].showLeds( 250 );
      delay( 300 );
      FastLED[ 0 ].showLeds( 0 );
      delay( 300 );
    }

    delay( 5000 );
    displayMode = displayModePrev;
    pref.putInt( "displayMode", 0 );
    couMillis    = 0;
    couMillisEnd = 0;
  }
}

/**
 * @brief MODE - scrolling
 * @details show multiple values on the front of the shelf
 * @since 1.0.0
 */
void modeScr() {
  scrMillis = millis(); // update current reference time

  if ( ( scrMillis - scrMillisPrev ) >= ( scrTime * 1000 ) ) { // run everything inside here every x seconds
    scrMillisPrev = scrMillis;                                 // update previous reference time

    if ( scrDisplayMode < 2 ) {            // if scrDisplayMode is smaller than 2 ...
      scrDisplayMode = scrDisplayMode + 1; // ... increase scrDisplayMode by 1
    } else {                               // ... else ...
      scrDisplayMode = 0;                  // ... restart with the first content
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
 * @details Create preferences array in read/write mode and load all saved data
 * @since 1.0.0
 */
void initPREFERENCES() {
  Serial.println( "- Preferences" );
  pref.begin( "shelfclock", false );
  // last mode
  displayMode = pref.getInt( "displayMode", 0 );
  // mode clock
  clkAddress  = pref.getString( "clkAddr", clkAddress );
  clkTimeZone = pref.getString( "clkZone", clkTimeZone );
  clkFormat   = pref.getInt( "clkFormat", 1 );
  clkColor    = pref.getInt( "clkColor", 0 );
  clkColorHR  = pref.getInt( "clkColHR", 85 );
  clkColorHG  = pref.getInt( "clkColHG", 26 );
  clkColorHB  = pref.getInt( "clkColHB", 139 );
  clkColorMR  = pref.getInt( "clkColMR", 255 );
  clkColorMG  = pref.getInt( "clkColMG", 0 );
  clkColorMB  = pref.getInt( "clkColMB", 132 );
  // mode date
  datFormat   = pref.getInt( "datFormat", 1 );
  datColor    = pref.getInt( "datColor", 0 );
  datColorDR  = pref.getInt( "datColorDR", 255 );
  datColorDG  = pref.getInt( "datColorDG", 215 );
  datColorDB  = pref.getInt( "datColorDB", 0 );
  datColorMR  = pref.getInt( "datColorMR", 255 );
  datColorMG  = pref.getInt( "datColorMG", 69 );
  datColorMB  = pref.getInt( "datColorMB", 0 );
  // mode temperature
  temSymbol   = pref.getInt( "temSymbol", 1 );
  // mode humidity
  humColor    = pref.getInt( "humColor", 0 );
  humColorAR  = pref.getInt( "humColorAR", 255 );
  humColorAG  = pref.getInt( "humColorAG", 255 );
  humColorAB  = pref.getInt( "humColorAB", 0 );
  humColorBR  = pref.getInt( "humColorBR", 0 );
  humColorBG  = pref.getInt( "humColorBG", 255 );
  humColorBB  = pref.getInt( "humColorBB", 255 );
  humColorCR  = pref.getInt( "humColorCR", 255 );
  humColorCG  = pref.getInt( "humColorCG", 0 );
  humColorCB  = pref.getInt( "humColorCB", 255 );
  // mode scoreboard
  scoColorLR  = pref.getInt( "scoLColR", 0 );
  scoColorLG  = pref.getInt( "scoLColG", 0 );
  scoColorLB  = pref.getInt( "scoLColB", 255 );
  scoColorRR  = pref.getInt( "scoRColR", 0 );
  scoColorRG  = pref.getInt( "scoRColG", 255 );
  scoColorRB  = pref.getInt( "scoRColB", 255 );
  // mode countdown
  couColorHR  = pref.getInt( "couColorHR", 255 );
  couColorHG  = pref.getInt( "couColorHG", 140 );
  couColorHB  = pref.getInt( "couColorHB", 0 );
  couColorMR  = pref.getInt( "couColorMR", 255 );
  couColorMG  = pref.getInt( "couColorMG", 165 );
  couColorMB  = pref.getInt( "couColorMB", 0 );
  couColorSR  = pref.getInt( "couColorSR", 255 );
  couColorSG  = pref.getInt( "couColorSG", 215 );
  couColorSB  = pref.getInt( "couColorSB", 0 );
  couColorAR  = pref.getInt( "couColorAR", 255 );
  couColorAG  = pref.getInt( "couColorAG", 0 );
  couColorAB  = pref.getInt( "couColorAB", 0 );
  // mode scroll
  scrTime     = pref.getInt( "scrTime", 4 );

  //  WiFi
  //  ----------------------------------------------------------------------------
  pref.putInt( "apSwitch", 1 );
  apSwitch      = pref.getInt( "apSwitch", 1 );
  apHidden      = pref.getInt( "apHidden", 0 );
  apSSID        = pref.getString( "apSSID", "Shelf Edge Clock" );
  apPass        = pref.getString( "apPass", "sEC_v2.0.0" );
  apIP1         = pref.getInt( "apIP1", 192 );
  apIP2         = pref.getInt( "apIP2", 168 );
  apIP3         = pref.getInt( "apIP3", 100 );
  wifiSwitch    = pref.getInt( "wifiSwitch", 0 );
  wifiSSID      = pref.getString( "wifiSSID", "ENTER_SSID" );
  wifiPass      = pref.getString( "wifiPass", "ENTER_PASSWORD" );

  //  REST API connection
  //  ----------------------------------------------------------------------------
  knxAddress    = pref.getString( "knxAddr", "0.0.0.0" );
  knxPort       = pref.getInt( "knxPort", knxPort );
  knxBrightPath = pref.getString( "knxBrightPath", "/rest/items/{item}/state" );
  knxBrightMin  = pref.getLong( "knxBrightMin", knxBrightMin );
  knxBrightMax  = pref.getLong( "knxBrightMax", knxBrightMax );
  knxTempPath   = pref.getString( "knxTempPath", "/rest/items/{item}/state" );
  knxHumidPath  = pref.getString( "knxHumPath", "/rest/items/{item}/state" );

  //  Edgelights
  //  ----------------------------------------------------------------------------
  edgeBrightIn  = pref.getInt( "edgBrightIn", edgeBrightIn );
  edgeBrightMin = pref.getInt( "edgBrightMin", edgeBrightMin );
  edgeBrightMax = pref.getInt( "edgBrightMax", edgeBrightMax );
  edgeBrightOut = pref.getInt( "edgBrightOut", edgeBrightOut );
  edgeFrequency = pref.getInt( "edgeFrequency", edgeFrequency );

  //  Downlights
  //  --------------------------------------------------------------------------
  downSwitch    = pref.getInt( "downSwitch", downSwitch );
  downBrightIn  = pref.getInt( "downBrightIn", downBrightIn );
  downBrightMin = pref.getInt( "downBrightMin", downBrightMin );
  downBrightMax = pref.getInt( "downBrightMax", downBrightMax );
  downBrightOut = pref.getInt( "downBrightOut", downBrightOut );
  downFrequency = pref.getInt( "downFrequency", downFrequency );
  downColorMode = pref.getInt( "downColorMode", downColorMode );
  downColorR00  = pref.getInt( "downColorR00", 255 );
  downColorR01  = pref.getInt( "downColorR01", 255 );
  downColorR02  = pref.getInt( "downColorR02", 255 );
  downColorR03  = pref.getInt( "downColorR03", 255 );
  downColorR04  = pref.getInt( "downColorR04", 255 );
  downColorR05  = pref.getInt( "downColorR05", 255 );
  downColorR06  = pref.getInt( "downColorR06", 255 );
  downColorR07  = pref.getInt( "downColorR07", 255 );
  downColorR08  = pref.getInt( "downColorR08", 255 );
  downColorR09  = pref.getInt( "downColorR09", 255 );
  downColorR10  = pref.getInt( "downColorR10", 255 );
  downColorR11  = pref.getInt( "downColorR11", 255 );
  downColorR12  = pref.getInt( "downColorR12", 255 );
  downColorR13  = pref.getInt( "downColorR13", 255 );
  downColorR14  = pref.getInt( "downColorR14", 255 );
  downColorG00  = pref.getInt( "downColorG00", 255 );
  downColorG01  = pref.getInt( "downColorG01", 255 );
  downColorG02  = pref.getInt( "downColorG02", 255 );
  downColorG03  = pref.getInt( "downColorG03", 255 );
  downColorG04  = pref.getInt( "downColorG04", 255 );
  downColorG05  = pref.getInt( "downColorG05", 255 );
  downColorG06  = pref.getInt( "downColorG06", 255 );
  downColorG07  = pref.getInt( "downColorG07", 255 );
  downColorG08  = pref.getInt( "downColorG08", 255 );
  downColorG09  = pref.getInt( "downColorG09", 255 );
  downColorG10  = pref.getInt( "downColorG10", 255 );
  downColorG11  = pref.getInt( "downColorG11", 255 );
  downColorG12  = pref.getInt( "downColorG12", 255 );
  downColorG13  = pref.getInt( "downColorG13", 255 );
  downColorG14  = pref.getInt( "downColorG14", 255 );
  downColorB00  = pref.getInt( "downColorB00", 255 );
  downColorB01  = pref.getInt( "downColorB01", 255 );
  downColorB02  = pref.getInt( "downColorB02", 255 );
  downColorB03  = pref.getInt( "downColorB03", 255 );
  downColorB04  = pref.getInt( "downColorB04", 255 );
  downColorB05  = pref.getInt( "downColorB05", 255 );
  downColorB06  = pref.getInt( "downColorB06", 255 );
  downColorB07  = pref.getInt( "downColorB07", 255 );
  downColorB08  = pref.getInt( "downColorB08", 255 );
  downColorB09  = pref.getInt( "downColorB09", 255 );
  downColorB10  = pref.getInt( "downColorB10", 255 );
  downColorB11  = pref.getInt( "downColorB11", 255 );
  downColorB12  = pref.getInt( "downColorB12", 255 );
  downColorB13  = pref.getInt( "downColorB13", 255 );
  downColorB14  = pref.getInt( "downColorB14", 255 );
}

/**
 * @brief SETUP - wifi
 * @details initialize and configure the access point and/or the wifi client
 * @since 1.0.0
 */
void initWIFI() {
  Serial.println( "- WiFi" );

  if ( apSwitch && wifiSwitch ) {
    Serial.println( "  ap & wifi" );
    WiFi.mode( WIFI_AP_STA ); // set mode: access point & wifi client
  } else if ( apSwitch && !wifiSwitch ) {
    Serial.println( "  ap only" );
    WiFi.mode( WIFI_AP ); // set mode: access point only
  } else if ( !apSwitch && wifiSwitch ) {
    Serial.println( "  wifi only" );
    WiFi.mode( WIFI_STA ); // set mode: wifi client only
  } else {
    Serial.println( "  wifi off" );
    WiFi.mode( WIFI_OFF ); // set mode: turn off wifi
  }

  // access point
  if ( apSwitch ) {
    // configure and start
    apIPLocal = IPAddress( apIP1, apIP2, apIP3, 1 );            // ip address
    apGateway = IPAddress( apIP1, apIP2, apIP3, 1 );            // gateway
    apSubnet  = IPAddress( 255, 255, 255, 0 );                  // subnet
    WiFi.softAPConfig( apIPLocal, apGateway, apSubnet );        // config ap
    WiFi.softAP( apSSID.c_str(), apPass.c_str(), 1, apHidden ); // start ap
  }

  // wifi client
  if ( wifiSwitch ) {
    // establish connection
    WiFi.begin( wifiSSID.c_str(), wifiPass.c_str() ); // connect to wifi
    for ( int i = 0; i < 10; i++ ) {                  // loop 10 times
      delay( 200 );                                   // wait before each check
      if ( WiFi.status() == WL_CONNECTED ) {          // check if connected
        break;                                        // if connected, leave the cycle
      }
    }
    if ( WiFi.status() != WL_CONNECTED ) { // if not connected
      WiFi.disconnect();                   // turn off wifi
    }
  }
}

/**
 * @brief SETUP - spiffs
 * @details initialize the spiffs storage for the webserver
 * @since 1.0.0
 */
void initSPIFFS() {
  if ( SPIFFS.begin( true ) ) {
    Serial.println( "- SPIFFS" );
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
  Serial.println( "- NTP" );
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
  Serial.println( "- webHandler" );
  // clang-format off
  //  Default
  //  --------------------------------------------------------------------------
  server.on( "/",                     HTTP_GET, []( AsyncWebServerRequest *request ) { request->send( SPIFFS, "/index.html", "text/html", false ); } );
  server.on( "/wifi",                 HTTP_GET, []( AsyncWebServerRequest *request ) { request->send( SPIFFS, "/wifi.html", "text/html", false ); } );
  server.on( "/settings",             HTTP_GET, []( AsyncWebServerRequest *request ) { request->send( SPIFFS, "/settings.html", "text/html", false ); } );

  //  index.html
  //  --------------------------------------------------------------------------
  server.on( "/updDisplayMode",       HTTP_POST, []( AsyncWebServerRequest *request ) { displayModePrev = displayMode; displayMode = request->arg( "mode" ).toInt(); pref.putInt( "displayMode", displayMode ); if ( displayMode == 4 ) { scoL = request->arg( "data1" ).toInt(); scoR = request->arg( "data2" ).toInt(); } if (displayMode == 5 ) { couMillis = request->arg( "data1" ).toInt(); couMillisEnd = millis() + couMillis; } request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updDownlight",         HTTP_POST, []( AsyncWebServerRequest *request ) { downSwitch = request->arg( "downSwitch" ).toInt(); pref.putInt( "downSwitch", downSwitch ); downColorMode = request->arg( "downColorMode" ).toInt(); pref.putInt( "downColorMode", downColorMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  WiFi
  //  --------------------------------------------------------------------------
  server.on( "/getAPSwitch",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSwitch ) ); } );
  server.on( "/updAPSwitch",          HTTP_POST, []( AsyncWebServerRequest *request ) { if ( request->arg( "apSwitch" ) == "true" ) { apSwitch = 1; } if ( request->arg( "apSwitch" ) == "false" ) { apSwitch = 0; } pref.putInt( "apSwitch", apSwitch ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPSSID",            []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSSID ) ); } );
  server.on( "/updAPSSID",            HTTP_POST, []( AsyncWebServerRequest *request ) { apSSID = request->arg( "apSSID" ); pref.putString( "apSSID", apSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPPass",            []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apPass ) ); } );
  server.on( "/updAPPass",            HTTP_POST, []( AsyncWebServerRequest *request ) { apPass = request->arg( "apPass" ); pref.putString( "apPass", apPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPHidden",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apHidden ) ); } );
  server.on( "/updAPHidden",          HTTP_POST, []( AsyncWebServerRequest *request ) { apHidden = request->arg( "apHidden" ).toInt(); pref.putInt( "apHidden", apHidden ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP1",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP1 ) ); } );
  server.on( "/updAPIP1",             HTTP_POST, []( AsyncWebServerRequest *request ) { apIP1 = request->arg( "apIP1" ).toInt(); pref.putInt( "apIP1", apIP1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP2",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP2 ) ); } );
  server.on( "/updAPIP2",             HTTP_POST, []( AsyncWebServerRequest *request ) { apIP2 = request->arg( "apIP2" ).toInt(); pref.putInt( "apIP2", apIP2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP3",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP3 ) ); } );
  server.on( "/updAPIP3",             HTTP_POST, []( AsyncWebServerRequest *request ) { apIP3 = request->arg( "apIP3" ).toInt(); pref.putInt( "apIP3", apIP3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSwitch",        []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSwitch ) ); } );                                                                                      
  server.on( "/updWifiSwitch",        HTTP_POST, []( AsyncWebServerRequest *request ) { if ( request->arg( "wifiSwitch" ) == "true" ) { wifiSwitch = 1; } if ( request->arg( "wifiSwitch" ) == "false" ) { wifiSwitch = 0; } pref.putInt( "wifiSwitch", wifiSwitch ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSSID",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSSID ) ); } );
  server.on( "/updWifiSSID",          HTTP_POST, []( AsyncWebServerRequest *request ) { wifiSSID = request->arg( "wifiSSID" ); pref.putString( "wifiSSID", wifiSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiPass",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiPass ) ); } );
  server.on( "/updWifiPass",          HTTP_POST, []( AsyncWebServerRequest *request ) { wifiPass = request->arg( "wifiPass" ); pref.putString( "wifiPass", wifiPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );  
  server.on( "/updWifiRestart",       HTTP_POST, []( AsyncWebServerRequest *request ) { initWIFI(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE clock
  //  --------------------------------------------------------------------------
  server.on( "/getClkAddress",        []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkAddress ) ); } );
  server.on( "/updClkAddress",        HTTP_POST, []( AsyncWebServerRequest *request ) { clkAddress = request->arg( "clkAddress" ); pref.putString( "clkAddr", clkAddress ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkTimeZone",       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkTimeZone ) ); } );
  server.on( "/updClkTimeZone",       HTTP_POST, []( AsyncWebServerRequest *request ) { clkTimeZone = request->arg( "clkTimeZone" ); pref.putString( "clkZone", clkTimeZone ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkFormat",         []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkFormat ) ); } );
  server.on( "/updClkFormat",         HTTP_POST, []( AsyncWebServerRequest *request ) { clkFormat = request->arg( "clkFormat" ).toInt(); pref.putInt( "clkFormat", clkFormat ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkColor",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkColor ) ); } );
  server.on( "/updClkColor",          HTTP_POST, []( AsyncWebServerRequest *request ) { clkColor = request->arg( "clkColor" ).toInt(); pref.putInt( "clkColor", clkColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkColorH",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", clkColorHR, clkColorHG, clkColorHB );  request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updClkColorH",         HTTP_POST, []( AsyncWebServerRequest *request ) { clkColorHR = request->arg( "r" ).toInt(); pref.putInt( "clkColorHR", clkColorHR ); clkColorHG = request->arg( "g" ).toInt(); pref.putInt( "clkColorHG", clkColorHG ); clkColorHB = request->arg( "b" ).toInt(); pref.putInt( "clkColorHB", clkColorHB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkColorM",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", clkColorMR, clkColorMG, clkColorMB );  request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updClkColorM",         HTTP_POST, []( AsyncWebServerRequest *request ) { clkColorMR      = request->arg( "r" ).toInt();            pref.putInt( "clkColorMR", clkColorMR );          clkColorMG = request->arg( "g" ).toInt(); pref.putInt( "clkColorMG", clkColorMG ); clkColorMB = request->arg( "b" ).toInt(); pref.putInt( "clkColorMB", clkColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE date
  //  --------------------------------------------------------------------------
  server.on( "/getDatFormat",         []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datFormat ) ); } );
  server.on( "/getDatColor",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datColor ) ); } );
  server.on( "/getDatColorD",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", datColorDR, datColorDG, datColorDB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getDatColorM",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", datColorMR, datColorMG, datColorMB ); request->send( 200, "text/plain", tempcolor ); } );

  server.on( "/updDatFormat",         HTTP_POST, []( AsyncWebServerRequest *request ) { datFormat       = request->arg( "datFormat" ).toInt();    pref.putInt( "datFormat", datFormat );            request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updDatColor",          HTTP_POST, []( AsyncWebServerRequest *request ) { datColor        = request->arg( "datColor" ).toInt();     pref.putInt( "datColor", datColor );              request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updDatColorD",         HTTP_POST, []( AsyncWebServerRequest *request ) { datColorDR      = request->arg( "r" ).toInt();            pref.putInt( "datColorDR", datColorDR );          datColorDG = request->arg( "g" ).toInt(); pref.putInt( "datColorDG", datColorDG ); datColorDB = request->arg( "b" ).toInt(); pref.putInt( "datColorDB", datColorDB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updDatColorM",         HTTP_POST, []( AsyncWebServerRequest *request ) { datColorMR      = request->arg( "r" ).toInt();            pref.putInt( "datColorMR", datColorMR );          datColorMG = request->arg( "g" ).toInt(); pref.putInt( "datColorMG", datColorMG ); datColorMB = request->arg( "b" ).toInt(); pref.putInt( "datColorMB", datColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE temperature
  //  --------------------------------------------------------------------------
  server.on( "/getTemSymbol",         []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( temSymbol ) ); } );
  server.on( "/updTemSymbol",         HTTP_POST, []( AsyncWebServerRequest *request ) { temSymbol       = request->arg( "temSymbol" ).toInt();    pref.putInt( "temSymbol", temSymbol ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE humidity
  //  --------------------------------------------------------------------------
  server.on( "/getHumColor",          []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( humColor ) ); } );
  server.on( "/getHumColorA",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorAR, humColorAG, humColorAB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getHumColorB",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorBR, humColorBG, humColorBB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getHumColorC",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", humColorCR, humColorCG, humColorCB ); request->send( 200, "text/plain", tempcolor ); } );

  server.on( "/updHumColor",          HTTP_POST, []( AsyncWebServerRequest *request ) { humColor        = request->arg( "humColor" ).toInt();     pref.putInt( "humColor", humColor ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updHumColorA",         HTTP_POST, []( AsyncWebServerRequest *request ) { humColorAR      = request->arg( "r" ).toInt();            pref.putInt( "humColorAR", humColorAR ); humColorAG = request->arg( "g" ).toInt(); pref.putInt( "humColorAG", humColorAG ); humColorAB = request->arg( "b" ).toInt(); pref.putInt( "humColorAB", humColorAB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updHumColorB",         HTTP_POST, []( AsyncWebServerRequest *request ) { humColorBR      = request->arg( "r" ).toInt();            pref.putInt( "humColorBR", humColorBR ); humColorBG = request->arg( "g" ).toInt(); pref.putInt( "humColorBG", humColorBG ); humColorBB = request->arg( "b" ).toInt(); pref.putInt( "humColorBB", humColorBB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updHumColorC",         HTTP_POST, []( AsyncWebServerRequest *request ) { humColorCR      = request->arg( "r" ).toInt();            pref.putInt( "humColorCR", humColorCR ); humColorCG = request->arg( "g" ).toInt(); pref.putInt( "humColorCG", humColorCG ); humColorCB = request->arg( "b" ).toInt(); pref.putInt( "humColorCB", humColorCB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE scoreboard
  //  --------------------------------------------------------------------------
  server.on( "/getScoColorL",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", scoColorLR, scoColorLG, scoColorLB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getScoColorR",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", scoColorRR, scoColorRG, scoColorRB ); request->send( 200, "text/plain", tempcolor ); } );

  server.on( "/updScoColorL",      HTTP_POST, []( AsyncWebServerRequest *request ) { scoColorLR      = request->arg( "r" ).toInt(); pref.putInt( "scoColorLR", scoColorLR ); scoColorLG = request->arg( "g" ).toInt(); pref.putInt( "scoColorLG", scoColorLG ); scoColorLB = request->arg( "b" ).toInt(); pref.putInt( "scoColorLB", scoColorLB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updScoColorR",      HTTP_POST, []( AsyncWebServerRequest *request ) { scoColorRR      = request->arg( "r" ).toInt(); pref.putInt( "scoColorRR", scoColorRR ); scoColorRG = request->arg( "g" ).toInt(); pref.putInt( "scoColorRG", scoColorRG ); scoColorRB = request->arg( "b" ).toInt(); pref.putInt( "scoColorRB", scoColorRB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE countdown
  //  --------------------------------------------------------------------------
  server.on( "/getCouColorH",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorHR, couColorHG, couColorHB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorM",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorMR, couColorMG, couColorMB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorS",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorSR, couColorSG, couColorSB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/getCouColorA",         []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", couColorAR, couColorAG, couColorAB ); request->send( 200, "text/plain", tempcolor ); } );

  server.on( "/updCouColorH",         HTTP_POST, []( AsyncWebServerRequest *request ) { couColorHR = request->arg( "r" ).toInt(); pref.putInt( "couColorHR", couColorHR ); couColorHG = request->arg( "g" ).toInt(); pref.putInt( "couColorHG", couColorHG ); couColorHB = request->arg( "b" ).toInt(); pref.putInt( "couColorHB", couColorHB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updCouColorM",         HTTP_POST, []( AsyncWebServerRequest *request ) { couColorMR = request->arg( "r" ).toInt(); pref.putInt( "couColorMR", couColorMR ); couColorMG = request->arg( "g" ).toInt(); pref.putInt( "couColorMG", couColorMG ); couColorMB = request->arg( "b" ).toInt(); pref.putInt( "couColorMB", couColorMB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updCouColorS",         HTTP_POST, []( AsyncWebServerRequest *request ) { couColorSR = request->arg( "r" ).toInt(); pref.putInt( "couColorSR", couColorSR ); couColorSG = request->arg( "g" ).toInt(); pref.putInt( "couColorSG", couColorSG ); couColorSB = request->arg( "b" ).toInt(); pref.putInt( "couColorSB", couColorSB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/updCouColorA",         HTTP_POST, []( AsyncWebServerRequest *request ) { couColorAR = request->arg( "r" ).toInt(); pref.putInt( "couColorAR", couColorAR ); couColorAG = request->arg( "g" ).toInt(); pref.putInt( "couColorAG", couColorAG ); couColorAB = request->arg( "b" ).toInt(); pref.putInt( "couColorAB", couColorAB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  MODE scrolling
  //  --------------------------------------------------------------------------
  server.on( "/getScrTime",           []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( scrTime ) ); } );
  server.on( "/updScrTime",           HTTP_POST, []( AsyncWebServerRequest *request ) { scrTime = request->arg( "scrTime" ).toInt(); pref.putInt( "scrTime", scrTime ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  REST API connect
  //  --------------------------------------------------------------------------
  server.on( "/getKNXAddress",        []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxAddress ) ); } );
  server.on( "/updKNXAddress",        HTTP_POST, []( AsyncWebServerRequest *request ) { knxAddress      = request->arg( "knxAddress" );           pref.putString( "knxAddr", knxAddress );          request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getKNXPort",           []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxPort ) ); } );
  server.on( "/updKNXPort",           HTTP_POST, []( AsyncWebServerRequest *request ) { knxPort       = request->arg( "knxPort" ).toInt();      pref.putInt( "knxPort", knxPort );                request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getKNXBrightPath",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightPath ) ); } );
  server.on( "/updKNXBrightPath",     HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightPath = request->arg( "knxBrightPath" );        pref.putString( "knxBrightPath", knxBrightPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getKNXBrightMin",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightMin ) ); } );
  server.on( "/updKNXBrightMin",      HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightMin  = request->arg( "knxBrightMin" ).toInt(); pref.putInt( "knxBrightMin", knxBrightMin );      request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getKNXBrightMax",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxBrightMax ) ); } );
  server.on( "/updKNXBrightMax",      HTTP_POST, []( AsyncWebServerRequest *request ) { knxBrightMax  = request->arg( "knxBrightMax" ).toInt(); pref.putInt( "knxBrightMax", knxBrightMax );      request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getKNXTempPath",       []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxTempPath ) ); } );
  server.on( "/updKNXTempPath",       HTTP_POST, []( AsyncWebServerRequest *request ) { knxTempPath   = request->arg( "knxTempPath" );          pref.putString( "knxTempPath", knxTempPath );     request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXtemperature(); } );
  server.on( "/getKNXHumidPath",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxHumidPath ) ); } );
  server.on( "/updKNXHumidPath",      HTTP_POST, []( AsyncWebServerRequest *request ) { knxHumidPath  = request->arg( "knxHumidPath" );         pref.putString( "knxHumPath", knxHumidPath );     request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXhumidity(); } );

  //  Edgelights
  //  --------------------------------------------------------------------------
  server.on( "/getEdgeBrightIn",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightIn ) ); } );
  server.on( "/updEdgeBrightIn",      HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightIn = request->arg( "edgeBrightIn" ).toInt(); pref.putInt( "edgBrightIn", edgeBrightIn ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getEdgeBrightMin",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightMin ) ); } );
  server.on( "/updEdgeBrightMin",     HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightMin = request->arg( "edgeBrightMin" ).toInt(); pref.putInt( "edgBrightMin", edgeBrightMin ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getEdgeBrightMax",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightMax ) ); } );
  server.on( "/updEdgeBrightMax",     HTTP_POST, []( AsyncWebServerRequest *request ) { edgeBrightMax = request->arg( "edgeBrightMax" ).toInt(); pref.putInt( "edgBrightMax", edgeBrightMax ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getEdgeBrightOut",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeBrightOut ) ); } );
  server.on( "/getEdgeFrequency",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( edgeFrequency ) ); } );
  server.on( "/updEdgeFrequency",     HTTP_POST, []( AsyncWebServerRequest *request ) { edgeFrequency = request->arg( "edgeFrequency" ).toInt(); pref.putInt( "edgeFrequency", edgeFrequency ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );

  //  Downlights
  //  --------------------------------------------------------------------------
  server.on( "/getDownSwitch",        []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downSwitch ) ); } );
  server.on( "/updDownSwitch",        HTTP_POST, []( AsyncWebServerRequest *request ) { downSwitch = request->arg( "downSwitch" ).toInt(); pref.putInt( "downSwitch", downSwitch ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownBrightIn",      []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightIn ) ); } );
  server.on( "/updDownBrightIn",      HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightIn = request->arg( "downBrightIn" ).toInt(); pref.putInt( "downBrightIn", downBrightIn ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getDownBrightMin",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightMin ) ); } );
  server.on( "/updDownBrightMin",     HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightMin = request->arg( "downBrightMin" ).toInt(); pref.putInt( "downBrightMin", downBrightMin ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getDownBrightMax",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightMax ) );  } );
  server.on( "/updDownBrightMax",     HTTP_POST, []( AsyncWebServerRequest *request ) { downBrightMax = request->arg( "downBrightMax" ).toInt(); pref.putInt( "downBrightMax", downBrightMax ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); getKNXbrightness(); } );
  server.on( "/getDownBrightOut",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downBrightOut ) ); } );
  server.on( "/getDownFrequency",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downFrequency ) ); } );
  server.on( "/updDownFrequency",     HTTP_POST, []( AsyncWebServerRequest *request ) { downFrequency = request->arg( "downFrequency" ).toInt(); pref.putInt( "downFrequency", downFrequency ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColorMode",     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( downColorMode ) ); } );
  server.on( "/updDownColorMode",     HTTP_POST, []( AsyncWebServerRequest *request ) { downColorMode = request->arg( "downColorMode" ).toInt(); pref.putInt( "downColorMode", downColorMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor00",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR00, downColorG00, downColorB00 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor00",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR00 = request->arg( "r" ).toInt(); pref.putInt( "downColorR00", downColorR00 ); downColorG00 = request->arg( "g" ).toInt(); pref.putInt( "downColorG00", downColorG00 ); downColorB00 = request->arg( "b" ).toInt(); pref.putInt( "downColorB00", downColorB00 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor01",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR01, downColorG01, downColorB01 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor01",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR01 = request->arg( "r" ).toInt(); pref.putInt( "downColorR01", downColorR01 ); downColorG01 = request->arg( "g" ).toInt(); pref.putInt( "downColorG01", downColorG01 ); downColorB01 = request->arg( "b" ).toInt(); pref.putInt( "downColorB01", downColorB01 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor02",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR02, downColorG02, downColorB02 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor02",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR02 = request->arg( "r" ).toInt(); pref.putInt( "downColorR02", downColorR02 ); downColorG02 = request->arg( "g" ).toInt(); pref.putInt( "downColorG02", downColorG02 ); downColorB02 = request->arg( "b" ).toInt(); pref.putInt( "downColorB02", downColorB02 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor03",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR03, downColorG03, downColorB03 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor03",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR03 = request->arg( "r" ).toInt(); pref.putInt( "downColorR03", downColorR03 ); downColorG03 = request->arg( "g" ).toInt(); pref.putInt( "downColorG03", downColorG03 ); downColorB03 = request->arg( "b" ).toInt(); pref.putInt( "downColorB03", downColorB03 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor04",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR04, downColorG04, downColorB04 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor04",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR04 = request->arg( "r" ).toInt(); pref.putInt( "downColorR04", downColorR04 ); downColorG04 = request->arg( "g" ).toInt(); pref.putInt( "downColorG04", downColorG04 ); downColorB04 = request->arg( "b" ).toInt(); pref.putInt( "downColorB04", downColorB04 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor05",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR05, downColorG05, downColorB05 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor05",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR05 = request->arg( "r" ).toInt(); pref.putInt( "downColorR05", downColorR05 ); downColorG05 = request->arg( "g" ).toInt(); pref.putInt( "downColorG05", downColorG05 ); downColorB05 = request->arg( "b" ).toInt(); pref.putInt( "downColorB05", downColorB05 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor06",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR06, downColorG06, downColorB06 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor06",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR06 = request->arg( "r" ).toInt(); pref.putInt( "downColorR06", downColorR06 ); downColorG06 = request->arg( "g" ).toInt(); pref.putInt( "downColorG06", downColorG06 ); downColorB06 = request->arg( "b" ).toInt(); pref.putInt( "downColorB06", downColorB06 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor07",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR07, downColorG07, downColorB07 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor07",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR07 = request->arg( "r" ).toInt(); pref.putInt( "downColorR07", downColorR07 ); downColorG07 = request->arg( "g" ).toInt(); pref.putInt( "downColorG07", downColorG07 ); downColorB07 = request->arg( "b" ).toInt(); pref.putInt( "downColorB07", downColorB07 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor08",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR08, downColorG08, downColorB08 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor08",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR08 = request->arg( "r" ).toInt(); pref.putInt( "downColorR08", downColorR08 ); downColorG08 = request->arg( "g" ).toInt(); pref.putInt( "downColorG08", downColorG08 ); downColorB08 = request->arg( "b" ).toInt(); pref.putInt( "downColorB08", downColorB08 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor09",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR09, downColorG09, downColorB09 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor09",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR09 = request->arg( "r" ).toInt(); pref.putInt( "downColorR09", downColorR09 ); downColorG09 = request->arg( "g" ).toInt(); pref.putInt( "downColorG09", downColorG09 ); downColorB09 = request->arg( "b" ).toInt(); pref.putInt( "downColorB09", downColorB09 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor10",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR10, downColorG10, downColorB10 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor10",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR10 = request->arg( "r" ).toInt(); pref.putInt( "downColorR10", downColorR10 ); downColorG10 = request->arg( "g" ).toInt(); pref.putInt( "downColorG10", downColorG10 ); downColorB10 = request->arg( "b" ).toInt(); pref.putInt( "downColorB10", downColorB10 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor11",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR11, downColorG11, downColorB11 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor11",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR11 = request->arg( "r" ).toInt(); pref.putInt( "downColorR11", downColorR11 ); downColorG11 = request->arg( "g" ).toInt(); pref.putInt( "downColorG11", downColorG11 ); downColorB11 = request->arg( "b" ).toInt(); pref.putInt( "downColorB11", downColorB11 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor12",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR12, downColorG12, downColorB12 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor12",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR12 = request->arg( "r" ).toInt(); pref.putInt( "downColorR12", downColorR12 ); downColorG12 = request->arg( "g" ).toInt(); pref.putInt( "downColorG12", downColorG12 ); downColorB12 = request->arg( "b" ).toInt(); pref.putInt( "downColorB12", downColorB12 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor13",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR13, downColorG13, downColorB13 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor13",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR13 = request->arg( "r" ).toInt(); pref.putInt( "downColorR13", downColorR13 ); downColorG13 = request->arg( "g" ).toInt(); pref.putInt( "downColorG13", downColorG13 ); downColorB13 = request->arg( "b" ).toInt(); pref.putInt( "downColorB13", downColorB13 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDownColor14",       []( AsyncWebServerRequest *request ) { char tempcolor[ 8 ]; sprintf( tempcolor, "#%02X%02X%02X", downColorR14, downColorG14, downColorB14 ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDownColor14",       HTTP_POST, []( AsyncWebServerRequest *request ) { downColorR14 = request->arg( "r" ).toInt(); pref.putInt( "downColorR14", downColorR14 ); downColorG14 = request->arg( "g" ).toInt(); pref.putInt( "downColorG14", downColorG14 ); downColorB14 = request->arg( "b" ).toInt(); pref.putInt( "downColorB14", downColorB14 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  // clang-format on
}

/**
 * @brief SETUP - webserver
 * @details initialize the webserver and ota
 * @since 1.0.0
 */
void initSERVER() {
  Serial.println( "- Webserver" );
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
  Serial.println( "- LED" );

  // edge
  FastLED.addLeds<NEOPIXEL, EDGE_PIN>( arrayEdge, EDGE_COUNT );
  FastLED[ 0 ].showLeds( edgeBrightOut );
  // down
  FastLED.addLeds<NEOPIXEL, DOWN_PIN>( arrayDown, DOWN_COUNT );
  FastLED[ 1 ].showLeds( downBrightOut );
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
 * @details fetch brightness value from the home automation system
 * @since 1.2.0
 */
void getKNXbrightness() {
  if ( WiFi.status() == WL_CONNECTED ) { // if wifi connected
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxBrightPath != "" ) ) {
      knxClient.get( knxBrightPath );                   // call knx for value
      knxBrightIn   = knxClient.responseBody().toInt(); // ...and write answer
      // edge
      edgeBrightOut = map( knxBrightIn, knxBrightMin, knxBrightMax, edgeBrightMin, edgeBrightMax );
      pref.putInt( "edgBrightOut", edgeBrightOut );
      // downlight
      downBrightOut = map( knxBrightIn, knxBrightMin, knxBrightMax, downBrightMin, downBrightMax );
      pref.putInt( "dowBrightOut", downBrightOut );

      Serial.println( "- knxBrightness" );
      Serial.printf( "  knxIn:      %i\n", knxBrightIn );
      Serial.printf( "  edgeOut:    %i\n", edgeBrightOut );
      Serial.printf( "  downOut:    %i\n", downBrightOut );
    }
  } else { // if wifi not connected
    // initWIFI(); // reconnect wifi first
  }
}

/**
 * @brief LOOP - knx temperature
 * @details fetch temperature value from the home automation system
 * @since 1.2.0
 */
void getKNXtemperature() {
  if ( WiFi.status() == WL_CONNECTED ) {
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxTempPath != "" ) ) {
      knxClient.get( knxTempPath );                        // call knx for value
      knxTempIn = ( knxClient.responseBody().toDouble() ); // ...and write answer
      Serial.println( "- knxTemperature" );
      Serial.printf( "  knxValue:   %i\n", knxTempIn );
    }
  } else { // if wifi not connected
    // initWIFI(); // reconnect wifi first
  }
}

/**
 * @brief LOOP - knx humidity
 * @details fetch humidity value from the home automation system
 * @since 1.2.0
 */
void getKNXhumidity() {
  if ( WiFi.status() == WL_CONNECTED ) {
    if ( ( knxAddress != "" ) && ( knxPort != 0 ) && ( knxHumidPath != "" ) ) {
      knxClient.get( knxHumidPath );                                    // call knx for value
      knxHumidIn = int( ( knxClient.responseBody().toDouble() ) * 10 ); // ...and write answer
      Serial.println( "- knxHumidity" );
      Serial.printf( "  knxValue:   %d\n", knxHumidIn );
    }
  } else {
    // initWIFI();
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
  switch ( d ) {
    case 0: // digit 0
      segA = segB = segC = segE = segF = segG = c;
      break;
    case 1: // digit 1
      segA = segE = c;
      break;
    case 2: // digit 2
      segA = segB = segD = segF = segG = c;
      break;
    case 3: // digit 3
      segA = segB = segD = segE = segF = c;
      break;
    case 4: // digit 4
      segA = segC = segD = segE = c;
      break;
    case 5: // digit 5
      segB = segC = segD = segE = segF = c;
      break;
    case 6: // digit 6
      segB = segC = segD = segE = segF = segG = c;
      break;
    case 7: // digit 7
      segA = segB = segE = c;
      break;
    case 8: // digit 8
      segA = segB = segC = segD = segE = segF = segG = c;
      break;
    case 9: // digit 9
      segA = segB = segC = segD = segE = segF = c;
      break;
    case 10: // sign -
      segD = c;
      break;
    case 11: // sign °
      segA = segB = segC = segD = c;
      break;
    case 12: // letter d
      segA = segD = segE = segF = segG = c;
      break;
    case 13: // letter E
      segB = segC = segD = segF = segG = c;
      break;
    case 14: // letter H
      segA = segC = segD = segE = segG = c;
      break;
    case 15: // letter n
      segD = segE = segG = c;
      break;
    default:
      break;
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
  if ( downSwitch == 0 ) {
    fill_solid( arrayDown, DOWN_COUNT, CRGB::Black );
  }
  // downlights - on
  if ( downSwitch == 1 ) {
    // 1 defined color
    if ( downColorMode == 0 ) {
      for ( int d = 0; d < DOWN_COUNT; d++ ) {
        arrayDown[ d ].setRGB( downColorR00, downColorG00, downColorB00 );
      }
    }
    // 14 defined colors
    if ( downColorMode == 1 ) {
      arrayDown[ 0 ].setRGB( downColorR01, downColorG01, downColorB01 );
      arrayDown[ 1 ].setRGB( downColorR02, downColorG02, downColorB02 );
      arrayDown[ 2 ].setRGB( downColorR03, downColorG03, downColorB03 );
      arrayDown[ 3 ].setRGB( downColorR04, downColorG04, downColorB04 );
      arrayDown[ 4 ].setRGB( downColorR05, downColorG05, downColorB05 );
      arrayDown[ 5 ].setRGB( downColorR06, downColorG06, downColorB06 );
      arrayDown[ 6 ].setRGB( downColorR07, downColorG07, downColorB07 );
      arrayDown[ 7 ].setRGB( downColorR08, downColorG08, downColorB08 );
      arrayDown[ 8 ].setRGB( downColorR09, downColorG09, downColorB09 );
      arrayDown[ 9 ].setRGB( downColorR10, downColorG10, downColorB10 );
      arrayDown[ 10 ].setRGB( downColorR11, downColorG11, downColorB11 );
      arrayDown[ 11 ].setRGB( downColorR12, downColorG12, downColorB12 );
      arrayDown[ 12 ].setRGB( downColorR13, downColorG13, downColorB13 );
      arrayDown[ 13 ].setRGB( downColorR14, downColorG14, downColorB14 );
    }
    // 1 random color
    if ( downColorMode == 2 &&
         ( ( downFrequency == 0 ) || ( downFrequency == 1 && downMinPassed ) ||
           ( downFrequency == 2 && downHourPassed ) || ( downFrequency == 3 && downDayPassed ) ) ) {
      CHSV dowColorRandom = CHSV( random8(), 255, 255 );
      for ( int d = 0; d < DOWN_COUNT; d++ ) {
        arrayDown[ d ] = dowColorRandom;
      }
    }
    // 14 random colors
    if ( downColorMode == 3 &&
         ( ( downFrequency == 0 ) || ( downFrequency == 1 && downMinPassed ) ||
           ( downFrequency == 2 && downHourPassed ) || ( downFrequency == 3 && downDayPassed ) ) ) {
      for ( int d = 0; d < DOWN_COUNT; d++ ) {
        arrayDown[ d ] = CHSV( random8(), 255, 255 );
      }
    }
  }
  // downlights - show
  if ( downBrightIn == 0 ) {
    FastLED[ 1 ].showLeds( downBrightOut );
  } else {
    FastLED[ 1 ].showLeds( downBrightIn );
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
      if ( WiFi.status() != WL_CONNECTED ) { // NO wifi
        // initWIFI();                          // -> reconnect
      }
    }

    if ( abs( currTmin - prevTimeMinCore0 ) >= 1 ) { // every minute
      prevTimeMinCore0 = currTmin;                   // set reference
      edgeMinPassed    = true;                       // set flag
      downMinPassed    = true;                       // set flag
    }

    if ( ( m2 == 0 || m2 == 5 ) && ( currTsec == 0 ) ) { // every 5 minutes
      getKNXbrightness();                                // brightness
      getKNXtemperature();                               // temperature
      getKNXhumidity();                                  // humidity
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
