#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
int command;
#define SERVOMIN  0 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on th
#define maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#define hand1 0
#define hand2 2
#define wrist 4


int hand1_pulse;
int wrist_pulse;
int hand2_pulse;

SoftwareSerial mySerial(11,12); //(TX,RX)

void setup() {
  Serial.begin(9600);
  pwm.begin();
  
 pwm.setOscillatorFrequency(27000000);
 pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  mySerial.begin(9600);
  delay(10);

  pwm.setPWM(wrist, 0, 70);
}
void loop() {
    char i;
  if (mySerial.available()>0)
  {
  i= mySerial.read();

  Serial.println(wrist_pulse);
   switch (i) {
    case 'F':        
        if(wrist_pulse > 500) wrist_pulse = 500;
        else{
        wrist_pulse = wrist_pulse + 10;
        pwm.setPWM(wrist, 0, wrist_pulse);
        }
        break;
    case 'B':
        if(wrist_pulse < 70) wrist_pulse = 70;
        else{
        wrist_pulse = wrist_pulse - 10;
        pwm.setPWM(wrist, 0, wrist_pulse);
        }
        break;
    case 'W': 
        if(wrist_pulse < 70) wrist_pulse = 70;
        else{
        wrist_pulse = wrist_pulse - 10;
        pwm.setPWM(wrist, 0, wrist_pulse);
        }
        break;
    case 'B':
        if(wrist_pulse < 70) wrist_pulse = 70;
        else{
        wrist_pulse = wrist_pulse - 10;
        pwm.setPWM(wrist, 0, wrist_pulse);
        }
        break;     
   }
  }
}
