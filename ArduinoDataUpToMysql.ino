#include <SoftwareSerial.h>
#include <Wire.h> //I2C pin libraries
/*******************************/
#include <SerialESP8266wifi.h>
/*********************************/
#include <DHT.h>
#define DHTTYPE DHT11
/*********************************/
#include <Adafruit_BMP085.h>
/*********************************/
#include <BH1750FVI.h>
/**********************************/
/* 
 *  2018-04-16 @Lyy
 */
#define DHTPIN 8 //DHT Data pin
#define sw_serial_rx_pin 6 //  Connect this pin to TX on the esp8266
#define sw_serial_tx_pin 4 //  Connect this pin to RX on the esp8266
#define esp8266_reset_pin 5 // Connect this pin to CH_PD on the esp8266, not reset. (let reset be unconnected)
// pin 7 is the light sensor enable

#define SSID "203_google_few flow no youtube"
#define PASSWORD "wsn12345678"
#define Host "192.168.1.160"
#define SERVER_PORT "80"
DHT dht(DHTPIN,DHTTYPE);  //DHT class
SoftwareSerial swSerial(sw_serial_rx_pin, sw_serial_tx_pin);  //wifi pin 
Adafruit_BMP085 bmp;  //BMP180 class
BH1750FVI LightSensor;//Light sensor class
// the last parameter sets the local echo option for the ESP8266 module..
SerialESP8266wifi wifi(swSerial, swSerial, esp8266_reset_pin, Serial);//wifi

uint8_t wifi_started = false;
//uint8_t wifi_connect_status = false;

void setup() {
  Wire.begin(); //actention the begin in libraties shulde be igno.
  LightSensor.begin();
  bmp.begin();
  dht.begin();
  Serial.begin(115200);  
  LightSensor.SetAddress(Device_Address_H);
  LightSensor.SetMode(Continuous_H_resolution_Mode);
  // start debug serial
  pinMode(sw_serial_rx_pin, INPUT);
  pinMode(sw_serial_tx_pin, OUTPUT);
  
  swSerial.begin(9600);
  // start HW serial for ESP8266 (change baud depending on firmware)
  
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
  
}

void loop() {
 // view();
  uint16_t lux = LightSensor.GetLightIntensity();// Get Lux value
  static WifiConnection *connections;
  String x = "GET /index.php?Humidity=",y="&Temperature=";
  String hh ,tt;
  char h[6]={'\r'};
  char t[6]={'\r'};
  // check connections if the ESP8266 is there
  if (wifi_started)
    wifi.checkConnections(&connections);
    Serial.println("test connection");
  if(!wifi.isConnectedToServer()){
    Serial.println("no connect to a server");
    if(wifi.connectToServer(Host,SERVER_PORT))
      Serial.println("connected server");
    else
     return;
  }
  
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
void view()
{
    uint16_t lux = LightSensor.GetLightIntensity();// Get Lux value
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lux");
  while (!Serial)
    ;
  Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(bmp.readSealevelPressure());
    Serial.println(" Pa");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
   while (!Serial)
    ;
   Serial.println(dht.readHumidity(true));
   Serial.println(dht.readTemperature());
}
