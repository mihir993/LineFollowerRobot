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
float coeff = 1.0;
float turncoeff = 1.0;
int dlay = 0;
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
  setPwmFrequency(3, 32);
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
if (distance > 10){ // No objects till 9cm
  if ((sensorValueLft < 50) && (sensorValueMid < 50) && (sensorValueRgt < 50)){  //000  --> Search for black
    if (Flag == "Left"){
      Stop();
      delay(100);
      Forward(Right, 255*coeff*0.5);
      Backward(Left, 255*coeff*0.5);
      delay(8);
      }
    else if (Flag == "Right"){
      Stop();
      delay(100);
      Forward(Left, 255*coeff*0.5);
      Backward(Right, 255*coeff*0.5);
      delay(8);
      }
    }
  else if ((sensorValueLft < 50) && (sensorValueMid < 50) && (sensorValueRgt > 90)){  //001 --> Fast turn Right
    //Stop();
    //Forward(Left, 255);
    Stop();
//    Turn(Right);
    Forward(Left, 255*coeff);
    Forward(Right, 255*coeff*0.25);
    Flag = "Right";
    delayMicroseconds(dlay);
    }
  else if ((sensorValueLft < 50) && (sensorValueMid > 90) && (sensorValueRgt < 50)){  //010 --> Fast Forward
    if(sensorValueMid > 180){
      Stop();
      Forward(Left, 255*coeff);
      Forward(Right, 255*coeff);
      delayMicroseconds(dlay);
      }
    else{
      Stop();
      Forward(Left, 255*coeff);
      Forward(Right, 255*coeff);
      delayMicroseconds(dlay);
      }
    }
  else if ((sensorValueLft < 50) && (sensorValueMid > 90) && (sensorValueRgt > 90)){  //011 --> Turn Right
    Stop();
    Turn(Right);
    Flag = "Right";
    delayMicroseconds(dlay);
    
    }
  else if ((sensorValueLft > 90) && (sensorValueMid < 50) && (sensorValueRgt < 50)){  //100 --> Fast turn Left
//    Stop();
//    Forward(Right, 255);
    Stop();
    //Turn(Left);
    Forward(Left, 255*coeff*0.25);
    Forward(Right, 255*coeff);
    Flag = "Left";
    delayMicroseconds(dlay);
    }
  else if ((sensorValueLft > 90) && (sensorValueMid > 90) && (sensorValueRgt < 50)){  //110 --> Turn Left
    Stop();
    Turn(Left);
    Flag = "Left";
    delayMicroseconds(dlay);
    }
  else if ((sensorValueLft > 90) && (sensorValueMid > 90) && (sensorValueRgt > 90)){  //111 --> Forward
    Stop();
    Forward(Left, 255*coeff);
    Forward(Right, 255*coeff);
    delayMicroseconds(dlay);
    }
  else{  //101 --> Not defined
    }
  }
//else if (distance > 5){ //object between 9 and 5cm
//    if ((sensorValueLft < 50) && (sensorValueMid < 50) && (sensorValueRgt < 50)){  //000  --> Search for black
//    if (Flag == "Left"){
//      Stop();
//      delay(200);
//      Forward(Right, 200);
//      Backward(Left, 200);
//      delay(10);
//      }
//    else if (Flag == "Right"){
//      Stop();
//      delay(200);
//      Forward(Left, 200);
//      Backward(Right, 200);
//      delay(10);
//      }
//    }
//  else if ((sensorValueLft < 50) && (sensorValueMid < 50) && (sensorValueRgt > 90)){  //001 --> Fast turn Right
//    Stop();
//    Forward(Left, 255/2);
//    Flag = "Right";
//    }
//  else if ((sensorValueLft < 50) && (sensorValueMid > 90) && (sensorValueRgt < 50)){  //010 --> Fast Forward
//    if(sensorValueMid > 180){
//      Forward(Left, 255/2);
//      Forward(Right, 255/2);
//      }
//    else{
//      Forward(Left, 190/2);
//      Forward(Right, 190/2);
//      }
//    }
//  else if ((sensorValueLft < 50) && (sensorValueMid > 90) && (sensorValueRgt > 90)){  //011 --> Turn Right
//    Turn(Right);
//    Flag = "Right";
//    }
//  else if ((sensorValueLft > 90) && (sensorValueMid < 50) && (sensorValueRgt < 50)){  //100 --> Fast turn Left
//    Stop();
//    Forward(Right, 255/2);
//    Flag = "Left";
//    }
//  else if ((sensorValueLft > 90) && (sensorValueMid > 90) && (sensorValueRgt < 50)){  //110 --> Turn Left
//    Turn(Left);
//    Flag = "Left";
//    }
//  else if ((sensorValueLft > 90) && (sensorValueMid > 90) && (sensorValueRgt > 90)){  //111 --> Forward
//    Forward(Left, 255/2);
//    Forward(Right, 255/2);
//    }
//  else{  //101 --> Not defined
//    }
//  }
else{ // detected object at less than 5cm
  Stop();
  delay(50);
  Forward(Right, 200);
  Backward(Left, 200);
  Flag = "Left";
  delay(10);
  }
}
void Forward(String side, int spd){ 
if (side == "Left"){
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgLeft, LOW);
  analogWrite(pwmLeft, spd);  
  }  
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(brgRight, HIGH);
  analogWrite(pwmRight, 255-spd);
  }
}
void Backward(String side, int spd){ 
if (side == "Left"){
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgLeft, HIGH);
  analogWrite(pwmLeft, 255-spd);
  }  
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(brgRight, LOW);
  analogWrite(pwmRight, spd);
  }
}
void Turn(String side){
if (side == "Left"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgRight, HIGH);
  digitalWrite(brgLeft, LOW);
  analogWrite(pwmRight,255-255*turncoeff);
  analogWrite(pwmLeft,255*turncoeff/2);
  }
else if (side == "Right"){
  digitalWrite(powerRight, HIGH);
  digitalWrite(powerLeft, HIGH);
  digitalWrite(brgRight, HIGH);
  digitalWrite(brgLeft, LOW);
  analogWrite(pwmLeft,255*turncoeff);
  analogWrite(pwmRight,255-(255*turncoeff/2));
  }
}
void Stop(){
digitalWrite(powerRight, LOW);
digitalWrite(powerLeft, LOW);
}

void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
