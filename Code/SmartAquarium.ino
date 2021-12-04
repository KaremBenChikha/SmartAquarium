/***************************************************
  SMART AQUARIUM BETA VERSION CODE 
 ****************************************************/
#include <String.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFiClientSecure.h>
#include <OneWire.h>

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "your hotspot"
#define WLAN_PASS       "your password"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "username"
#define AIO_KEY         "API KEY"

/************************* IFTT SMS Setup *********************************/

const char* host = "maker.ifttt.com";
const int httpsPort = 443;

// Make a POST or GET web request to
String temp_url = ;
String feed_url = ;

/************ Global State ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/

Adafruit_MQTT_Subscribe Food = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Food", MQTT_QOS_1);

Adafruit_MQTT_Subscribe RGB = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/RGB", MQTT_QOS_1);

Adafruit_MQTT_Subscribe PUMP = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Oxygen", MQTT_QOS_1);

Adafruit_MQTT_Subscribe Temperature = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Temperature", MQTT_QOS_1);

Adafruit_MQTT_Publish Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");

// System control : push current state
//Adafruit_MQTT_Publish State_push = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Notification");


/*************************** Sketch Code ************************************/

//DS18B20 temeprature sensor parameters
OneWire ds(2);
byte i;
byte present = 0;
byte type_s;
byte data[12];
byte addr[8];

// RGB parameters 
#define RED_PIN     14
#define GREEN_PIN   15
#define BLUE_PIN    13

// other system parameters
#define PUMP_PIN    12

// light sensor config 
const int sensorPin = 10;

// stepper config 
#define pin1 16
#define pin2 5
#define pin3 4
#define pin4 0

#define delaytime         5
#define numberOfSteps     520
#define MAX_FOOD_AMOUNT   520

int food_revolution;
String nightMode;

// timer parameters
int sec;
int minn;
int hour;

int timeZone = -4; // utc-4 eastern daylight time (nyc)

////////////////////////////////////////////// Temperature /////////////////////////////////

void temperatureCallback(double x) {
  // read temperature feed
  // Serial.print("Hey we re in a temperature callback, the current temp value is: ");
  // Serial.println(x);  
}


////////////////////////////////////////////// Oxygen pump setup /////////////////////////////////

void pumpCallback(char *data, uint16_t len) {
  // read oxygen feed
  //Serial.print("Hey we re in a ogygen callback, the current pump state is: ");
  //Serial.println(data);
  String state = (char *)data;
  if(state == "low") 
  {
    //Serial.println("low");
    digitalWrite(PUMP_PIN,HIGH);  
  }
  else 
  {
    //Serial.println("Height");
    digitalWrite(PUMP_PIN,LOW);
  }
}

/////////////////////////////////////////////// Timer //////////////////////////////////////

void timecallback(uint32_t current) {

  // adjust to local time zone
  current += (timeZone * 60 * 60);

  // calculate current time
  sec = current % 60;
  current /= 60;
  minn = current % 60;
  current /= 60;
  hour = current % 24;

  // print hour
  if(hour == 0 || hour == 12)
    Serial.print("12");
  if(hour < 12)
    Serial.print(hour);
  else
    Serial.print(hour - 12);

  // print mins
  Serial.print(":");
  if(minn < 10) Serial.print("0");
  Serial.print(minn);

  // print seconds
  Serial.print(":");
  if(sec < 10) Serial.print("0");
  Serial.print(sec);

  if(hour < 12)
    Serial.println(" am");
  else
    Serial.println(" pm");

}

////////////////////////////////////////// RGB control ///////////////////////////////////////

void rgbCallback(char *data, uint16_t len) {
  // Serial.print("Hey we're in a rgb callback, the color hex value is: ");
  // Serial.println(data);
  String colorHex = (char *)data;
          // Convert it to integer
          int number = (int) strtol( &colorHex[2], NULL, 16);

          // Split them up into r, g, b values
          int r = number >> 16;
          int g = number >> 8 & 0xFF;
          int b = number & 0xFF;
          // Serial.println(r);
          // Serial.println(g);
          // Serial.println(b);
          setColor(r,g,b);
}

////////////////////////////////////////// Feeding control ///////////////////////////////////

void foodCallback(double amount) {
  // Serial.print("Hey we're in a onoff callback, the button value is: ");
  // Serial.println(amount);
  int steps = (int) amount;
  feed(steps);
}

////////////////////////////////////////////// Stability /////////////////////////////////

void stateCallback(double x) {
  // read state feed
//  Serial.print("Hey we re in a state callback, the current state is: ");
//  Serial.println(x);  
}

void setup() {
//  Serial.begin(115200);
//  delay(100);
//
//  Serial.println(F("Adafruit MQTT demo"));
//
//  // Connect to WiFi access point.
//  
//  Serial.println(); Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(WLAN_SSID);
//
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
 //   Serial.print(".");
  }
//  Serial.println();
//
//  Serial.println("WiFi connected");
//  Serial.println("IP address: "); Serial.println(WiFi.localIP());

// init food revolution
  food_revolution = 0;

// initialize light sensor
  pinMode(sensorPin, INPUT);

// pin setup
  pinMode(pin1,OUTPUT);
  pinMode(pin2,OUTPUT);
  pinMode(pin3,OUTPUT);
  pinMode(pin4,OUTPUT);


//  pinMode(REGULATION_PIN,OUTPUT);
//  pinMode(EVACUATION_PIN,OUTPUT);

 //  Serial.println("wselt 8adi");
  
  pinMode(RED_PIN,OUTPUT);
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(BLUE_PIN,OUTPUT);

  pinMode(PUMP_PIN,OUTPUT);
  
// set callbacks 
  Food.setCallback(foodCallback);
  RGB.setCallback(rgbCallback);
  Temperature.setCallback(temperatureCallback);
  PUMP.setCallback(pumpCallback);
//  State_sub.setCallback(stateCallback);
  
  // Setup MQTT subscription for time feed.
  mqtt.subscribe(&Food);
  mqtt.subscribe(&RGB);
  mqtt.subscribe(&Temperature);
  mqtt.subscribe(&PUMP);

  // set PUMP default mode 
  digitalWrite(PUMP_PIN,LOW);  
  
  // connect to Adafruit MQTT
  MQTT_connect();

}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  
  if (Temp.publish(getTemperature()))
  {
  //  Serial.print("temperature : ");
  //  Serial.println(getTemperature());
    if(getTemperature()< 7 || getTemperature()> 27)
    {
      sendSMS("temp");
    }
  }



  int level = analogRead(sensorPin);
   Serial.printf("Light level: %d\n", level);

  if(level == 1023) 
  {
    setColor(255,255,255);
    nightMode = "on";
  }
  else if(nightMode == "on")
  {
    setColor(0,255,0); // by default set to blue color 
    nightMode = "off";
  }
   
   //delay(5000);
        
  // this is our 'wait for incoming subscription packets and callback em' busy subloop
  // try to spend your time here:
  mqtt.processPackets(5000);
  
  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds

  
//  if(! mqtt.ping()) {
//    mqtt.disconnect();
//  }
}

////////////////////////////// MQTT Connect ///////////////////////
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) 
  {
    return;
  }

 // Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
      // Serial.println(mqtt.connectErrorString(ret));
      // Serial.println("Retrying MQTT connection in 10 seconds...");
       mqtt.disconnect();
       delay(10000);  // wait 10 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  // Serial.println("MQTT Connected!");
  // setColor(255,255,255);
}

////////////////////////////// Temperature Sensor ///////////

float getTemperature()
{
  
  if ( !ds.search(addr))
  {
    ds.reset_search();
    delay(200);
  }
  if (OneWire::crc8(addr, 7) != addr[7])
  {
//    Serial.println("CRC is not valid!");
  }
  switch (addr[0])
  {
    case 0x10:
      type_s = 1;
      break;
    case 0x28:
      type_s = 0;
      break;
    case 0x22:
      type_s = 0;
      break;
    default: ;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion, with parasite power on at the end
  delay(100);
  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  for ( i = 0; i < 9; i++)
    data[i] = ds.read();
  // Convert the data to actual temperature
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s)
  {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10)
      raw = (raw & 0xFFF0) + 12 - data[6];
  }
  else
  {
    byte cfg = (data[4] & 0x60);
    if (cfg == 0x00) raw = raw & ~7; // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
  }
  return ((float)raw / 16.0);
}


void stepA()
{
  digitalWrite(pin1,1);
  digitalWrite(pin2,0);
  digitalWrite(pin3,0);
  digitalWrite(pin4,0);
}
void stepB()
{
  digitalWrite(pin1,0);
  digitalWrite(pin2,1);
  digitalWrite(pin3,0);
  digitalWrite(pin4,0);
}
void stepC()
{
  digitalWrite(pin1,0);
  digitalWrite(pin2,0);
  digitalWrite(pin3,1);
  digitalWrite(pin4,0);
}
void stepD()
{
  digitalWrite(pin1,0);
  digitalWrite(pin2,0);
  digitalWrite(pin3,0);
  digitalWrite(pin4,1);
}

void forward()
{
  stepA();
  delay(delaytime);
  stepB();
  delay(delaytime);
  stepC();
  delay(delaytime);
  stepD();
  delay(delaytime);
}

void backward()
{
  stepD();
  delay(delaytime);
  stepC();
  delay(delaytime);
  stepB();
  delay(delaytime);
  stepA();
  delay(delaytime);
}

void stepOff()
{
  digitalWrite(pin1,0);
  digitalWrite(pin2,0);
  digitalWrite(pin3,0);
  digitalWrite(pin4,0);
}

/////////////////////////// Feeding Mecanism ////////////////

void feed(int steps)
{
  int s = numberOfSteps;
  switch(steps) 
  {
    case 20:
      s = 104;
      food_revolution += 20;
      break;
      
    case 40:
      s = 208;
      food_revolution += 40;
      break;

    case 60:
      s = 312;
      food_revolution += 60;
      break;

    case 80:
      s = 416;
      food_revolution += 80;
      break;

    case 100:
      s = 520;
      food_revolution += 100;
      break;
  }
  
  stepOff();
  while(s>0)
  {
    forward();
    s--;
  }
  
  if(food_revolution > MAX_FOOD_AMOUNT)
  {
    sendSMS("food");  
  }
  
//  Serial.println("5raj me stepper");
}

///////////////////////////// RGB controller /////////////////

void setColor(int red, int green, int blue) 
{
 // invert RGB values for common anode LEDs
  analogWrite(RED_PIN, red); 
  analogWrite(GREEN_PIN, green); 
  analogWrite(BLUE_PIN, blue); 
}

////////////////////////////// SMS Sender /////////////////////

void sendSMS(String issue) 
{

  WiFiClientSecure client;
//  Serial.print("connecting to ");
//  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
//    Serial.println("connection failed");
    return;
  }
 
//  Serial.print("requesting URL: ");
  if(issue == "temp") 
  {
//    Serial.println(temp_url);
    client.print(String("GET ") + temp_url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");
//    Serial.println("request sent");
  }
  
  if(issue == "food")
  {
//    Serial.println(feed_url);
    client.print(String("GET ") + feed_url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: BuildFailureDetectorESP8266\r\n" +
                 "Connection: close\r\n\r\n");
//    Serial.println("request sent");
    
  }   

  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      // Serial.println("headers received");
      break;
    }
  }
  
  String line = client.readStringUntil('\n');
//  Serial.println("reply was:");
//  Serial.println("==========");
//  Serial.println(line);
//  Serial.println("==========");
//  Serial.println("closing connection");
     
}

