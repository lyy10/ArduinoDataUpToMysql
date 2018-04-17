#include <SoftwareSerial.h>
#include <SerialESP8266wifi.h>
/*********************************/
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 8

/*********************************/
/* 
 *  2018-04-16 @Lyy
 */

#define sw_serial_rx_pin 6 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 4 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)

//#define SERVER_PORT "2121"
#define SSID "203_google_few flow no youtube"
#define PASSWORD "wsn12345678"
#define Host "192.168.1.160"
#define Port "80"
DHT dht(DHTPIN,DHTTYPE);
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);

// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin, Serial);

uint8_t wifi_started = false;
uint8_t wifi_connect_status = false;

void setup() {

  // start debug serial
  pinMode(sw_serial_rx_pin, INPUT);
  pinMode(sw_serial_tx_pin, OUTPUT);
  dht.begin();
  swSerial.begin(9600);
  // start HW serial for ESP8266 (change baud depending on firmware)
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Starting wifi-Lyy");


  wifi.setTransportToTCP();// this is also default
  wifi.endSendWithNewline(false); // Will end all transmissions with a newline and carrage return ie println.. default is true

  wifi_started = wifi.begin();
  if (wifi_started) {
    wifi.connectToAP(SSID, PASSWORD);
  } else {
    // ESP8266 isn't working..
    Serial.println(" wifi wrong");
  }
 /* wifi_connect_status = wifi.connectToServer("192.168.1.160","3306");
  if(wifi_connect_status)
    Serial.println("Connected mysql");
  else
    Serial.println("mysql wrong");*/
  
}

void loop() {

  static WifiConnection *connections;
  // check connections if the ESP8266 is there
  if (wifi_started)
    wifi.checkConnections(&connections);
    Serial.println("test connection");
  if(!wifi.isConnectedToServer()){
    Serial.println("no connect to a server");
    if(wifi.connectToServer(Host,Port))
      Serial.println("connected server");
    //else
    // return;
  }
  String x = "GET /index.php?Humidity=",y="&Temperature=";
  String hh ,tt;
  char h[6]={'\r'};
  char t[6]={'\r'};
  Serial.println(dht.readHumidity(true));
  Serial.println(dht.readTemperature());
  while (!Serial)
    ;
  dtostrf(dht.readHumidity(true),2,2,h);
  dtostrf(dht.readTemperature(),2,2,t);
  Serial.println(h);
  while (!Serial)
    ;
  Serial.println(t);
  while (!Serial)
    ;
  hh = h;
  tt = t;
  x += hh + y + tt;
  x += " HTTP/1.1\r\nHost: 192.168.1.160\r\nConnection: keep-alive\r\n\r\n";
  //strcat(sendx , h);
  Serial.println(x);
  Serial.println("start send to a server");
  wifi.send(SERVER,x);
  delay(1500);
  
}
//
//void processCommand(WifiMessage msg) {
//  // return buffer
//  char espBuf[MSG_BUFFER_MAX];
//  // scanf holders
//  int set;
//  char str[16];
//
//  // Get command and setting
//  sscanf(msg.message,"%15s %d",str,&set);
//  /* swSerial.print(str);*/
//  /* swSerial.println(set);*/
//
//  if ( !strcmp_P(str,IDN) ) {
//    wifi.send(msg.channel,"ESP8266wifi Example");
//  }
//  // Reset system by temp enable watchdog
//  else if ( !strcmp_P(str,RST) ) {
//    wifi.send(msg.channel,"SYSTEM RESET...");
//    // soft reset by reseting PC
//    asm volatile ("  jmp 0");
//  }
//  // Unknown command
//  else {
//    wifi.send(msg.channel,"ERR");
//  }
//}
