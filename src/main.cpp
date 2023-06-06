//  ----------------------------------------------------------------------------------------------------
//  S H E L F   E D G E   C L O C K
//
//  Author:   CaptSnus
//  Date:     May 16, 2023
//  License:  tbd
//  ----------------------------------------------------------------------------------------------------
//
//  TABLE OF CONTENTS
//  - libraries, header files and board definitions
//  - pin assignments & global variables
//  - functions
//  - setup()
//  - loop()



//  ----------------------------------------------------------------------------------------------------
//  LIBRARIES, HEADER FILES & BOARD DEFINITIONS
//  ----------------------------------------------------------------------------------------------------

#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <ArduinoHttpClient.h>
#include <AsyncElegantOTA.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <NonBlockingRtttl.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <time.h>
#include <WiFi.h>



//  ----------------------------------------------------------------------------------------------------
//  PIN ASSIGNMENTS & GLOBAL VARIABLES
//  ----------------------------------------------------------------------------------------------------

//  general
char                version[]           = "1.1.1";                        // software version
int                 displayMode         = 0;                              // 0 = clock, 1 = date, 2 = temperature, 3 = humidity, 4 = scoreboard, 5 = countdown
TaskHandle_t        taskCore0;                                            // task executed in the taskCore0code() function
Preferences         pref;                                                 // shortcut for Preferences

//  time
//  |- external
struct tm           ntpTime;                                              // structure to save time from ntp server
time_t              now;                                                  // this is the epoch
//  |- internal
uint32_t            currMillisCore0     = millis();                       // current time in core 0
uint32_t            currMillisCore1     = millis();                       // current time in core 1
uint32_t            prevTimeSecCore0    = 0;                              // reference for previous time in core 0 (1 second)
uint32_t            prevTimeSecCore1    = 0;                              // reference for previous time in core 1 (1 second)
int                 prevTimeMinCore0    = 0;                              // reference for previous time in core 0 (1 minute)
int                 prevTimeHourCore0   = 0;                              // reference for previous time in core 0 (1 hour)
int                 prevTimeDayCore0    = 0;                              // reference for previous time in core 0 (1 day)
bool                clockMinPassed      = true;                           // flag for the color change of the clock face (1 minute)
bool                clockHourPassed     = true;                           // flag for the color change of the clock face (1 hour)
bool                clockDayPassed      = true;                           // flag for the color change of the clock face (1 day)
bool                downMinPassed       = true;                           // flag for the color change of the downlights (1 minute)
bool                downHourPassed      = true;                           // flag for the color change of the downlights (1 hour)
bool                downDayPassed       = true;                           // flag for the color change of the downlights (1 day)

//  wiFi
String              apSSID              = "Shelf Edge Clock";             // access point - ssid
String              apPass              = "shelfEdgeClock";               // access point - password
bool                apHide              = false;                          // access point - hidden: false = no, true = yes
IPAddress           apLocalIP;                                            // access point - ip address
int                 apIP1, apIP2, apIP3, apIP4;                           // access point - ip address in blocks
IPAddress           apGateway;                                            // access point - gateway address
int                 apGW1, apGW2, apGW3, apGW4;                           // access point - gateway address in blocks
IPAddress           apSubnet;                                             // access point - subnet mask
int                 apSN1, apSN2, apSN3, apSN4;                           // access point - subnet mask in blocks

bool                wifiConfig          = false;                          // is the wifi client configured?
String              wifiSSID            = "ENTER_SSID";                   // wifi client - ssid
String              wifiPass            = "ENTER_PASSWORD";               // wifi client - password
bool                wifiDHCP            = false;                          // wifi client - dhcp: false = DHCP, true = manual
IPAddress           wifiLocalIP;                                          // wifi client - ip address
int                 wifiIP1, wifiIP2, wifiIP3, wifiIP4;                   // wifi client - ip address in blocks
IPAddress           wifiGateway;                                          // wifi client - gateway address
int                 wifiGW1, wifiGW2, wifiGW3, wifiGW4;                   // wifi client - gateway address in blocks
IPAddress           wifiSubnet;                                           // wifi client - subnet mask
int                 wifiSN1, wifiSN2, wifiSN3, wifiSN4;                   // wifi client - subnet mask in blocks
WiFiClient          wifi;                                                 // shortcut for WifiClient
String              host                = "ShelfEdgeClock";               // define the hostname (mDNS)

//  server
AsyncWebServer      server(80);                                           // shortcut for AsyncWebServer and binding to port 80

//  led
#define             CLOCK_PIN           36
#define             CLOCK_COUNT         252
#define             DOWNLIGHT_PIN       35
#define             DOWNLIGHT_COUNT     14
Adafruit_NeoPixel   ledClock( CLOCK_COUNT, CLOCK_PIN, NEO_GRB + NEO_KHZ800 );
Adafruit_NeoPixel   ledDownlight( DOWNLIGHT_COUNT, DOWNLIGHT_PIN, NEO_GRB + NEO_KHZ800 );

//  content
//  |- global
int                 gloBrightness       = 0;                              // setting of the brightness -> 0 = automatic, > 0 = manual
int                 gloColorFreq        = 0;                              // 0 = every second, 1 = every minute, 2 = every hour, 3 = every day
int                 dowUsage            = 0;                              // 0 = downlights off, 1 = downlights on
int                 dowBrightness       = 0;                              // setting of the brightness -> 0 = automatic, > 0 = manual
int                 dowColorFreq        = 0;                              // 0 = every second, 1 = every minute, 2 = every hour, 3 = every day
int                 briClock, briDown;
int                 briClockMin, briClockMax;
int                 briDownMin, briDownMax;
//  |- knx
String              knxAddr             = "";                             // ip address of your knx server
int                 knxPort             = 8080;                           // port number of your knx server
String              briKNXPath          = "";                             // path to the brightness item in the home automation system
int                 briKNXmin, briKNXmax;                                 // minimum and maximum values provided by the home automation
String              temKNXPath          = "";                             // path to the temperature item in the home automation system
String              humKNXPath          = "";                             // path to the humidity item in the home automation system
HttpClient          client              = HttpClient( wifi, knxAddr.c_str(), knxPort );
//  |- downlights
int                 dowColorSet         = 0;                              // 0 = 1 defined color, 1 = 14 defined colors , 2 = 1 random color, 3 = 14 random colors
uint32_t            dowColor;
int                 dowColorR, dowColorG, dowColorB;
uint32_t            dowColor0;
int                 dowColor0R, dowColor0G, dowColor0B;
uint32_t            dowColor1;
int                 dowColor1R, dowColor1G, dowColor1B;
uint32_t            dowColor2;
int                 dowColor2R, dowColor2G, dowColor2B;
uint32_t            dowColor3;
int                 dowColor3R, dowColor3G, dowColor3B;
uint32_t            dowColor4;
int                 dowColor4R, dowColor4G, dowColor4B;
uint32_t            dowColor5;
int                 dowColor5R, dowColor5G, dowColor5B;
uint32_t            dowColor6;
int                 dowColor6R, dowColor6G, dowColor6B;
uint32_t            dowColor7;
int                 dowColor7R, dowColor7G, dowColor7B;
uint32_t            dowColor8;
int                 dowColor8R, dowColor8G, dowColor8B;
uint32_t            dowColor9;
int                 dowColor9R, dowColor9G, dowColor9B;
uint32_t            dowColor10;
int                 dowColor10R, dowColor10G, dowColor10B;
uint32_t            dowColor11;
int                 dowColor11R, dowColor11G, dowColor11B;
uint32_t            dowColor12;
int                 dowColor12R, dowColor12G, dowColor12B;
uint32_t            dowColor13;
int                 dowColor13R, dowColor13G, dowColor13B;
//  |- mode clock
String              clkAddr             = "ch.pool.ntp.org";              // pool.ntp.org, europe.pool.ntp.org
String              clkZone             = "CET-1CEST,M3.5.0,M10.5.0/3";   // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
int                 clkFormat           = 1;                              // 0 = 12 hours, 1 = 24 hours
int                 clkColorSet         = 0;                              // 0 = two defined colors, 1 = two random colors
uint32_t            clkHColor;
int                 clkHColorR, clkHColorG, clkHColorB;
uint32_t            clkMColor;
int                 clkMColorR, clkMColorG, clkMColorB;
//  |- mode date
int                 datFormat           = 1;                              // 0 = MMdd, 1 = ddMM
int                 datColorSet         = 0;                              // 0 = two defined colors, 1 = two random colors
uint32_t            datDColor;
int                 datDColorR, datDColorG, datDColorB;
uint32_t            datMColor;
int                 datMColorR, datMColorG, datMColorB;
//  |- mode temperature
int                 temSymbol           = 1;                              // 0 = Fahrenheit, 1 = Celsius
int                 temColorSet         = 0;                              // 0 = three defined colors, 1 = three random colors
uint32_t            temAColor;
int                 temAColorR, temAColorG, temAColorB;
uint32_t            temBColor;
int                 temBColorR, temBColorG, temBColorB;
uint32_t            temCColor;
int                 temCColorR, temCColorG, temCColorB;
int                 temKNXValue         = 0;                              // temperature value from knx
//  |- mode humidity
int                 humColorSet         = 0;                              // 0 = three defined colors, 1 = three random colors
uint32_t            humAColor;
int                 humAColorR, humAColorG, humAColorB;
uint32_t            humBColor;
int                 humBColorR, humBColorG, humBColorB;
uint32_t            humCColor;
int                 humCColorR, humCColorG, humCColorB;
int                 humKNXValue         = 0;                              // humidity value from knx
//  |- mode scoreboard
uint32_t            scoHColor;
int                 scoHColorR, scoHColorG, scoHColorB;
int                 scoHValue           = 0;                              // score home team (left)
uint32_t            scoAColor;
int                 scoAColorR, scoAColorG, scoAColorB;
int                 scoAValue           = 0;                              // score away team (right)
//  |- mode countdown
uint32_t            couMillis           = 0;
uint32_t            couMillisEnd        = 0;
uint32_t            couHColor;
int                 couHColorR, couHColorG, couHColorB;
uint32_t            couMColor;
int                 couMColorR, couMColorG, couMColorB;
uint32_t            couSColor;
int                 couSColorR, couSColorG, couSColorB;
uint32_t            couAColor;
int                 couAColorR, couAColorG, couAColorB;
//  |- mode scroll
uint32_t            scrMillis           = 0;
uint32_t            scrMillisPrev       = 0;
int                 scrTime;
int                 scrDisplayMode      = 0;



//  ----------------------------------------------------------------------------------------------------
//  FUNCTIONS
//  ----------------------------------------------------------------------------------------------------

//  init message
void initMessage() {
  Serial.println( "====================================================================================================" );
  Serial.println( "Shelf Edge Clock for ESP32" );
  Serial.println( "Author         CaptSnus" );
  Serial.println( "Licence        tbd" );
  Serial.println( "Info           https://github.com/CaptSnus/ShelfEdgeClock" );
  Serial.printf ( "Version        %s\n", version );
  Serial.println( "====================================================================================================" );
}

//  flash
void initFlash() {
  Serial.println( "\nPreferences" );
  Serial.println( "--------------------------------------------------" );
  Serial.println( "[*] Loading values from flash ..." );
  pref.begin( "shelfclock", false );                                      // create array for the values
  //  wifi
  apSSID            = pref.getString( "apSSID", apSSID );
  apPass            = pref.getString( "apPass", apPass );
  apHide            = pref.getInt( "apHide", apHide );
  apIP1             = pref.getInt( "apIP1", 192 );
  apIP2             = pref.getInt( "apIP2", 168 );
  apIP3             = pref.getInt( "apIP3", 100 );
  apIP4             = pref.getInt( "apIP4", 1 );
  apGW1             = pref.getInt( "apGW1", 192 );
  apGW2             = pref.getInt( "apGW2", 168 );
  apGW3             = pref.getInt( "apGW3", 100 );
  apGW4             = pref.getInt( "apGW4", 1 );
  apSN1             = pref.getInt( "apSN1", 255 );
  apSN2             = pref.getInt( "apSN2", 255 );
  apSN3             = pref.getInt( "apSN3", 255 );
  apSN4             = pref.getInt( "apSN4", 0 );
  wifiConfig        = pref.getInt( "wifiConfig", wifiConfig );
  wifiSSID          = pref.getString( "wifiSSID", wifiSSID );
  wifiPass          = pref.getString( "wifiPass", wifiPass );
  wifiDHCP          = pref.getInt( "wifiDHCP", wifiDHCP );
  wifiIP1           = pref.getInt( "wifiIP1" );
  wifiIP2           = pref.getInt( "wifiIP2" );
  wifiIP3           = pref.getInt( "wifiIP3" );
  wifiIP4           = pref.getInt( "wifiIP4" );
  wifiGW1           = pref.getInt( "wifiGW1" );
  wifiGW2           = pref.getInt( "wifiGW2" );
  wifiGW3           = pref.getInt( "wifiGW3" );
  wifiGW4           = pref.getInt( "wifiGW4" );
  wifiSN1           = pref.getInt( "wifiSN1" );
  wifiSN2           = pref.getInt( "wifiSN2" );
  wifiSN3           = pref.getInt( "wifiSN3" );
  wifiSN4           = pref.getInt( "wifiSN4" );
  host              = pref.getString ( "host", host );
  //  content
  //  |- global settings
  gloBrightness     = pref.getInt( "gloBrightness", gloBrightness );
  briClockMin       = pref.getInt( "briClockMin", 2 );
  briClockMax       = pref.getInt( "briClockMax", 125 );
  gloColorFreq      = pref.getInt( "gloColorFreq", gloColorFreq );
  dowUsage          = pref.getInt( "dowUsage", dowUsage );
  dowBrightness     = pref.getInt( "dowBrightness", dowBrightness );
  briDownMin        = pref.getInt( "briDownMin ", 10 );
  briDownMax        = pref.getInt( "briDownMax", 250 );
  dowColorFreq      = pref.getInt( "dowColorFreq", dowColorFreq );
  //  |- knx connection
  knxAddr           = pref.getString( "knxAddr" );
  knxPort           = pref.getInt( "knxPort" );
  briKNXPath        = pref.getString( "briKNXPath" );
  briKNXmin         = pref.getInt( "briKNXmin", 0 );
  briKNXmax         = pref.getInt( "briKNXmax", 150000 );
  temKNXPath        = pref.getString( "temKNXPath" );
  humKNXPath        = pref.getString( "humKNXPath" );
  //  |- downlights
  dowColorSet       = pref.getInt( "dowColorSet", 0 );
  dowColorR         = pref.getInt( "dowColorR", 255 );
  dowColorG         = pref.getInt( "dowColorG", 255 );
  dowColorB         = pref.getInt( "dowColorB", 255 );
  dowColor0R        = pref.getInt( "dowColor0R", 255 );
  dowColor0G        = pref.getInt( "dowColor0G", 255 );
  dowColor0B        = pref.getInt( "dowColor0B", 255 );
  dowColor1R        = pref.getInt( "dowColor1R", 255 );
  dowColor1G        = pref.getInt( "dowColor1G", 255 );
  dowColor1B        = pref.getInt( "dowColor1B", 255 );
  dowColor2R        = pref.getInt( "dowColor2R", 255 );
  dowColor2G        = pref.getInt( "dowColor2G", 255 );
  dowColor2B        = pref.getInt( "dowColor2B", 255 );
  dowColor3R        = pref.getInt( "dowColor3R", 255 );
  dowColor3G        = pref.getInt( "dowColor3G", 255 );
  dowColor3B        = pref.getInt( "dowColor3B", 255 );
  dowColor4R        = pref.getInt( "dowColor4R", 255 );
  dowColor4G        = pref.getInt( "dowColor4G", 255 );
  dowColor4B        = pref.getInt( "dowColor4B", 255 );
  dowColor5R        = pref.getInt( "dowColor5R", 255 );
  dowColor5G        = pref.getInt( "dowColor5G", 255 );
  dowColor5B        = pref.getInt( "dowColor5B", 255 );
  dowColor6R        = pref.getInt( "dowColor6R", 255 );
  dowColor6G        = pref.getInt( "dowColor6G", 255 );
  dowColor6B        = pref.getInt( "dowColor6B", 255 );
  dowColor7R        = pref.getInt( "dowColor7R", 255 );
  dowColor7G        = pref.getInt( "dowColor7G", 255 );
  dowColor7B        = pref.getInt( "dowColor7B", 255 );
  dowColor8R        = pref.getInt( "dowColor8R", 255 );
  dowColor8G        = pref.getInt( "dowColor8G", 255 );
  dowColor8B        = pref.getInt( "dowColor8B", 255 );
  dowColor9R        = pref.getInt( "dowColor9R", 255 );
  dowColor9G        = pref.getInt( "dowColor9G", 255 );
  dowColor9B        = pref.getInt( "dowColor9B", 255 );
  dowColor10R       = pref.getInt( "dowColor10R", 255 );
  dowColor10G       = pref.getInt( "dowColor10G", 255 );
  dowColor10B       = pref.getInt( "dowColor10B", 255 );
  dowColor11R       = pref.getInt( "dowColor11R", 255 );
  dowColor11G       = pref.getInt( "dowColor11G", 255 );
  dowColor11B       = pref.getInt( "dowColor11B", 255 );
  dowColor12R       = pref.getInt( "dowColor12R", 255 );
  dowColor12G       = pref.getInt( "dowColor12G", 255 );
  dowColor12B       = pref.getInt( "dowColor12B", 255 );
  dowColor13R       = pref.getInt( "dowColor13R", 255 );
  dowColor13G       = pref.getInt( "dowColor13G", 255 );
  dowColor13B       = pref.getInt( "dowColor13B", 255 );
  //  |- mode clock
  clkAddr           = pref.getString( "clkAddr", clkAddr );
  clkZone           = pref.getString( "clkZone", clkZone );
  clkFormat         = pref.getInt( "clkFormat", 1 );
  clkColorSet       = pref.getInt( "clkColorSet", 0 );
  clkHColorR        = pref.getInt( "clkHColorR", 85 );
  clkHColorG        = pref.getInt( "clkHColorG", 26 );
  clkHColorB        = pref.getInt( "clkHColorB", 139 );
  clkMColorR        = pref.getInt( "clkMColorR", 255 );
  clkMColorG        = pref.getInt( "clkMColorG", 0 );
  clkMColorB        = pref.getInt( "clkMColorB" , 132);
  //  |- mode date
  datFormat         = pref.getInt( "datFormat", 1 );
  datColorSet       = pref.getInt( "datColorSet", 0 );
  datDColorR        = pref.getInt( "datDColorR", 255 );
  datDColorG        = pref.getInt( "datDColorG", 215 );
  datDColorB        = pref.getInt( "datDColorB", 0 );
  datMColorR        = pref.getInt( "datMColorR", 255 );
  datMColorG        = pref.getInt( "datMColorG", 69 );
  datMColorB        = pref.getInt( "datMColorB", 0 );
  //  |- mode temperature
  temSymbol         = pref.getInt( "temSymbol", 1 );
  temColorSet       = pref.getInt( "temColorSet", 0 );
  temAColorR        = pref.getInt( "temAColorR", 255 );
  temAColorG        = pref.getInt( "temAColorG", 0 );
  temAColorB        = pref.getInt( "temAColorB", 0 );
  temBColorR        = pref.getInt( "temBColorR", 0 );
  temBColorG        = pref.getInt( "temBColorG", 255 );
  temBColorB        = pref.getInt( "temBColorB", 0 );
  temCColorR        = pref.getInt( "temCColorR", 0 );
  temCColorG        = pref.getInt( "temCColorG", 0 );
  temCColorB        = pref.getInt( "temCColorB", 255 );
  //  |- mode humidity
  humColorSet       = pref.getInt( "humColorSet", 0 );
  humAColorR        = pref.getInt( "humAColorR", 255 );
  humAColorG        = pref.getInt( "humAColorG", 255 );
  humAColorB        = pref.getInt( "humAColorB", 0 );
  humBColorR        = pref.getInt( "humBColorR", 0 );
  humBColorG        = pref.getInt( "humBColorG", 255 );
  humBColorB        = pref.getInt( "humBColorB", 255 );
  humCColorR        = pref.getInt( "humCColorR", 255 );
  humCColorG        = pref.getInt( "humCColorG", 0 );
  humCColorB        = pref.getInt( "humCColorB", 255 );
  //  |- mode scoreboard
  scoHColorR        = pref.getInt( "scoHColorR", 0 );
  scoHColorG        = pref.getInt( "scoHColorG", 0 );
  scoHColorB        = pref.getInt( "scoHColorB", 255 );
  scoAColorR        = pref.getInt( "scoAColorR", 0 );
  scoAColorG        = pref.getInt( "scoAColorG", 255 );
  scoAColorB        = pref.getInt( "scoAColorB", 255 );
  //  |- mode countdown
  couHColorR        = pref.getInt( "couHColorR", 255 );
  couHColorG        = pref.getInt( "couHColorG", 140 );
  couHColorB        = pref.getInt( "couHColorB", 0 );
  couMColorR        = pref.getInt( "couMColorR", 255 );
  couMColorG        = pref.getInt( "couMColorG", 165 );
  couMColorB        = pref.getInt( "couMColorB", 0 );
  couSColorR        = pref.getInt( "couSColorR", 255 );
  couSColorG        = pref.getInt( "couSColorG", 215 );
  couSColorB        = pref.getInt( "couSColorB", 0 );
  couAColorR        = pref.getInt( "couAColorR", 255 );
  couAColorG        = pref.getInt( "couAColorG", 0 );
  couAColorB        = pref.getInt( "couAColorB", 0 );
  //  |- mode scroll
  scrTime           = pref.getInt( "scrTime", 5 );
  
  Serial.println( "[+] All values loaded");
}

//  wifi
void initWifi() {
  Serial.println( "\nWiFi" );
  Serial.println( "--------------------------------------------------" );
  //  access point
  apLocalIP = IPAddress( apIP1, apIP2, apIP3, apIP4 );                    // populated and validated in web gui or predefinied
  apGateway = IPAddress( apGW1, apGW2, apGW3, apGW4 );                    // populated and validated in web gui or predefinied
  apSubnet = IPAddress( apSN1, apSN2, apSN3, apSN4 );                     // populated and validated in web gui or predefinied
  Serial.println( "[*] Creating access point ...");
  WiFi.mode( WIFI_AP_STA );                                               // set mode to access point and wifi client
  WiFi.softAPConfig( apLocalIP, apGateway, apSubnet );                    // configure access point
  WiFi.softAP( apSSID.c_str(), apPass.c_str(), 1, apHide );               // start access point
  Serial.println( "[+] Access point created" );
  Serial.print( "    SSID:          " );
  Serial.println( WiFi.softAPSSID() );
  Serial.print( "    Password:      " );
  Serial.println( apPass );
  Serial.print( "    IP Address:    " );
  Serial.println( WiFi.softAPIP() );

  //  wifi client
  if ( wifiDHCP ) {                                                       // special settings for wifi client
    wifiLocalIP = IPAddress( wifiIP1, wifiIP2, wifiIP3, wifiIP4 );        // populated in web gui
    wifiGateway = IPAddress( wifiGW1, wifiGW2, wifiGW3, wifiGW4 );        // populated in web gui
    wifiSubnet = IPAddress( wifiSN1, wifiSN2, wifiSN3, wifiSN4 );         // populated in web gui
  }
  Serial.println( "[*] Connecting to wifi ..." );
  WiFi.config( wifiLocalIP, wifiGateway, wifiSubnet );                    // configure wifi client
  WiFi.begin( wifiSSID.c_str(), wifiPass.c_str() );                       // start wifi client
  for ( int i = 0; i < 10; i++ ) {                                        // wait for wifi connection
    delay(200);
    if ( WiFi.status() == WL_CONNECTED ) {                                // check if connected to wifi
      Serial.println( "[+] Connected to wifi");
      Serial.print( "    SSID:          " );
      Serial.println( WiFi.SSID() );
      Serial.print( "    IP Address:    " );
      Serial.println( WiFi.localIP() );
      break;                                                              // if connected, leave the FOR
    }        
  }
  if ( WiFi.status() != WL_CONNECTED ) {                                  // if no wifi connection after the wait period
    WiFi.disconnect();                                                    // turn off wifi client connection
    Serial.println( "[!] Not connected to your network" );
  }
}

void reconnectWifi() {
  WiFi.disconnect();                                                    // close current connection
  WiFi.reconnect();                                                     // reconnect with previous settings
}

//  mdns
void initmDNS() {
  Serial.println( "\nmDNS" );
  Serial.println( "--------------------------------------------------" );
  if ( MDNS.begin( host.c_str() ) ) {
    Serial.println( "[+] mDNS started");
    Serial.print( "    Host:          " );
    Serial.println( host );
  } else {
    Serial.println( "[!] An error occured during mDNS start!" );
  }
}

//  spiffs
void initSPIFFS() {
  Serial.println( "\nSPIFFS" );
  Serial.println( "--------------------------------------------------" );
  if ( SPIFFS.begin( true ) ) {
    Serial.println( "[+] SPIFFS mounted correctly." );
  } else {
    Serial.println( "[!] An error occurred during SPIFFS mounting!" );
  }
}

//  ntp
void initNTP() {
  if ( WiFi.status() != WL_CONNECTED ) { return; }                        // NO wifi -> no call

  Serial.println( "\nNTP" );
  Serial.println( "--------------------------------------------------" );
  Serial.println( "[*] Connecting to NTP server ..." );
  configTime( 0, 0, clkAddr.c_str() );                                    // First connect to NTP server, with 0 TZ offset
  setenv( "TZ", clkZone.c_str(), 1 );                                     // Overwrite system timezone with defined timezone
  tzset();                                                                // Now adjust the timezone
  if ( getLocalTime( &ntpTime ) ) {
    Serial.println( "[+] NTP started correctly." );
  } else {
    Serial.println( "[!] An error occured during NTP starting!" );
  }
}

void getNTP() {
  if ( WiFi.status() != WL_CONNECTED ) { return; }                        // NO wifi -> no call

  if ( getLocalTime( &ntpTime ) ) {
    Serial.println( "[+] NTP fetch complete." );
  } else {
    Serial.println( "[!] NTP fetch failure!" );
  }
}

//  web
void initServer() {
  Serial.println( "\nWebserver" );
  Serial.println( "--------------------------------------------------" );
  Serial.println( "[*] Starting webserver ..." );
  server.serveStatic("/", SPIFFS, "/" );                                  // set path
  AsyncElegantOTA.begin(&server);                                         // start ElegantOTA
  server.begin();                                                         // start webserver
  Serial.println("[+] Web ready to surf");
}

//  led
void initLED() {
  ledClock.begin();                                                       // start ledstrip for the clock ...
  ledClock.show();                                                        // ... show() is mandatory
  ledDownlight.begin();                                                   // start ledstrip for the downlights ...
  ledDownlight.show();                                                    // ... show() is mandatory
}

void displayDownlights() {
  if ( dowUsage == 1 ) {
    if ( dowColorSet == 0 ) {
      dowColor = ledDownlight.Color( dowColorR, dowColorG, dowColorB );
      ledDownlight.fill( dowColor, 0, DOWNLIGHT_COUNT );
    }
    if ( dowColorSet == 1 ) {
      dowColor0 = ledDownlight.Color( dowColor0R, dowColor0G, dowColor0B );
      ledDownlight.fill( dowColor0, 0, 1 );
      dowColor1 = ledDownlight.Color( dowColor1R, dowColor1G, dowColor1B );
      ledDownlight.fill( dowColor1, 1, 1 );
      dowColor2 = ledDownlight.Color( dowColor2R, dowColor2G, dowColor2B );
      ledDownlight.fill( dowColor2, 2, 1 );
      dowColor3 = ledDownlight.Color( dowColor3R, dowColor3G, dowColor3B );
      ledDownlight.fill( dowColor3, 3, 1 );
      dowColor4 = ledDownlight.Color( dowColor4R, dowColor4G, dowColor4B );
      ledDownlight.fill( dowColor4, 4, 1 );
      dowColor5 = ledDownlight.Color( dowColor5R, dowColor5G, dowColor5B );
      ledDownlight.fill( dowColor5, 5, 1 );
      dowColor6 = ledDownlight.Color( dowColor6R, dowColor6G, dowColor6B );
      ledDownlight.fill( dowColor6, 6, 1 );
      dowColor7 = ledDownlight.Color( dowColor7R, dowColor7G, dowColor7B );
      ledDownlight.fill( dowColor7, 7, 1 );
      dowColor8 = ledDownlight.Color( dowColor8R, dowColor8G, dowColor8B );
      ledDownlight.fill( dowColor8, 8, 1 );
      dowColor9 = ledDownlight.Color( dowColor9R, dowColor9G, dowColor9B );
      ledDownlight.fill( dowColor9, 9, 1 );
      dowColor10 = ledDownlight.Color( dowColor10R, dowColor10G, dowColor10B );
      ledDownlight.fill( dowColor10, 10, 1 );
      dowColor11 = ledDownlight.Color( dowColor11R, dowColor11G, dowColor11B );
      ledDownlight.fill( dowColor11, 11, 1 );
      dowColor12 = ledDownlight.Color( dowColor12R, dowColor12G, dowColor12B );
      ledDownlight.fill( dowColor12, 12, 1 );
      dowColor13 = ledDownlight.Color( dowColor13R, dowColor13G, dowColor13B );
      ledDownlight.fill( dowColor13, 13, 1 );
    }
    if ( dowColorSet == 2 && (
      ( dowColorFreq == 0 ) ||
      ( dowColorFreq == 1 && downMinPassed ) ||
      ( dowColorFreq == 2 && downHourPassed ) ||
      ( dowColorFreq == 3 && downDayPassed ) ) ) {
      dowColor = ledDownlight.ColorHSV( random( 0, 65535 ), 255, 255 );
      ledDownlight.fill( dowColor, 0, DOWNLIGHT_COUNT );
    }
    if ( dowColorSet == 3 && (
      ( dowColorFreq == 0 ) ||
      ( dowColorFreq == 1 && downMinPassed ) ||
      ( dowColorFreq == 2 && downHourPassed ) ||
      ( dowColorFreq == 3 && downDayPassed ) ) ) {
      for ( int i = 0; i < DOWNLIGHT_COUNT; i++ ) {
        dowColor = ledDownlight.ColorHSV( random( 0, 65535 ), 255, 255 );
        ledDownlight.fill( dowColor, i, 1 );
      }
    }
  } else {
    ledDownlight.clear();                                                 // or turn them all off
  }

  downMinPassed = false;                                                  // reset the color change flag (1 minute)
  downHourPassed = false;                                                 // reset the color change flag (1 hour)
  downDayPassed = false;                                                  // reset the color change flag (1 day)

  ledDownlight.show();                                                    // show the downlights
}

void numberDisplay( int digitToDisplay, int offset, uint32_t color ) {
  switch ( digitToDisplay ) {
    case 0:   ledClock.fill( color, (  0 + offset ), 27 );                // 0
              ledClock.fill( color, ( 36 + offset ), 27 );
              break;
    case 1:   ledClock.fill( color, (  0 + offset ),  9 );                // 1
              ledClock.fill( color, ( 36 + offset ),  9 );
              break;
    case 2:   ledClock.fill( color, (  0 + offset ), 18 );                // 2
              ledClock.fill( color, ( 27 + offset ),  9 );
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 3:   ledClock.fill( color, (  0 + offset ), 18 );                // 3
              ledClock.fill( color, ( 27 + offset ), 27 );
              break;
    case 4:   ledClock.fill( color, (  0 + offset ),  9 );                // 4
              ledClock.fill( color, ( 18 + offset ), 27 );
              break;
    case 5:   ledClock.fill( color, (  9 + offset ), 45 );                // 5
              break;
    case 6:   ledClock.fill( color, (  9 + offset ), 54 );                // 6
              break;
    case 7:   ledClock.fill( color, (  0 + offset ), 18 );                // 7
              ledClock.fill( color, ( 36 + offset ),  9 );
              break;
    case 8:   ledClock.fill( color, (  0 + offset ), 63 );                // 8
              break;
    case 9:   ledClock.fill( color, (  0 + offset ), 54 );                // 9
              break;
    case 10:  ledClock.fill( color, (  0 + offset ), 45 );                // A
              ledClock.fill( color, ( 54 + offset ),  9 );
              break;
    case 11:  ledClock.fill( color, ( 18 + offset ), 45 );                // b
              break;
    case 12:  ledClock.fill( color, (  9 + offset ), 18 );                // C
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 13:  ledClock.fill( color, (  0 + offset ),  9 );                // d
              ledClock.fill( color, ( 27 + offset ), 36 );
              break;
    case 14:  ledClock.fill( color, (  9 + offset ), 27 );                // E
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 15:  ledClock.fill( color, (  9 + offset ), 27 );                // F
              ledClock.fill( color, ( 54 + offset ),  9 );
              break;
    case 16:  ledClock.fill( color, (  0 + offset ),  9 );                // G
              ledClock.fill( color, ( 27 + offset ), 36 );
              break;
    case 17:  ledClock.fill( color, (  0 + offset ),  9 );                // H
              ledClock.fill( color, ( 18 + offset ), 27 );
              ledClock.fill( color, ( 54 + offset ),  9 );
              break;
    case 18:  ledClock.fill( color, (  0 + offset ),  9 );                // I
              ledClock.fill( color, ( 36 + offset ),  9 );
              break;
    case 19:  ledClock.fill( color, (  0 + offset ),  0 );                // J
              ledClock.fill( color, ( 36 + offset ), 18 );
              break;
    case 20:  break;                                                      // K
    case 21:  ledClock.fill( color, ( 18 + offset ),  9 );                // L
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 22:  break;                                                      // M
    case 23:  ledClock.fill( color, ( 27 + offset ), 18 );                // n
              ledClock.fill( color, ( 54 + offset ),  9 );
              break;
    case 24:  ledClock.fill( color, ( 27 + offset ), 36 );                // o
              break;
    case 25:  ledClock.fill( color, (  0 + offset ), 36 );                // P
              ledClock.fill( color, ( 45 + offset ),  9 );
              break;
    case 26:  break;                                                      // Q
    case 27:  ledClock.fill( color, ( 27 + offset ),  9 );                // r
              ledClock.fill( color, ( 54 + offset ),  9 );
              break;
    case 28:  ledClock.fill( color, (  9 + offset ), 45 );                // S
              break;
    case 29:  ledClock.fill( color, ( 18 + offset ), 18 );                // t
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 30:  ledClock.fill( color, (  0 + offset ),  9 );                // U
              ledClock.fill( color, ( 18 + offset ),  9 );
              ledClock.fill( color, ( 36 + offset ), 27 );
              break;
    case 31:  break;                                                      // V
    case 32:  break;                                                      // W
    case 33:  break;                                                      // X
    case 34:  ledClock.fill( color, (  0 + offset ),  9 );                // Y
              ledClock.fill( color, ( 18 + offset ), 36 );
              break;
    case 35:  ledClock.fill( color, (  0 + offset ), 18 );                // Z
              ledClock.fill( color, ( 27 + offset ),  9 );
              ledClock.fill( color, ( 45 + offset ), 18 );
              break;
    case 36:  ledClock.fill( color, (  0 + offset ), 36 );                // °
              break;
    default:  break;
  }
}

void countdownEnd() {
  ledClock.setBrightness( 150 );
  couAColor = ledClock.Color( couAColorR, couAColorG, couAColorB ); 
  ledClock.clear();
  ledClock.show();
  for ( int i = 0; i < 5; i++ ) {
    numberDisplay( 14, 189, couAColor );
    numberDisplay( 23, 126, couAColor );
    numberDisplay( 13,  63, couAColor );
    numberDisplay( 14,   0, couAColor );
    ledClock.show();
    delay( 500 );
    ledClock.clear();
    ledClock.show();
    delay( 300 );
  }
  ledClock.clear();
  ledClock.show();
  delay( 5000 );
  if ( gloBrightness == 0 ) {                                             // value = 0 -> automatic mode
    ledClock.setBrightness( briClock );
  } else {
    ledClock.setBrightness( gloBrightness / 2 );
  }
  displayMode = 0;
  pref.putInt( "displayMode", 0 );
}

//  content
void getBrightness() {
  if ( WiFi.status() != WL_CONNECTED ) { return; }                        // NO wifi -> no call

  if ( gloBrightness == 0 ) {                                             // value = 0 -> automatic mode
    client.get( briKNXPath );                                             // call knx for value
    int brightnessKNX = client.responseBody().toInt();                    // write answer to value
    briClock = map( brightnessKNX, briKNXmin, briKNXmax, briClockMin, briClockMax );
    ledClock.setBrightness( briClock );
  } else {                                                                // value > 0 -> manual mode
    ledClock.setBrightness( gloBrightness / 2 );
  }
  if ( dowBrightness == 0 ) {
    client.get( briKNXPath );                                             // call knx for value
    int brightnessKNX = client.responseBody().toInt();                    // write answer to value
    briDown = map( brightnessKNX, briKNXmin, briKNXmax, briDownMin, briDownMax );
    ledDownlight.setBrightness( briDown );
  } else {
    ledDownlight.setBrightness( dowBrightness );
  }
}

void getHumidity() {
  if ( WiFi.status() != WL_CONNECTED ) { return; }                        // NO wifi -> no call

  client.get( humKNXPath );
  humKNXValue = int( ( client.responseBody().toDouble() ) * 10 );
}

void getTemperature() {
  if ( WiFi.status() != WL_CONNECTED ) { return; }                        // NO wifi -> no call

  client.get(temKNXPath);
  temKNXValue = int( ( client.responseBody().toDouble() ) * 10 );
}

//  webhandler
void initHandlers() {

  // default
  server.on( "/", HTTP_GET, []( AsyncWebServerRequest *request ){ request->send( SPIFFS, "/index.html", "text/html", false ); } );

  //  index.html
  server.on( "/goModeClk",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 0; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/goModeDat",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 1; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/goModeTem",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 2; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/goModeHum",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 3; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/goModeSco",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 4; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); scoHValue = request->arg( "scoHome" ).toInt(); if ( scoHValue < 0 ) { scoHValue = 0; } if ( scoHValue > 99 ) {scoHValue = 99; } scoAValue = request->arg( "scoAway" ).toInt(); if ( scoAValue < 0 ) { scoAValue = 0; } if ( scoAValue > 99 ) { scoAValue = 99; } } );
  server.on( "/goModeCou",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 5; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); couMillis = request->arg( "millis" ).toInt(); if ( couMillis > 86400000 ) { couMillis = 86400000; } couMillisEnd = millis() + couMillis; } );
  server.on( "/goModeScr",        HTTP_POST,  []( AsyncWebServerRequest *request ) { displayMode = 6; pref.putInt( "displayMode", displayMode ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/goDow",            HTTP_POST,  []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); dowUsage = request->arg( "dowUsage" ).toInt(); pref.putInt( "dowUsage", dowUsage ); dowColorSet = request->arg( "dowColorSet" ).toInt(); pref.putInt( "dowColorSet", dowColorSet ); } );

  //  settings.html
  //  |- global
  server.on( "/getGloBrightness",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( gloBrightness ) ); } );
  server.on( "/updGloBrightness", HTTP_POST,  []( AsyncWebServerRequest *request ) { gloBrightness = request->arg( "gloBrightness" ).toInt(); pref.putInt( "gloBrightness", gloBrightness ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriClockMin",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briClockMin ) ); } );
  server.on( "/updBriClockMin",   HTTP_POST,  []( AsyncWebServerRequest *request ) { briClockMin = request->arg( "briClockMin" ).toInt(); pref.putInt( "briClockMin", briClockMin ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriClockMax",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briClockMax ) ); } );
  server.on( "/updBriClockMax",   HTTP_POST,  []( AsyncWebServerRequest *request ) { briClockMax = request->arg( "briClockMax" ).toInt(); pref.putInt( "briClockMax", briClockMax ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getGloColorFreq",              []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( gloColorFreq ) ); } );
  server.on( "/updGloColorFreq",  HTTP_POST,  []( AsyncWebServerRequest *request ) { gloColorFreq = request->arg( "gloColorFreq" ).toInt(); pref.putInt( "gloColorFreq", gloColorFreq ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowUsage",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( dowUsage ) ); } );
  server.on( "/updDowUsage",      HTTP_POST,  []( AsyncWebServerRequest *request ) { dowUsage = request->arg( "dowUsage" ).toInt(); pref.putInt( "dowUsage", dowUsage ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowBrightness",             []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( dowBrightness ) ); } );
  server.on( "/updDowBrightness", HTTP_POST,  []( AsyncWebServerRequest *request ) { dowBrightness = request->arg( "dowBrightness" ).toInt(); pref.putInt( "dowBrightness", dowBrightness ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriDownMin",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briDownMin ) ); } );
  server.on( "/updBriDownMin",    HTTP_POST,  []( AsyncWebServerRequest *request ) { briDownMin = request->arg( "briDownMin" ).toInt(); pref.putInt( "briDownMin", briDownMin ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriDownMax",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briDownMax ) ); } );
  server.on( "/updBriDownMax",    HTTP_POST,  []( AsyncWebServerRequest *request ) { briDownMax = request->arg( "briDownMax" ).toInt(); pref.putInt( "briDownMax", briDownMax ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColorFreq",              []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( dowColorFreq ) ); } );
  server.on( "/updDowColorFreq",  HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColorFreq = request->arg( "dowColorFreq" ).toInt(); pref.putInt( "dowColorFreq", dowColorFreq ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); });
  //  |- knx
  server.on( "/getKNXAddr",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxAddr ) ); } );
  server.on( "/updKNXAddr",       HTTP_POST,  []( AsyncWebServerRequest *request ) { knxAddr = request->arg( "knxAddr" ); pref.putString( "knxAddr", knxAddr ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getKNXPort",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( knxPort ) ); } );
  server.on( "/updKNXPort",       HTTP_POST,  []( AsyncWebServerRequest *request ) { knxPort = request->arg( "knxPort" ).toInt(); pref.putInt( "knxPort", knxPort ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriKNXPath",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briKNXPath ) ); } );
  server.on( "/updBriKNXPath",    HTTP_POST,  []( AsyncWebServerRequest *request ) { briKNXPath = request->arg( "briKNXPath" ); pref.putString( "briKNXPath", briKNXPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriKNXmin",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briKNXmin ) ); } );
  server.on( "/updBriKNXmin",     HTTP_POST,  []( AsyncWebServerRequest *request ) { briKNXmin = request->arg( "briKNXmin" ).toInt(); pref.putInt( "briKNXmin", briKNXmin ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getBriKNXmax",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( briKNXmax ) ); } );
  server.on( "/updBriKNXmax",     HTTP_POST,  []( AsyncWebServerRequest *request ) { briKNXmax = request->arg( "briKNXmax" ).toInt(); pref.putInt( "briKNXmax", briKNXmax ); getBrightness(); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getTemKNXPath",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( temKNXPath ) ); } );
  server.on( "/updTemKNXPath",    HTTP_POST,  []( AsyncWebServerRequest *request ) { temKNXPath = request->arg( "temKNXPath" ); pref.putString( "temKNXPath", temKNXPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getHumKNXPath",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( humKNXPath ) ); } );
  server.on( "/updHumKNXPath",    HTTP_POST,  []( AsyncWebServerRequest *request ) { humKNXPath = request->arg( "humKNXPath" ); pref.putString( "humKNXPath", humKNXPath ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- clock
  server.on( "/getClkAddr",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkAddr ) ); } );
  server.on( "/updClkAddr",       HTTP_POST,  []( AsyncWebServerRequest *request ) { clkAddr = request->arg( "clkAddr" ); pref.putString( "clkAddr", clkAddr ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkZone",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkZone ) ); } );
  server.on( "/updClkZone",       HTTP_POST,  []( AsyncWebServerRequest *request ) { clkZone = request->arg( "clkZone" ); pref.putString( "clkZone", clkZone ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkFormat",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkFormat ) ); } );
  server.on( "/updClkFormat",     HTTP_POST,  []( AsyncWebServerRequest *request ) { clkFormat = request->arg( "clkFormat" ).toInt(); pref.putInt( "clkFormat", clkFormat ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkColorSet",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( clkColorSet ) ); } );
  server.on( "/updClkColorSet",   HTTP_POST,  []( AsyncWebServerRequest *request ) { clkColorSet = request->arg( "clkColorSet" ).toInt(); pref.putInt( "clkColorSet", clkColorSet ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkHColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", clkHColorR, clkHColorG, clkHColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updClkHColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { clkHColorR = request->arg( "r" ).toInt(); clkHColorG = request->arg( "g" ).toInt(); clkHColorB = request->arg( "b" ).toInt(); pref.putInt( "clkHColorR", clkHColorR ); pref.putInt( "clkHColorG", clkHColorG ); pref.putInt( "clkHColorB", clkHColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getClkMColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", clkMColorR, clkMColorG, clkMColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updClkMColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { clkMColorR = request->arg( "r" ).toInt(); clkMColorG = request->arg( "g" ).toInt(); clkMColorB = request->arg( "b" ).toInt(); pref.putInt( "clkMColorR", clkMColorR ); pref.putInt( "clkMColorG", clkMColorG ); pref.putInt( "clkMColorB", clkMColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- date
  server.on( "/getDatFormat",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datFormat ) ); } );
  server.on( "/updDatFormat",     HTTP_POST,  []( AsyncWebServerRequest *request ) { datFormat = request->arg( "datFormat" ).toInt(); pref.putInt( "datFormat", datFormat ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDatColorSet",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( datColorSet ) ); } );
  server.on( "/updDatColorSet",   HTTP_POST,  []( AsyncWebServerRequest *request ) { datColorSet = request->arg( "datColorSet" ).toInt(); pref.putInt( "datColorSet", datColorSet ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDatDColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", datDColorR, datDColorG, datDColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDatDColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { datDColorR = request->arg( "r" ).toInt(); datDColorG = request->arg( "g" ).toInt(); datDColorB = request->arg( "b" ).toInt(); pref.putInt( "datDColorR", datDColorR ); pref.putInt( "datDColorG", datDColorG ); pref.putInt( "datDColorB", datDColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDatMColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", datMColorR, datMColorG, datMColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDatMColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { datMColorR = request->arg( "r" ).toInt(); datMColorG = request->arg( "g" ).toInt(); datMColorB = request->arg( "b" ).toInt(); pref.putInt( "datMColorR", datMColorR ); pref.putInt( "datMColorG", datMColorG ); pref.putInt( "datMColorB", datMColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- temperature
  server.on( "/getTemSymbol",                 []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( temSymbol ) ); } );
  server.on( "/updTemSymbol",     HTTP_POST,  []( AsyncWebServerRequest *request ) { temSymbol =request->arg( "temSymbol" ).toInt(); pref.putInt( "temSymbol", temSymbol ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getTemColorSet",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( temColorSet ) ); } );
  server.on( "/updTemColorSet",   HTTP_POST,  []( AsyncWebServerRequest *request ) { temColorSet = request->arg( "temColorSet" ).toInt(); pref.putInt( "temColorSet", temColorSet ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getTemAColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", temAColorR, temAColorG, temAColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updTemAColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { temAColorR = request->arg( "r" ).toInt(); temAColorG = request->arg( "g" ).toInt(); temAColorB = request->arg( "b" ).toInt(); pref.putInt( "temAColorR", temAColorR ); pref.putInt( "temAColorG", temAColorG ); pref.putInt( "temAColorB", temAColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getTemBColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", temBColorR, temBColorG, temBColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updTemBColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { temBColorR = request->arg( "r" ).toInt(); temBColorG = request->arg( "g" ).toInt(); temBColorB = request->arg( "b" ).toInt(); pref.putInt( "temBColorR", temBColorR ); pref.putInt( "temBColorG", temBColorG ); pref.putInt( "temBColorB", temBColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getTemCColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", temCColorR, temCColorG, temCColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updTemCColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { temCColorR = request->arg( "r" ).toInt(); temCColorG = request->arg( "g" ).toInt(); temCColorB = request->arg( "b" ).toInt(); pref.putInt( "temCColorR", temCColorR ); pref.putInt( "temCColorG", temCColorG ); pref.putInt( "temCColorB", temCColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- humidity
  server.on( "/getHumColorSet",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( humColorSet ) ); } );
  server.on( "/updHumColorSet",   HTTP_POST,  []( AsyncWebServerRequest *request ) { humColorSet = request->arg( "humColorSet" ).toInt(); pref.putInt( "humColorSet", humColorSet ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getHumAColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", humAColorR, humAColorG, humAColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updHumAColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { humAColorR = request->arg( "r" ).toInt(); humAColorG = request->arg( "g" ).toInt(); humAColorB = request->arg( "b" ).toInt(); pref.putInt( "humAColorR", humAColorR ); pref.putInt( "humAColorG", humAColorG ); pref.putInt( "humAColorB", humAColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getHumBColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", humBColorR, humBColorG, humBColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updHumBColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { humBColorR = request->arg( "r" ).toInt(); humBColorG = request->arg( "g" ).toInt(); humBColorB = request->arg( "b" ).toInt(); pref.putInt( "humBColorR", humBColorR ); pref.putInt( "humBColorG", humBColorG ); pref.putInt( "humBColorB", humBColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getHumCColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", humCColorR, humCColorG, humCColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updHumCColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { humCColorR = request->arg( "r" ).toInt(); humCColorG = request->arg( "g" ).toInt(); humCColorB = request->arg( "b" ).toInt(); pref.putInt( "humCColorR", humCColorR ); pref.putInt( "humCColorG", humCColorG ); pref.putInt( "humCColorB", humCColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- scoreboard
  server.on( "/getScoHColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", scoHColorR, scoHColorG, scoHColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updScoHColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { scoHColorR = request->arg( "r" ).toInt(); scoHColorG = request->arg( "g" ).toInt(); scoHColorB = request->arg( "b" ).toInt(); pref.putInt( "scoHColorR", scoHColorR ); pref.putInt( "scoHColorG", scoHColorG ); pref.putInt( "scoHColorB", scoHColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getScoAColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", scoAColorR, scoAColorG, scoAColorB) ; request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updScoAColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { scoAColorR = request->arg( "r" ).toInt(); scoAColorG = request->arg( "g" ).toInt(); scoAColorB = request->arg( "b" ).toInt(); pref.putInt( "scoAColorR", scoAColorR ); pref.putInt( "scoAColorG", scoAColorG ); pref.putInt( "scoAColorB", scoAColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- countdown
  server.on( "/getCouHColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", couHColorR, couHColorG, couHColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updCouHColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { couHColorR = request->arg( "r" ).toInt(); couHColorG = request->arg( "g" ).toInt(); couHColorB = request->arg( "b" ).toInt(); pref.putInt( "couHColorR", couHColorR ); pref.putInt( "couHColorG", couHColorG ); pref.putInt( "couHColorB", couHColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getCouMColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", couMColorR, couMColorG, couMColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updCouMColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { couMColorR = request->arg( "r" ).toInt(); couMColorG = request->arg( "g" ).toInt(); couMColorB = request->arg( "b" ).toInt(); pref.putInt( "couMColorR", couMColorR ); pref.putInt( "couMColorG", couMColorG ); pref.putInt( "couMColorB", couMColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getCouSColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", couSColorR, couSColorG, couSColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updCouSColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { couSColorR = request->arg( "r" ).toInt(); couSColorG = request->arg( "g" ).toInt(); couSColorB = request->arg( "b" ).toInt(); pref.putInt( "couSColorR", couSColorR ); pref.putInt( "couSColorG", couSColorG ); pref.putInt( "couSColorB", couSColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getCouAColor",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", couAColorR, couAColorG, couAColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updCouAColor",     HTTP_POST,  []( AsyncWebServerRequest *request ) { couAColorR = request->arg( "r" ).toInt(); couAColorG = request->arg( "g" ).toInt(); couAColorB = request->arg( "b" ).toInt(); pref.putInt( "couAColorR", couAColorR ); pref.putInt( "couAColorG", couAColorG ); pref.putInt( "couAColorB", couAColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- scroll
  server.on( "/getScrTime",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( scrTime ) ); } );
  server.on( "/updScrTime",       HTTP_POST,  []( AsyncWebServerRequest *request ) { scrTime = request->arg( "scrTime" ).toInt(); pref.putInt( "scrTime", scrTime ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- downlights
  server.on( "/getDowColorSet",               []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( dowColorSet ) ); } );
  server.on( "/updDowColorSet",   HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColorSet = request->arg( "dowColorSet" ).toInt(); pref.putInt( "dowColorSet", dowColorSet ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor",                  []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColorR, dowColorG, dowColorB ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor",      HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColorR = request->arg( "r" ).toInt(); dowColorG = request->arg( "g" ).toInt(); dowColorB = request->arg( "b" ).toInt(); pref.putInt( "dowColorR", dowColorR ); pref.putInt( "dowColorG", dowColorG ); pref.putInt( "dowColorB", dowColorB ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor0",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor0R, dowColor0G, dowColor0B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor0",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor0R = request->arg( "r" ).toInt(); dowColor0G = request->arg( "g" ).toInt(); dowColor0B = request->arg( "b" ).toInt(); pref.putInt( "dowColor0R", dowColor0R ); pref.putInt( "dowColor0G", dowColor0G ); pref.putInt( "dowColor0B", dowColor0B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor1",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor1R, dowColor1G, dowColor1B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor1",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor1R = request->arg( "r" ).toInt(); dowColor1G = request->arg( "g" ).toInt(); dowColor1B = request->arg( "b" ).toInt(); pref.putInt( "dowColor1R", dowColor1R ); pref.putInt( "dowColor1G", dowColor1G ); pref.putInt( "dowColor1B", dowColor1B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor2",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor2R, dowColor2G, dowColor2B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor2",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor2R = request->arg( "r" ).toInt(); dowColor2G = request->arg( "g" ).toInt(); dowColor2B = request->arg( "b" ).toInt(); pref.putInt( "dowColor2R", dowColor2R ); pref.putInt( "dowColor2G", dowColor2G ); pref.putInt( "dowColor2B", dowColor2B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor3",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor3R, dowColor3G, dowColor3B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor3",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor3R = request->arg( "r" ).toInt(); dowColor3G = request->arg( "g" ).toInt(); dowColor3B = request->arg( "b" ).toInt(); pref.putInt( "dowColor3R", dowColor3R ); pref.putInt( "dowColor3G", dowColor3G ); pref.putInt( "dowColor3B", dowColor3B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor4",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor4R, dowColor4G, dowColor4B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor4",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor4R = request->arg( "r" ).toInt(); dowColor4G = request->arg( "g" ).toInt(); dowColor4B = request->arg( "b" ).toInt(); pref.putInt( "dowColor4R", dowColor4R ); pref.putInt( "dowColor4G", dowColor4G ); pref.putInt( "dowColor4B", dowColor4B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor5",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor5R, dowColor5G, dowColor5B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor5",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor5R = request->arg( "r" ).toInt(); dowColor5G = request->arg( "g" ).toInt(); dowColor5B = request->arg( "b" ).toInt(); pref.putInt( "dowColor5R", dowColor5R ); pref.putInt( "dowColor5G", dowColor5G ); pref.putInt( "dowColor5B", dowColor5B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor6",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor6R, dowColor6G, dowColor6B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor6",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor6R = request->arg( "r" ).toInt(); dowColor6G = request->arg( "g" ).toInt(); dowColor6B = request->arg( "b" ).toInt(); pref.putInt( "dowColor6R", dowColor6R ); pref.putInt( "dowColor6G", dowColor6G ); pref.putInt( "dowColor6B", dowColor6B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor7",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor7R, dowColor7G, dowColor7B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor7",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor7R = request->arg( "r" ).toInt(); dowColor7G = request->arg( "g" ).toInt(); dowColor7B = request->arg( "b" ).toInt(); pref.putInt( "dowColor7R", dowColor7R ); pref.putInt( "dowColor7G", dowColor7G ); pref.putInt( "dowColor7B", dowColor7B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor8",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor8R, dowColor8G, dowColor8B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor8",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor8R = request->arg( "r" ).toInt(); dowColor8G = request->arg( "g" ).toInt(); dowColor8B = request->arg( "b" ).toInt(); pref.putInt( "dowColor8R", dowColor8R ); pref.putInt( "dowColor8G", dowColor8G ); pref.putInt( "dowColor8B", dowColor8B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor9",                 []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor9R, dowColor9G, dowColor9B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor9",     HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor9R = request->arg( "r" ).toInt(); dowColor9G = request->arg( "g" ).toInt(); dowColor9B = request->arg( "b" ).toInt(); pref.putInt( "dowColor9R", dowColor9R ); pref.putInt( "dowColor9G", dowColor9G ); pref.putInt( "dowColor9B", dowColor9B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor10",                []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor10R, dowColor10G, dowColor10B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor10",    HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor10R = request->arg( "r" ).toInt(); dowColor10G = request->arg( "g" ).toInt(); dowColor10B = request->arg( "b" ).toInt(); pref.putInt( "dowColor10R", dowColor10R ); pref.putInt( "dowColor10G", dowColor10G ); pref.putInt( "dowColor10B", dowColor10B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor11",                []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor11R, dowColor11G, dowColor11B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor11",    HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor11R = request->arg( "r" ).toInt(); dowColor11G = request->arg( "g" ).toInt(); dowColor11B = request->arg( "b" ).toInt(); pref.putInt( "dowColor11R", dowColor11R ); pref.putInt( "dowColor11G", dowColor11G ); pref.putInt( "dowColor11B", dowColor11B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor12",                []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor12R, dowColor12G, dowColor12B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor12",    HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor12R = request->arg( "r" ).toInt(); dowColor12G = request->arg( "g" ).toInt(); dowColor12B = request->arg( "b" ).toInt(); pref.putInt( "dowColor12R", dowColor12R ); pref.putInt( "dowColor12G", dowColor12G ); pref.putInt( "dowColor12B", dowColor12B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getDowColor13",                []( AsyncWebServerRequest *request ) { char tempcolor[8]; sprintf( tempcolor, "#%02X%02X%02X", dowColor13R, dowColor13G, dowColor13B ); request->send( 200, "text/plain", tempcolor ); } );
  server.on( "/updDowColor13",    HTTP_POST,  []( AsyncWebServerRequest *request ) { dowColor13R = request->arg( "r" ).toInt(); dowColor13G = request->arg( "g" ).toInt(); dowColor13B = request->arg( "b" ).toInt(); pref.putInt( "dowColor13R", dowColor13R ); pref.putInt( "dowColor13G", dowColor13G ); pref.putInt( "dowColor13B", dowColor13B ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  wifi.html
  server.on( "/getAPSSID",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSSID           ) ); } );
  server.on( "/updAPSSID",        HTTP_POST,  []( AsyncWebServerRequest *request ) { apSSID = request->arg( "apSSID" ); pref.putString( "apSSID", apSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPPass",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apPass           ) ); } );
  server.on( "/updAPPass",        HTTP_POST,  []( AsyncWebServerRequest *request ) { apPass = request->arg( "apPass" ); pref.putString( "apPass", apPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPHide",                    []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apHide           ) ); } );
  server.on( "/updAPHide",        HTTP_POST,  []( AsyncWebServerRequest *request ) { apHide = request->arg( "apHide" ).toInt(); pref.putInt( "apHide", apHide ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP1            ) ); } );
  server.on( "/updAPIP1",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apIP1 = request->arg( "apIP1" ).toInt(); pref.putInt( "apIP1", apIP1 ); pref.putInt( "apGW1", apIP1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP2            ) ); } );
  server.on( "/updAPIP2",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apIP2 = request->arg( "apIP2" ).toInt(); pref.putInt( "apIP2", apIP2 ); pref.putInt( "apGW2", apIP2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP3            ) ); } );
  server.on( "/updAPIP3",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apIP3 = request->arg( "apIP3" ).toInt(); pref.putInt( "apIP3", apIP3 ); pref.putInt( "apGW3", apIP3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPIP4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apIP4            ) ); } );
  server.on( "/updAPIP4",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apIP4 = request->arg( "apIP4" ).toInt(); pref.putInt( "apIP4", apIP4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPGW1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW1            ) ); } );
  server.on( "/updAPGW1",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apGW1 = request->arg( "apGW1" ).toInt(); pref.putInt( "apGW1", apGW1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPGW2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW2            ) ); } );
  server.on( "/updAPGW2",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apGW2 = request->arg( "apGW2" ).toInt(); pref.putInt( "apGW2", apGW2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPGW3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW3            ) ); } );
  server.on( "/updAPGW3",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apGW3 = request->arg( "apGW3" ).toInt(); pref.putInt( "apGW3", apGW3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPGW4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apGW4            ) ); } );
  server.on( "/updAPGW4",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apGW4 = request->arg( "apGW4" ).toInt(); pref.putInt( "apGW4", apGW4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPSN1",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN1            ) ); } );
  server.on( "/updAPSN1",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apSN1 = request->arg( "apSN1" ).toInt(); pref.putInt( "apSN1", apSN1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPSN2",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN2            ) ); } );
  server.on( "/updAPSN2",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apSN2 = request->arg( "apSN2" ).toInt(); pref.putInt( "apSN2", apSN2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPSN3",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN3            ) ); } );
  server.on( "/updAPSN3",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apSN3 = request->arg( "apSN3" ).toInt(); pref.putInt( "apSN3", apSN3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getAPSN4",                     []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( apSN4            ) ); } );
  server.on( "/updAPSN4",         HTTP_POST,  []( AsyncWebServerRequest *request ) { apSN4 = request->arg( "apSN4" ).toInt(); pref.putInt( "apSN4", apSN4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/apRestart",        HTTP_POST,  []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); WiFi.softAPdisconnect(); WiFi.softAPConfig( apLocalIP, apGateway, apSubnet ); WiFi.softAP( apSSID.c_str(), apPass.c_str(), 1, apHide ); } );
  server.on( "/getWifiConfig",                []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiConfig       ) ); } );
  server.on( "/updWifiConfig",    HTTP_POST,  []( AsyncWebServerRequest *request ) { if ( request->arg( "wifiConfig" ) == "true" ) { wifiConfig = 1; } if ( request->arg( "wifiConfig" ) == "false" ) { wifiConfig = 0; } pref.putInt( "wifiConfig", wifiConfig ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSSID",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSSID         ) ); } );
  server.on( "/updWifiSSID",      HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiSSID = request->arg( "wifiSSID" ); pref.putString( "wifiSSID", wifiSSID ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiPass",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiPass         ) ); } );
  server.on( "/updWifiPass",      HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiPass = request->arg( "wifiPass" ); pref.putString( "wifiPass", wifiPass ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiDHCP",                  []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiDHCP         ) ); } );
  server.on( "/updWifiDHCP",      HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiDHCP = request->arg( "wifiDHCP" ).toInt(); pref.putInt( "wifiDHCP", wifiDHCP ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiIP1",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP1          ) ); } );
  server.on( "/updWifiIP1",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiIP1 = request->arg( "wifiIP1" ).toInt(); pref.putInt( "wifiIP1", wifiIP1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiIP2",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP2          ) ); } );
  server.on( "/updWifiIP2",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiIP2 = request->arg( "wifiIP2" ).toInt(); pref.putInt( "wifiIP2", wifiIP2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiIP3",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP3          ) ); } );
  server.on( "/updWifiIP3",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiIP3 = request->arg( "wifiIP3" ).toInt(); pref.putInt( "wifiIP3", wifiIP3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiIP4",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiIP4          ) ); } );
  server.on( "/updWifiIP4",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiIP4 = request->arg( "wifiIP4" ).toInt(); pref.putInt( "wifiIP4", wifiIP4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiGW1",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW1          ) ); } );
  server.on( "/updWifiGW1",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiGW1 = request->arg( "wifiGW1" ).toInt(); pref.putInt( "wifiGW1", wifiGW1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiGW2",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW2          ) ); } );
  server.on( "/updWifiGW2",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiGW2 = request->arg( "wifiGW2" ).toInt(); pref.putInt( "wifiGW2", wifiGW2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiGW3",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW3          ) ); } );
  server.on( "/updWifiGW3",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiGW3 = request->arg( "wifiGW3" ).toInt(); pref.putInt( "wifiGW3", wifiGW3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiGW4",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiGW4          ) ); } );
  server.on( "/updWifiGW4",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiGW4 = request->arg( "wifiGW4" ).toInt(); pref.putInt( "wifiGW4", wifiGW4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSN1",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN1          ) ); } );
  server.on( "/updWifiSN1",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiSN1 = request->arg( "wifiSN1" ).toInt(); pref.putInt( "wifiSN1", wifiSN1 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSN2",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN2          ) ); } );
  server.on( "/updWifiSN2",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiSN2 = request->arg( "wifiSN2" ).toInt(); pref.putInt( "wifiSN2", wifiSN2 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSN3",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN3          ) ); } );
  server.on( "/updWifiSN3",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiSN3 = request->arg( "wifiSN3" ).toInt(); pref.putInt( "wifiSN3", wifiSN3 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/getWifiSN4",                   []( AsyncWebServerRequest *request ) { request->send( 200, "text/plain", String( wifiSN4          ) ); } );
  server.on( "/updWifiSN4",       HTTP_POST,  []( AsyncWebServerRequest *request ) { wifiSN4 = request->arg( "wifiSN4" ).toInt(); pref.putInt( "wifiSN4", wifiSN4 ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  server.on( "/wifiRestart",      HTTP_POST,  []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); WiFi.disconnect(); WiFi.config( wifiLocalIP, wifiGateway, wifiSubnet ); WiFi.begin( wifiSSID.c_str(), wifiPass.c_str() ); for ( int i = 0; i < 100; i++ ) { delay(100); if ( WiFi.status() == WL_CONNECTED ) { Serial.println( "[+] Connected to wifi"); Serial.print( "    SSID:          " ); Serial.println( WiFi.SSID() ); Serial.print( "    IP Address:    " ); Serial.println( WiFi.localIP() ); break; } } if ( WiFi.status() != WL_CONNECTED ) { WiFi.disconnect(); Serial.println( "[!] Not connected to your network" ); } } );

  //  help.html
  //  |- date & time 
  server.on( "/updDateTime",      HTTP_POST,  []( AsyncWebServerRequest *request ) { int yeararg = request->arg( "year" ).toInt(); int montharg = request->arg( "month" ).toInt(); int dayarg = request->arg( "day" ).toInt(); int hourarg = request->arg( "hour" ).toInt(); int minarg = request->arg( "min" ).toInt(); int secarg = request->arg( "sec" ).toInt(); ntpTime.tm_year = yeararg - 1900; ntpTime.tm_mon = montharg - 1; ntpTime.tm_mday = dayarg; ntpTime.tm_hour = hourarg; ntpTime.tm_min = minarg; ntpTime.tm_sec = secarg; time_t t = mktime( &ntpTime ); struct timeval now1 = { .tv_sec = t }; settimeofday( &now1, NULL ); request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); } );
  //  |- resets
  server.on( "/resetFlash",       HTTP_POST,  []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); pref.clear(); ESP.restart(); } );
  server.on( "/resetESP",         HTTP_POST,  []( AsyncWebServerRequest *request ) { request->send( 200, "text/json", "{ \"result\":\"ok\" }" ); ESP.restart(); } );
}

//  task1 on core0
void taskCore0code( void * pvParameters ){
  for(;;) {                                                               // required to keep the task running

    // Setup time-passage trackers
    int currentTimeSec = ntpTime.tm_sec;
    int currentTimeMin = ntpTime.tm_min;
    int currentTimeHour = ntpTime.tm_hour;
    int currentTimeDay = ntpTime.tm_mday;
    int m1 = currentTimeMin / 10;
    int m2 = currentTimeMin % 10;

    currMillisCore0 = millis();                                           // update current time reference

    if ( ( currMillisCore0 - prevTimeSecCore0 ) >= 1000) {                // run every 1 second
      prevTimeSecCore0 = currMillisCore0;                                 // update previous time reference
      displayDownlights();                                                // control the downlights
      if ( WiFi.status() != WL_CONNECTED ) { reconnectWifi(); }           // NO wifi -> reconnect
    }

    if( abs( currentTimeMin - prevTimeMinCore0 ) >= 1 ) {                 // run every 1 minute
      prevTimeMinCore0 = currentTimeMin;
      clockMinPassed = true;                                              // set the flag for the clock leds
      downMinPassed = true;                                               // set the flag for the downlight leds
      getBrightness();
    }

    if( ( m2 == 0 || m2 == 5 ) && ( currentTimeSec == 0 ) ) {             // run every 5 minutes
      getHumidity();
      getTemperature();
    }

    if( ( m2 == 0 ) && ( currentTimeSec == 0) ) {                         // run every 10 minutes
      /* code goes here */
    }

    if( ( ( m1 == 0 && m2 == 0 ) || ( m1 == 1 && m2 == 5 ) ||             // run every 15 minutes
      ( m1 == 3 && m2 == 0 ) || ( m1 == 4 && m2 == 5 ) ) &&
      ( currentTimeSec == 0 ) ) {
      /* code goes here */
    }

    if( ( ( m1 == 0 && m2 == 0 ) ||                                       // run every 30 minutes
          ( m1 == 3 && m2 == 0 ) ) &&
          ( currentTimeSec == 0 ) ) {
      /* code goes here */
    }

    if( abs( currentTimeHour - prevTimeHourCore0 ) >= 1 ) {               // run every hour
      prevTimeHourCore0 = currentTimeHour;
      clockHourPassed = true;                                             // set the flag for the clock leds
      downHourPassed = true;                                              // set the flag for the downlight leds
      getNTP();
    }

    if( abs( currentTimeDay - prevTimeDayCore0 ) >= 1 ) {                 // run every day
      prevTimeDayCore0 = currentTimeDay;
      clockDayPassed = true;                                              // set the flag for the clock leds
      downDayPassed = true;                                               // set the flag for the downlight leds
    }
  }
}



//  ----------------------------------------------------------------------------------------------------
//  MAIN FUNCTIONS
//  ----------------------------------------------------------------------------------------------------
//  These functions are literally the content shown on the front side of the shelf:
//  modeClk:  current local time according to the timezone settings
//  modeDat:  current local date according to the timezone settings
//  modeTem:  current temperature obtained from your smarthome system (REST API)
//  modeHum:  current humidity obtained from your smarthome system (REST API)
//  modeSco:  scoreboard
//  modeCou:  countdown
//  modeScr:  scroll mode


//  ----- CLOCK -----
void modeClk() {                                                          // display the current time
  ledClock.clear();                                                       // clear all edge led's

  // get current time
  int hour = ntpTime.tm_hour;                                             // hours
  int mins = ntpTime.tm_min;                                              // minutes
  int secs = ntpTime.tm_sec;                                              // seconds

  // adjust hours for 12 hour (AM/PM)
  if ( !clkFormat ) {
    if ( hour > 12 ) { hour = hour - 12; }                                // if hours is above 12, turn it from 13:mm into 01:mm PM
    if ( hour < 1 ) { hour = hour + 12; }                                 // if hours is 00:mm, turn it from 00:mm into 12:mm AM
  }

  // build the digits
  int h1 = floor( hour / 10 );                                            // build h1
  int h2 = hour % 10;                                                     // build h2
  int m1 = floor( mins / 10 );                                            // build m1
  int m2 = mins % 10;                                                     // build m2
  int s1 = floor( secs / 10 );                                            // build s1
  int s2 = secs % 10;                                                     // build s2

  // set the colors
  if ( clkColorSet == 0 ) {                                               // two defined colors: one for hours, one for minutes
    clkHColor = ledClock.Color( clkHColorR, clkHColorG, clkHColorB );
    clkMColor = ledClock.Color( clkMColorR, clkMColorG, clkMColorB );
  }
  if ( clkColorSet == 1 && (                                              // two random colors: one for hours, one for minutes
    ( gloColorFreq == 0 ) ||                                              // change color every second
    ( gloColorFreq == 1 && clockMinPassed ) ||                            // change color every minute and 1 minute passed
    ( gloColorFreq == 2 && clockHourPassed ) ||                           // change color every hour and 1 hour passed
    ( gloColorFreq == 3 && clockDayPassed ) ) ) {                         // change color every day and 1 day passed
    clkHColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
    clkMColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
  }

  // display the time
  numberDisplay( h1, 189, clkHColor );                                    // show first digit
  numberDisplay( h2, 126, clkHColor );                                    // show second digit
  numberDisplay( m1,  63, clkMColor );                                    // show third digit
  numberDisplay( m2,   0, clkMColor );                                    // show fourth digit

  clockMinPassed = false;                                                 // reset the color change flag (1 minute)
  clockHourPassed = false;                                                // reset the color change flag (1 hour)
  clockDayPassed = false;                                                 // reset the color change flag (1 day)
}


//  ----- DATE -----
void modeDat() {                                                          // display the current date
  ledClock.clear();                                                       // clear all edge led's

  // build the digits
  int d1 = floor( ntpTime.tm_mday / 10 );                                 // build d1
  int d2 = ntpTime.tm_mday % 10;                                          // build d2
  int m1 = floor( ( ntpTime.tm_mon + 1 ) / 10 );                          // build m1
  int m2 = ( ntpTime.tm_mon + 1 ) % 10;                                   // build m2

  // set the colors
  if ( datColorSet == 0 ) {                                               // two defined colors: one for days, one for months
    datDColor = ledClock.Color( datDColorR, datDColorG, datDColorB );
    datMColor = ledClock.Color( datMColorR, datMColorG, datMColorB );
  }
  if ( datColorSet == 1 && (                                              // two random colors: one for hours, one for minutes
    ( gloColorFreq == 0 ) ||                                              // change color every second
    ( gloColorFreq == 1 && clockMinPassed ) ||                            // change color every minute and 1 minute passed
    ( gloColorFreq == 2 && clockHourPassed ) ||                           // change color every hour and 1 hour passed
    ( gloColorFreq == 3 && clockDayPassed ) ) ) {                         // change color every day and 1 day passed
    datDColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
    datMColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
  }

  // display the digits
  if ( datFormat ) {
    numberDisplay( d1, 189, datDColor );                                  // show first digit
    numberDisplay( d2, 126, datDColor );                                  // show second digit
    numberDisplay( m1,  63, datMColor );                                  // show third digit
    numberDisplay( m2,   0, datMColor );                                  // show fourth digit
  } else {
    numberDisplay( m1, 189, datMColor );                                  // show first digit
    numberDisplay( m2, 126, datMColor );                                  // show second digit
    numberDisplay( d1,  63, datDColor );                                  // show third digit
    numberDisplay( d2,   0, datDColor );                                  // show fourth digit
  }

  clockMinPassed = false;                                                 // reset the color change flag (1 minute)
  clockHourPassed = false;                                                // reset the color change flag (1 hour)
  clockDayPassed = false;                                                 // reset the color change flag (1 day)
}


//  ----- TEMPERATURE -----
void modeTem() {                                                          // display the temperature
  ledClock.clear();                                                       // clear the display

  // build the digits
  int t1 = floor( temKNXValue / 100 );                                    // build t1
  int t2 = ( temKNXValue / 10 ) % 10;                                     // build t2
  int t3 = temKNXValue % 10;                                              // build t3

  // set the colors
  if ( temColorSet == 0) {                                                // three defined colors: one for degrees, one for decimal and one for symbol
    temAColor = ledClock.Color(temAColorR, temAColorG, temAColorB);
    temBColor = ledClock.Color(temBColorR, temBColorG, temBColorB);
    temCColor = ledClock.Color(temCColorR, temCColorG, temCColorB);
  }
  if ( temColorSet == 1 && (                                              // three random colors: one for degrees, one for decimal and one for symbol
    ( gloColorFreq == 0 ) ||                                              // change color every second
    ( gloColorFreq == 1 && clockMinPassed ) ||                            // change color every minute and 1 minute passed
    ( gloColorFreq == 2 && clockHourPassed ) ||                           // change color every hour and 1 hour passed
    ( gloColorFreq == 3 && clockDayPassed ) ) ) {                         // change color every day and 1 day passed
    temAColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
    temBColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
    temCColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
  }

  // display the digits
  numberDisplay( t1, 189, temAColor );                                    // show first digit
  numberDisplay( t2, 126, temAColor );                                    // show second digit
  numberDisplay( t3,  63, temBColor );                                    // show third digit
  if ( temSymbol == 0 ) {
    numberDisplay( 15,   0, temCColor );                                  // show F
  } else {
    numberDisplay( 12,   0, temCColor );                                  // show C
  }

  clockMinPassed = false;                                                 // reset the color change flag (1 minute)
  clockHourPassed = false;                                                // reset the color change flag (1 hour)
  clockDayPassed = false;                                                 // reset the color change flag (1 day)
}


//  ----- HUMIDITY -----
void modeHum() {                                                          // display the humidity
  ledClock.clear();                                                       // clear the display

  // build the digits
  int h1 = floor( humKNXValue / 100 );                                    // build h1
  int h2 = ( humKNXValue / 10 ) % 10;                                     // build h2
  int h3 = humKNXValue % 10;                                              // build h3

  // set the colors
  if ( humColorSet == 0 ) {                                               // three defined colors: one for percents, one for decimal and one for symbol
    humAColor = ledClock.Color( humAColorR, humAColorG, humAColorB );
    humBColor = ledClock.Color( humBColorR, humBColorG, humBColorB );
    humCColor = ledClock.Color( humCColorR, humCColorG, humCColorB );
  }
  if ( humColorSet == 1 && (                                              // three random colors: one for percents, one for decimal and one for symbol
    ( gloColorFreq == 0 ) ||                                              // change color every second
    ( gloColorFreq == 1 && clockMinPassed ) ||                            // change color every minute and 1 minute passed
    ( gloColorFreq == 2 && clockHourPassed ) ||                           // change color every hour and 1 hour passed
    ( gloColorFreq == 3 && clockDayPassed ) ) ) {                         // change color every day and 1 day passed
    humAColor = ledClock.ColorHSV( random( 0, 65535 ), 255, 255 );
    humBColor = ledClock.ColorHSV(random( 0, 65535 ), 255, 255 );
    humCColor = ledClock.ColorHSV(random( 0, 65535 ), 255, 255 );
  }

  // display the digits
  numberDisplay( h1, 189, humAColor );                                    // show first digit
  numberDisplay( h2, 126, humAColor );                                    // show second digit
  numberDisplay( h3,  63, humBColor );                                    // show third digit
  numberDisplay( 17,   0, humCColor );                                    // show icon

  clockMinPassed = false;                                                 // reset the color change flag (1 minute)
  clockHourPassed = false;                                                // reset the color change flag (1 hour)
  clockDayPassed = false;                                                 // reset the color change flag (1 day)
}


//  ----- SCOREBOARD -----
void modeSco() {                                                          // display the scoreboard
  ledClock.clear();                                                       // clear the display

  // build the digits
  int s1 = floor( scoHValue / 10 );                                       // build s1
  int s2 = scoHValue % 10;                                                // build s2
  int s3 = floor( scoAValue / 10 );                                       // build s3
  int s4 = scoAValue % 10;                                                // build s4

  // set the colors
  scoHColor = ledClock.Color( scoHColorR, scoHColorG, scoHColorB );       // color for home team
  scoAColor = ledClock.Color( scoAColorR, scoAColorG, scoAColorB );       // color for away team

  // display the digits
  numberDisplay( s1, 189, scoHColor );                                    // show first digit
  numberDisplay( s2, 126, scoHColor );                                    // show second digit
  numberDisplay( s3,  63, scoAColor );                                    // show third digit
  numberDisplay( s4,   0, scoAColor );                                    // show fourth digit
}


//  ----- COUNTDOWN -----
void modeCou() {                                                          // display the countdown
  ledClock.clear();                                                       // clear the display

  if ( couMillis == 0 ) {                                                 // abort empty countdown
    if ( gloBrightness == 0 ) {                                           // value = 0 -> automatic mode
      ledClock.setBrightness( briClock );
    } else {
      ledClock.setBrightness( gloBrightness / 2 );
    }
    displayMode = 0;
    pref.putInt( "displayMode", 0 );
    return;
  }

  // build the digits
  uint32_t restMillis = couMillisEnd - millis();                          // continously build the remaining milliseconds and ...
  uint32_t hours   = ( ( restMillis / 1000 ) / 60 ) / 60;                 // ... create the countdown in hours
  uint32_t minutes = ( restMillis / 1000 ) / 60;                          // ... create the countdown in minutes
  uint32_t seconds = restMillis / 1000;                                   // ... create the countdown in seconds
  int remM = minutes - ( hours * 60 );                                    // build the minutes between 0 and 59
  int remS = seconds - ( minutes * 60 );                                  // build the seconds between 0 and 59

  int h1 = hours / 10;                                                    // build h1
  int h2 = hours % 10;                                                    // build h2
  int m1 = remM / 10;                                                     // build m1
  int m2 = remM % 10;                                                     // build m2
  int s1 = remS / 10;                                                     // build s1
  int s2 = remS % 10;                                                     // build s2

  // set the colors
  if ( restMillis > 10000 ) {                                             // three defined colors
    couHColor = ledClock.Color( couHColorR, couHColorG, couHColorB );
    couMColor = ledClock.Color( couMColorR, couMColorG, couMColorB );
    couSColor = ledClock.Color( couSColorR, couSColorG, couSColorB );
  } else {                                                                // change color for the last 10 seconds
    couMColor = ledClock.Color( couAColorR, couAColorG, couAColorB );
    couSColor = ledClock.Color( couAColorR, couAColorG, couAColorB );
  }

  // display the digits
  if ( hours > 0 ) {                                                      // if more than 60 minutes remaining on the countdown
    numberDisplay( h1, 189, couHColor);                                   // show hours and minutes
    numberDisplay( h2, 126, couHColor);
    numberDisplay( m1,  63, couMColor);
    numberDisplay( m2,   0, couMColor);  
  } else {                                                                // if less than 60 minutes remaining on the countdown
    numberDisplay( m1, 189, couMColor);                                   // show minutes and seconds
    numberDisplay( m2, 126, couMColor);
    numberDisplay( s1,  63, couSColor);
    numberDisplay( s2,   0, couSColor);  
  }

  // display "EndE"
  if ( hours <= 0 && remM <= 0 && remS <= 0 ) {
    countdownEnd();
    couMillis = 0;
    couMillisEnd = 0;
    return;
  } 
}


//  ----- SCROLLING -----
void modeScr() {                                                          // display different modes continously
  ledClock.clear();                                                       // clear the display

  scrMillis = millis();                                                   // update current reference time

  if ( ( scrMillis - scrMillisPrev ) >= ( scrTime * 1000 ) ) {            // run everything inside here every x seconds
    scrMillisPrev = scrMillis;                                            // update previous reference time

    if ( scrDisplayMode < 2 ) {                                           // if scrDisplayMode is smaller than 2 ...
      scrDisplayMode = scrDisplayMode + 1;                                // ... increase scrDisplayMode by 1
    } else {                                                              // ... else ...
      scrDisplayMode = 0;                                                 // ... restart with the first content
    }
  }

  if ( scrDisplayMode == 0 ) { modeClk(); }                               // call clock mode
  if ( scrDisplayMode == 1 ) { modeDat(); }                               // call date mode
  if ( scrDisplayMode == 2 ) { modeTem(); }                               // call temperature mode
}



//  ----------------------------------------------------------------------------------------------------
//  SETUP
//  ----------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);                                                   // initialize serial connection
  delay(1000);                                                            // wait a second for the serial to properly start
  initMessage();                                                          // display initial message with some basic information
  initFlash();                                                            // load all saved settings
  initWifi();                                                             // configure wifi
  initmDNS();                                                             // configure mdns
  initSPIFFS();                                                           // initialize spiffs
  initNTP();                                                              // get the time from ntp
  initHandlers();                                                         // load all web handlers
  initServer();                                                           // configure webserver
  initLED();                                                              // init neopixels

  getBrightness();
  getTemperature();
  getHumidity();

  xTaskCreatePinnedToCore(                                                // task 1 (running on core 0)
    taskCore0code,                                                        // function to implement the task
    "taskCore0",                                                          // name of the task
    10000,                                                                // stack size in words
    NULL,                                                                 // task input parameter
    0,                                                                    // priority of the task
    &taskCore0,                                                           // task handle
    0);                                                                   // core where the task should run

  randomSeed(analogRead(1));                                              // initializes the pseudo-random number generator
}



//  ----------------------------------------------------------------------------------------------------
//  LOOP
//  ----------------------------------------------------------------------------------------------------

void loop() {
  currMillisCore1 = millis();                                             // update current reference time

  if ( ( currMillisCore1 - prevTimeSecCore1 ) >= 1000 ) {                 // run everything inside here every second
    prevTimeSecCore1 = currMillisCore1;                                   // update previous reference time

    time( &now );                                                         // read the current time
    localtime_r( &now, &ntpTime );                                        // update ntpTime with the current time

    if ( displayMode == 0 ) { modeClk(); }                                // call clock mode
    if ( displayMode == 1 ) { modeDat(); }                                // call date mode
    if ( displayMode == 2 ) { modeTem(); }                                // call temperature mode
    if ( displayMode == 3 ) { modeHum(); }                                // call humidity mode
    if ( displayMode == 4 ) { modeSco(); }                                // call scoreboard mode
    if ( displayMode == 5 ) { modeCou(); }                                // call countdown mode
    if ( displayMode == 6 ) { modeScr(); }                                // call scroll mode
    
    ledClock.show();                                                      // show the content of the mode
  }
}
