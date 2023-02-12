// Declairations for motors
#define powerRight 9
#define powerLeft 6
#define brgRight 4
#define pwmRight 5
#define brgLeft 2
#define pwmLeft 3
//Declairations for Octocouplers
const int sensorRgt = A0;  // Analog input pin that the potentiometer is attached to
const int sensorMid = A1;
const int sensorLft = A3;
int sensorValueRgt = 0;// value read from the pot
int sensorValueMid = 0;
int sensorValueLft = 0;
//Declairations for Ulrasonic
const int trigPin = 8;
const int echoPin = 7;
long traveltime;
int distance;
// Declairations for functions
String Left = "Left";
String Right = "Right";
String Flag = "Left";
//Declairatios for LCD
#include<Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <LCD.h>
LiquidCrystal_I2C  lcd(0x3F,2,1,0,4,5,6,7,3,POSITIVE);
void setup() {
  // Setups for motors
  pinMode(powerRight, OUTPUT);
  pinMode(powerLeft, OUTPUT);
  pinMode(brgRight, OUTPUT);
  pinMode(brgLeft, OUTPUT);
  pinMode(pwmRight, OUTPUT);
  pinMode(pwmLeft, OUTPUT);
  digitalWrite(powerRight, LOW);
  digitalWrite(powerLeft, LOW);
  digitalWrite(brgRight, LOW);
  digitalWrite(brgLeft, LOW);
  digitalWrite(pwmRight, LOW);
  digitalWrite(pwmLeft, LOW);
  // Setups for Octocouplers
  pinMode(sensorRgt, INPUT_PULLUP);
  pinMode(sensorMid, INPUT_PULLUP);
  pinMode(sensorLft, INPUT_PULLUP);
  // Setups for Ultrasonics
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Setups for LCD
    lcd.begin(16,2);
  // Serial communications
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
lcd.clear();
sensorValueRgt = analogRead(sensorRgt);
sensorValueMid = analogRead(sensorMid);
sensorValueLft = analogRead(sensorLft);
sensorValueRgt = map(sensorValueRgt, 0, 1023, 0, 255);
sensorValueMid = map(sensorValueMid, 0, 1023, 0, 255);
sensorValueLft = map(sensorValueLft, 0, 1023, 0, 255);
Serial.print(sensorValueLft);
Serial.print("\t");
lcd.print(sensorValueLft);
lcd.print("   ");
Serial.print(sensorValueMid);
Serial.print("\t");
lcd.print(sensorValueMid);
lcd.print("   ");
Serial.print(sensorValueRgt);
Serial.print("\n");
lcd.print(sensorValueRgt);
lcd.print("   ");

digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
traveltime = pulseIn(echoPin, HIGH);
distance = traveltime * 0.034/2;
lcd.setCursor(0,1);
lcd.print("dis=");
lcd.print(distance);
if ((sensorValueMid > 90) && distance > 5){
  if ((sensorValueLft <50) && (sensorValueRgt < 50)){
    Forward(Left);
    Forward(Right);
    }
  else if ((sensorValueLft > 90)&& (sensorValueRgt < 50)){
    Turn(Left);
    Flag =  "Left";
    }
  else if ((sensorValueLft <50)&& (sensorValueRgt > 90)){
    Turn(Right);
    Flag = "Right";
    }
  else if ((sensorValueLft > 90) && (sensorValueRgt >90)){
    Forward(Left);
    Forward(Right);
    }
  else{
    Forward(Right);
    Backward(Left);
    }
  }
else if ((sensorValueLft > 90)&& (sensorValueRgt < 50) && distance > 5){
    Turn(Left);
    Flag = "Left";
    }
else if ((sensorValueLft <50)&& (sensorValueRgt > 90) && distance > 5){
    Turn(Right);
    Flag = "Right";
    }
else{
  if (Flag == "Left"){
    Stop();
    delay(200);
    Forward(Right);
    Backward(Left);
    delay(10);
  }
  else if (Flag == "Right"){
    Stop();
    delay(200);
    Forward(Left);
    Backward(Right);
    delay(10);
    
    }
}
/*Forward(Left);
Forward(Right);
delay(500);
Backward(Left);
Backward(Right);
delay(500);
Turn(Left);
delay(800);
Turn(Right);
delay(800);
Stop();
delay(500);*/
}
void Forward(String side){ 
if (side == "Left"){
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgLeft, LOW);
  digitalWrite(pwmLeft, HIGH);  
  }  
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(brgRight, HIGH);
  digitalWrite(pwmRight, LOW);
  }
}
void Backward(String side){ 
if (side == "Left"){
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgLeft, HIGH);
  digitalWrite(pwmLeft, LOW);
  }  
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(brgRight, LOW);
  digitalWrite(pwmRight, HIGH);
  }
}
void Turn(String side){
if (side == "Left"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgRight, HIGH);
  digitalWrite(brgLeft, LOW);
  analogWrite(pwmRight,0);
  analogWrite(pwmLeft,127);
  }
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgRight, HIGH);
  digitalWrite(brgLeft, LOW);
  analogWrite(pwmLeft,255);
  analogWrite(pwmRight,127);
  }
}
void Stop(){
digitalWrite(powerRight, LOW);
digitalWrite(powerLeft, LOW);
}
