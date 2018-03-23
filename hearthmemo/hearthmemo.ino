// Coding By IOXhop : www.ioxhop.com
// This version 0.2
#include <OneWire.h>
#include <DallasTemperature.h>



#include "BLE.h"
#include <ArduinoJson.h>
#define ONE_WIRE_BUS 33
#define UpperThreshold 2000
#define LowerThreshold 1850
int PulseSensorPurplePin = 32;        // Pulse Sensor PURPLE WIRE connected to ANALOG PIN 0
int Signal;     
int oxygen ;// holds the incoming raw data. Signal value can range from 0-1024
int LastTime=0;
bool BPMTiming=false;
bool BeatComplete=false;
int BPM=0;
int temp =0;
int data =0; 
BLE ble("BLE Basic");
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
void setup() {
  Serial.begin(115200);
  sensors.begin();
  ble.begin();
  BLEService *service = new BLEService(0xFF);
  service->addCharacteristic(0xFF01);
  service->addCharacteristic(0xFF02);
  service->addCharacteristic(0xFF03);
  ble.addService(service);
}

void loop() {
sensors.requestTemperatures();
  
temp =sensors.getTempCByIndex(0);

 //Serial.println(temp);
   Signal = analogRead(PulseSensorPurplePin);
    if(Signal>UpperThreshold)
  {
    if(BeatComplete)
    {
      BPM=millis()-LastTime;
      BPM=int(60/(float(BPM)/1000));
      BPMTiming=false;
      BeatComplete=false;
    }
    if(BPMTiming==false)
    {
      LastTime=millis();
      BPMTiming=true;
    }
   
  }
  if((Signal<LowerThreshold)&(BPMTiming))
    BeatComplete=true;
     
//Serial.println(BPM);


   temp = abs(temp);
   //data = oxygen+(BPM*1000)+(temp*1000000)+1000000000;
   
   //Serial.println(data);
    
  ble.on(READ, [](int service_uuid, int char_uuid) {
    Serial.println("Event: READ");
    Serial.print("Service UUID: 0x");
    Serial.println(service_uuid, HEX);
    Serial.print("Characteristic UUID: 0x");
    Serial.println(char_uuid, HEX);
    Serial.println(char_uuid);
    if(char_uuid == 65281 ){
       ble.reply(String(temp));
      }
    if(char_uuid == 65282){
        ble.reply(String(BPM));
      }
    if(char_uuid == 65283){
        ble.reply(String(oxygen));
      }
   
  
  });
}

