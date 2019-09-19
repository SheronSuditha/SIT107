int ledPin = 13;               
int PIR_SENSOR1 = 2;
int PIR_SENSOR2 = 3;                          
int sen1_reading = 0;   
int sen2_reading = 0;                 
 
void setup() {
  pinMode(ledPin, OUTPUT);      //LEDPIN light was used instead of an Alarm
  pinMode(PIR_SENSOR1, INPUT); 
  pinMode(PIR_SENSOR2, INPUT);    
  Serial.begin(9600);
}

void loop() {
  sen_1_reading = digitalRead(PIR_SENSOR1);
  sen_2_reading = digitalRead(PIR_SENSOR2);

  if(sen1_reading == HIGH && sen2_reading == HIGH) { // checks if both sensors detect movement.
    digitalWrite(ledPin, HIGH); //if alarm used, the alarm will be turned on. 
    Serial.println("MOTION DETECTED!!");
  } else {
    digitalWrite(ledPin, LOW);
    serial.println("NO MOTION");
  }
  delay(2000); //2 second delay
}
