#include "manchester.h"
#include "hamming.h"
#include <Servo.h>
/*

  Manchester Receiver example
  
  In this example receiver will receive array of 10 bytes per transmittion

  try different speeds using this constants, your maximum possible speed will 
  depend on various factors like transmitter type, distance, microcontroller speed, ...

  MAN_300 0
  MAN_600 1
  MAN_1200 2
  MAN_2400 3
  MAN_4800 4
  MAN_9600 5
  MAN_19200 6
  MAN_38400 7

*/

#define RX_PIN 11
#define LED_PIN 13
#define SERVO_PIN 10
#define BUFFER_SIZE 8
uint8_t buffer[BUFFER_SIZE];
uint8_t Rx_num=0;
Servo servo;
void setup() {
  Serial.begin(115200); 
  man.setupReceive(RX_PIN, MAN_300 );
  man.beginReceiveArray(BUFFER_SIZE, buffer);
  servo.attach(SERVO_PIN);
  servo.write(90);
}

void loop() {
	
	Hamming decoder = Hamming::Hamming();
  uint8_t* decoded_data;
  if (man.receiveComplete()) {
	  
	  Rx_num++;
    Serial.println();
	  Serial.println("Received... message # ");
	  Serial.print(Rx_num);
	  digitalWrite(LED_PIN, 1);
	  

    man.beginReceiveArray(BUFFER_SIZE, buffer);
    
    for(int i=0; i<BUFFER_SIZE; i++){
        Serial.print(buffer[i]);
    }
    Serial.println();
    decoded_data = decoder.Decode(buffer, 4);
    for(int i=0;i<4;i++)
    {
      Serial.print(decoded_data[i]);
    }
    int angle = 0;
    int pot_val = 0;
    for (int i = 0; i < 4; i++)
    {
      pot_val += decoded_data[3-i]*pow(10,i);
    }
    angle = map(pot_val,0,1023,0,180);
    servo.write(angle);
    Serial.println();
    Serial.print("Pot Val: ");
    Serial.print(pot_val);
    Serial.println();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.println();
  }
  
}
