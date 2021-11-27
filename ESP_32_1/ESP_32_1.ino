#define BLYNK_TEMPLATE_ID "TMPLSczi3Omv"
#define BLYNK_DEVICE_NAME "Irrigator"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
//#include <BlynkSimpleEsp32.h>
#include <dht.h>
//#include "DHT.h"
#include "BlynkEdgent.h"

#define APP_DEBUG
#define DHTPIN 21
#define DHTTYPE DHT11   // DHT 11

const int waterLevelSensor = 2;
const int moistureLevelSensor = 34;
const int RELAY = 23;

int waterLevel;
int moistureLevel;

dht DHT;
BlynkTimer timer;




void sendSensor1()
{ DHT.read11(DHTPIN);
  float h = DHT.humidity;
  float t = DHT.temperature; // or dht.readTemperature(true) for Fahrenheit
    
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V0, t);

}

void sendSensor2()
{
  //waterLevel = analogRead(waterLevelSensor);
  //waterLevel = map(waterLevel,0,2000,0,20);
  moistureLevel = analogRead(moistureLevelSensor);
  moistureLevel = map(moistureLevel,0,4000,0,4000);
  Serial.println(moistureLevel);
  

  //Blynk.virtualWrite(V7, waterLevel);
  Blynk.virtualWrite(V2, moistureLevel);
}


BLYNK_WRITE(V3){
  if(param.asInt()==1){
    digitalWrite(RELAY, HIGH);
  }
  else{
    digitalWrite(RELAY, LOW);
  }
}
BLYNK_CONNECTED(){
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(DHTPIN, INPUT);
  //pinMode(moistureLevelSensor, INPUT);
  pinMode(RELAY, OUTPUT);
  BlynkEdgent.begin();
  //dht.begin();

    // Setup a function to be called every second
  timer.setInterval(2000L, sendSensor1);
  timer.setInterval(5000L, sendSensor2);

}


void loop() {
  // put your main code here, to run repeatedly:
  BlynkEdgent.run();
  timer.run();  
 
}
