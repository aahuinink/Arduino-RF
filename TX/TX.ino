#include "manchester.h"
#include "hamming.h"

/*
  Manchester Transmitter example

  In this example transmitter will send 10 bytes array  per transmittion
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

#define TX_PIN  11          //pin where your transmitter is connected
#define datalength 4       //this is size of array
#define button  10          //pin where your active-low button is connected
#define ADC_PIN A0          // pin where the adc is

void setup()
{
  pinMode(ADC_PIN,INPUT);
  pinMode(button,INPUT);

  Serial.begin(115200);
  man.setupTransmit(TX_PIN, MAN_300);
}



void loop()
{
  uint8_t data[datalength];
  int adc_val = 0;
  Hamming encoder = Hamming::Hamming();
  while(1)
  {
    while (digitalRead(button)); // wait for button press indicating the potentiometer has been set
    adc_val = analogRead(ADC_PIN);

    for(char i = 1; i < datalength+1; i++)
    {
      data[datalength-i] = adc_val % 10;
      adc_val /= 10;
    }
    Serial.println();
    Serial.println();
    Serial.print("Pot Value: ");
    for(char i = 0; i<4;i++)
    {
      Serial.print(data[i]);
    }
    uint8_t* encoded_data = encoder.Encode(data, datalength);
    Serial.println();
    Serial.print("Hamming encoded data: ");
    for (char i = 0; i<8; i++)
    {
      Serial.print(encoded_data[i]);
    }
    encoded_data[2] ^= 0x40;
    Serial.println();
    Serial.print("Bit-flip error data: ");
    for (char i = 0; i<8; i++)
    {
      Serial.print(encoded_data[i]);
    }
    uint8_t encoded_length = datalength*2;
    // man.transmitArray(datalength*2, encoded_data);
    man.transmitArray(encoded_length, encoded_data);
    // delay(10000); // prevent transmission for 10s for regulation reasons
  }
}
