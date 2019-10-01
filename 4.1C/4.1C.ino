#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
RTC_DS1307 rtc;
const int chipSelect = 4;

#define sensor A0
#define LED 13
float reading = 0;
String ledlock;

void setup() {
  Serial.print("Initializing SD card...");
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  for(int i = 0; i <= 100; i++) {
    reading = reading + analogRead(sensor);
    delay(1);
  }
  reading = reading/100.0;
  formatSoilMoistureReading(reading, checkIfHighOrLow(reading), CHECK_FOR_WARNINGS(reading));

}

String checkIfHighOrLow(int reading) {
   if(reading <= 50){
    return "EX_LOW";
  } else if(reading >= 300) {
    return "EX_HIGH";
  } else {
    return "NORMAL";
  }
    
}

void formatSoilMoistureReading(float data, String result, String warning) {
  DateTime now = rtc.now();
  String datastring = "";
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
   if (dataFile) {
    datastring += now.unixtime() + String(data) + "," + result;
    dataFile.println();
    dataFile.close();
    LED_CONTROLLER(result);
    Serial.print("Soil Moisture reading : ");
    Serial.print(data);
    Serial.print("   Current status   " + result + warning);
    Serial.println();
    delay(500);
    Serial.println(dataString);
  } else {
    Serial.println("error opening datalog.txt");
  }

  
}

String CHECK_FOR_WARNINGS(float data) {
  if(data >= 50) {
    return " : HIGH MOISTURE. Please check.... ";
  } if(data <= 50) {
    return " : EXTREMELY LOW MOISTURE. Please check....";
  } else {
    return " : CONSIDERABLE....";
  }
}

void LED_CONTROLLER(String result) {
  if(result == "EX_LOW") {
     ledlock = "LOCK";
      if(ledlock == "LOCK") {
         digitalWrite(LED_BUILTIN, HIGH);  
         checkLedLock(result);
      }
  } else if(result == "EX_HIGH") {
    ledlock = "LOCK";
    if(ledlock == "LOCK") {
         digitalWrite(LED_BUILTIN, HIGH);  
         checkLedLock(result);
    }
  }else {
    digitalWrite(LED_BUILTIN, LOW);  
  }
}

void checkLedLock(String result) {
  if(result == "EX_LOW") {
    EX_LOW();
    //ledlock = "LOCK";
  } else if(result == "EX_HIGH") {
    EX_HIGH();
  } else {
    ledlock = "HIGH";
  }
  
}

void EX_LOW() {
  for(int c=0;c<=10;c++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(60);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void EX_HIGH() {
  for(int c=0;c<=10;c++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(60);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(60);
    digitalWrite(LED_BUILTIN, LOW);
    delay(60);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(60);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
