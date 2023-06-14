
#include <SoftwareSerial.h>
#include <util/atomic.h>
#include <Wire.h>

//Mega
/*
 only the following can be used for RX: 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69)
 */
const int MPU = 0x68; // MPU6050 I2C address
float GyroX;
float gyroAngleX; //used in void loop()
float AccErrorX, GyroErrorX;
float elapsedTime, currentTime, previousTime;
int c = 0;
int temp = 0;
int  SpeedTurn = 190;
float previousAngleX = 0;

#define dirPin 6
#define stepPin 7

#define ENLF  2 // LF = Left Front = motor số 2, mạch cầu A
#define ENRF  3 // RF = Right Front = motor số 1, mạch cầu A
#define ENLB  4 // LB = Left Back = motor số 4, mạch cầu B
#define ENRB  5 // RB = Right Back = motor số 3, mạch cầu B
    
#define LF1  22 //IN3A, LF = Left Front = motor số 2, mạch cầu A
#define LF2  23 //IN4A, LF = Left Front = motor số 2, mạch cầu A
#define RF1  24 //IN1A, RF = Right Front = motor số 1, mạch cầu A
#define RF2  25 //IN2A, RF = Right Front = motor số 1, mạch cầu A
    
#define LB1  26 //IN3B, LB = Left Back = motor số 4, mạch cầu B
#define LB2  27 //IN4B, LB = Left Back = motor số 4, mạch cầu B
#define RB1  28 //IN1B, RB = Right Back = motor số 3, mạch cầu B
#define RB2  29 //IN2B, RB = Right Back = motor số 3, mạch cầu B
int command;
int Speed=130;
int LSpeed = Speed;
int RSpeed = Speed;
SoftwareSerial mySerial(50,51); //(TX,RX)

void setup() {
  Serial.begin(9600);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  // Call this function if you need to get the IMU error values for your module
  calculateError();
  delay(20);
  pinMode(LF1,OUTPUT);
  pinMode(LF2,OUTPUT);
  pinMode(RF1,OUTPUT);
  pinMode(RF2,OUTPUT);
  
  pinMode(ENLF,OUTPUT);
  pinMode(ENRF,OUTPUT);
  
  pinMode(LB1,OUTPUT);
  pinMode(LB2,OUTPUT);
  pinMode(RB1,OUTPUT);
  pinMode(RB2,OUTPUT);

  pinMode(ENLB,OUTPUT);
  pinMode(ENRB,OUTPUT);

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  mySerial.begin(9600);
}


void Forward(){
 if(gyroAngleX <= -0.5){
  RSpeed = Speed/2;
}
else if(gyroAngleX >= 0.5){
  LSpeed = Speed/2;
}
else{
  RSpeed = Speed;
  LSpeed = Speed;
}
previousAngleX = gyroAngleX;
calculateGyroAngleX(); 
  digitalWrite(LF1,LOW);
  digitalWrite(LF2,HIGH);
  analogWrite(ENLF,LSpeed);  
  digitalWrite(LB1,LOW);
  digitalWrite(LB2,HIGH);
  analogWrite(ENLB,LSpeed);
  digitalWrite(RF1,LOW);
  digitalWrite(RF2,HIGH);
  analogWrite(ENRF,RSpeed);
  digitalWrite(RB1,LOW);
  digitalWrite(RB2,HIGH);
  analogWrite(ENRB,RSpeed);
  Serial.println(gyroAngleX);
}

void Backward(){

  if(gyroAngleX <= -0.5){
  RSpeed = Speed/1.5;
}
else if(gyroAngleX >= 0.5){
  LSpeed = Speed/1.5;
}
else{
  RSpeed = Speed;
  LSpeed = Speed;
}
calculateGyroAngleX();  
  digitalWrite(LF1,HIGH);
  digitalWrite(LF2,LOW);
  analogWrite(ENLF,LSpeed);  
  digitalWrite(LB1,HIGH);
  digitalWrite(LB2,LOW);
  analogWrite(ENLB,LSpeed);
  digitalWrite(RF1,HIGH);
  digitalWrite(RF2,LOW);
  analogWrite(ENRF,RSpeed);
  digitalWrite(RB1,HIGH);
  digitalWrite(RB2,LOW);
  analogWrite(ENRB,RSpeed);
}
void Right() {
  digitalWrite(LF1,HIGH);
  digitalWrite(LF2,LOW);
  analogWrite(ENLF,SpeedTurn);
  digitalWrite(RF1,LOW);
  digitalWrite(RF2,HIGH);
  analogWrite(ENRF,SpeedTurn);
  digitalWrite(LB1,HIGH);
  digitalWrite(LB2,LOW);
  analogWrite(ENLB,SpeedTurn);
  digitalWrite(RB1,LOW);
  digitalWrite(RB2,HIGH);
  analogWrite(ENRB,SpeedTurn);
}
void Left() {
  digitalWrite(LF1,LOW);
  digitalWrite(LF2,HIGH);
  analogWrite(ENLF,SpeedTurn);
  digitalWrite(RF1,HIGH);
  digitalWrite(RF2,LOW);
  analogWrite(ENRF,SpeedTurn);
  digitalWrite(LB1,LOW);
  digitalWrite(LB2,HIGH);
  analogWrite(ENLB,SpeedTurn);
  digitalWrite(RB1,HIGH);
  digitalWrite(RB2,LOW);
  analogWrite(ENRB,SpeedTurn);
}
void forwardleft() {
  digitalWrite(LF1,LOW);
  digitalWrite(LF2,HIGH);
  analogWrite(ENLF,Speed);  
  digitalWrite(LB1,LOW);
  digitalWrite(LB2,HIGH);
  analogWrite(ENLB,Speed);
  digitalWrite(RF1,LOW);
  digitalWrite(RF2,HIGH);
  analogWrite(ENRF,Speed/3);
  digitalWrite(RB1,LOW);
  digitalWrite(RB2,HIGH);
  analogWrite(ENRB,Speed/3);
}
void forwardright() {
  digitalWrite(LF1,LOW);
  digitalWrite(LF2,HIGH);
  analogWrite(ENLF,Speed/3);
  digitalWrite(RF1,LOW);
  digitalWrite(RF2,HIGH);
  analogWrite(ENRF,Speed);///3
  digitalWrite(LB1,LOW);
  digitalWrite(LB2,HIGH);
  analogWrite(ENLB,Speed/3);
  digitalWrite(RB1,LOW);
  digitalWrite(RB2,HIGH);
  analogWrite(ENRB,Speed);
}
void backleft() {
  digitalWrite(LF1,HIGH);
  digitalWrite(LF2,LOW);
  analogWrite(ENLF,Speed);
  digitalWrite(RF1,HIGH);
  digitalWrite(RF2,LOW);
  analogWrite(ENRF,Speed/3);
  digitalWrite(LB1,HIGH);
  digitalWrite(LB2,LOW);
  analogWrite(ENLB,Speed);
  digitalWrite(RB1,HIGH);
  digitalWrite(RB2,LOW);
  analogWrite(ENRB,Speed/3);
}
void backright() {
  digitalWrite(LF1,HIGH);
  digitalWrite(LF2,LOW);
  analogWrite(ENLF,Speed/3);
  digitalWrite(RF1,HIGH);
  digitalWrite(RF2,LOW);
  analogWrite(ENRF,Speed);
  digitalWrite(LB1,HIGH);
  digitalWrite(LB2,LOW);
  analogWrite(ENLB,Speed/3);
  digitalWrite(RB1,HIGH);
  digitalWrite(RB2,LOW);
  analogWrite(ENRB,Speed);
}
void Stop() {
  digitalWrite(LF1,LOW);
  digitalWrite(LF2,LOW);
  analogWrite(ENLF,0);
  digitalWrite(RF1,LOW);
  digitalWrite(RF2,LOW);
  analogWrite(ENRF,0);
  digitalWrite(LB1,LOW);
  digitalWrite(LB2,LOW);
  analogWrite(ENLB,0);
  digitalWrite(RB1,LOW);
  digitalWrite(RB2,LOW);
  analogWrite(ENRB,0);
}

void loop() {
  char i;
  if (mySerial.available()>0)
  {
  i= mySerial.read();
  Stop();
  LSpeed = Speed;
  RSpeed = Speed;
  Serial.println(gyroAngleX);
  switch (i) {
      case 'F':
        Forward();
        break;
      case 'B':
        Backward();
        break;
      case 'L':
        gyroAngleX = 0;
        Left();
        break;
      case 'R':
        gyroAngleX = 0;
        Right();
        break;
      case 'G':
        gyroAngleX = 0;
        forwardleft();
        break;
      case 'I':
        gyroAngleX = 0;
        forwardright();
        break;
      case 'H':
        gyroAngleX = 0;
        backleft();
        break;
      case 'J':
        gyroAngleX = 0;
        backright();
        break;    
       case 'W':
        digitalWrite(dirPin, HIGH);
        temp = 1;
        break;     
       case 'w':
        temp = 0;
        break;          
      case 'U':
        digitalWrite(dirPin, LOW);
        temp = 1;
        break;
      case 'u':
        temp = 0;
        break;    
      case '0':
        Stop();
        break;
        //set speed
      case '1':
        Speed = 90;
        break;
      case '2':
        Speed = 100;
        break;
      case '3':
        Speed = 120;
        break;
      case '4':
        Speed = 130;
        break;
      case '5':
        Speed = 150;
        break;
      case '6':
        Speed = 170;
        break;
      case '7':
        Speed = 190;
        break;
      case '8':
        Speed = 210;
        break;
      case '9':
        Speed = 230;
        break;
      case 'q':
        Speed = 255;
        break;
    }      

    }
  if(temp == 1){
  digitalWrite(stepPin, LOW);
  delayMicroseconds(2000);
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(2000);  
  }
  else if(temp == 0){
  digitalWrite(stepPin, LOW); 
  digitalWrite(stepPin, LOW);
  }
  }
void calculateGyroAngleX(){
  previousTime = currentTime;
  currentTime = micros();
  elapsedTime = (currentTime - previousTime) / 1000000; 
  readGyro();
  GyroX -= GyroErrorX; 
  gyroAngleX += GyroX*elapsedTime  ; 
  
}

void calculateError() {
  // Read gyro values 200 times
  for(int i=0; i<200; i++) {
    readGyro();
    // Sum all readings
    GyroErrorX += GyroX;
  }
  GyroErrorX = GyroErrorX / 200;
  }
void readGyro() {
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
}
