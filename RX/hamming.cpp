/*
* @file: "hamming.cpp"
* @brief: Source code that contains an implementation of a Hamming code encoder and decoder.
*/

#include "hamming.h"

  // public constructor
Hamming::Hamming() {};  // encoder/decoder
  // Encode function
uint8_t* Hamming::Encode(uint8_t *msg, int length)
  {
  int byte_position, data_byte;
        // byte1 and byte2
  uint8_t byte1;
  uint8_t byte2;
    // data_bytes for processing
  uint8_t data_bit = 0;
    //parity bit
    int pb1 = 0;
    int pb2 = 0;
    int pb4 = 0;
    int pb8 = 0;
    uint8_t* encoded_msg = new uint8_t[length*2];
  byte_position = 0;
    //for each byte in the message
    for (uint8_t byte=0; byte < length; byte++)
    {
      // spread the byte into an int making room for the parity bits
        // msgbit8,7,6,5,pb8,msgbit4,3,2,pb4, msb1, pb2, pb1 
      data_byte = (((int)msg[byte] & 0xF0) << 4) | (((int)msg[byte] & 0x0E) << 3) | (((int)msg[byte] & 0x01) << 2);
      
      // reset parity bits
      pb1 = pb2 = pb4 = pb8 = 0;
      // calculate parity bits
      for(uint8_t bit_position = 1; bit_position < 13; bit_position++)
      {
        // grab bit from data byte
        data_bit = data_byte & 0x0001;
        // toggle parity bits if bit position is correct and there is another 1 in that spot
        pb1 = (bit_position & 0x01) ? pb1 ^ data_bit : pb1;
        pb2 = (bit_position & 0x02) ? pb2 ^ data_bit : pb2; 
        pb4 = (bit_position & 0x04) ? pb4 ^ data_bit : pb4;
        pb8 = (bit_position & 0x08) ? pb8 ^ data_bit : pb8;

        data_byte = data_byte >> 1;
      }

      // spread the encoded data across 2 bytes
        //byte1 = pb8,msgbit4,3,2,pb4, msb1, pb2, pb1
      byte1 = (pb8 << 7) | ((msg[byte] << 3) & 0x70) | (pb4 << 3) | (msg[byte]<<2)&0x04 | (pb2 << 1) | pb1;
        // byte2 = 0,0,0,0,msgb8,7,6,5
      byte2 = (msg[byte] & 0xF0) >> 4;
      // add encoded bytes to encoded message and increase byte position counter 
      encoded_msg[byte_position] = byte1;
      byte_position++;
      encoded_msg[byte_position] = byte2;
      byte_position++;
    }
    return encoded_msg;
  }

/*
  Decodes a hamming encoded byte array and returns a pointer to the decoded array
  @param msg : The hamming encoded message
  @param length : The expected length of the message AFTER decoding
  @returns decoded_msg : Pointer to a uint8_t array of the decoded uint8_ts
*/
uint8_t* Hamming::Decode(uint8_t* msg, int length)
{
  uint8_t* decoded_msg = new uint8_t[length];
  //variables
    // byte positions
  uint8_t byte_position = 0; // index of byte in recieved message
  uint8_t decode_index = 0;  // index of byte to add to decoded message
  uint8_t flip_index = 0;    // index of byte to flip
  int rx_length = length*2;
    // byte1 and byte2
  uint8_t byte1;
  uint8_t byte2;
    // data_bytes for processing
  int rx_byte = 0;
  int data_byte = 0;
  uint8_t data_bit = 0;
    // parity bits
  int pb1 = 0;
  int pb2 = 0;
  int pb4 = 0;
  int pb8 = 0;

  // process the rx message
  while(byte_position < rx_length)  // while not at the end of the rx message
  {
    pb1 = pb2 = pb4 = pb8 = 0;  // reset parity bits
    // pull out next two bytes and put them into an int for processing  
      //byte1 = pb8,msgbit4,3,2,pb4, msb1, pb2, pb1
    byte1 = msg[byte_position];
    byte_position++;
      // byte2 = 0,0,0,0,msgb8,7,6,5
    byte2 = msg[byte_position];
    byte_position++;

    rx_byte = ((int)byte2 << 8) | (byte1);
    data_byte = rx_byte; // make a copy for data processing
    // calculate parity bits of rx data
    // calculate parity bits
      for(uint8_t bit_position = 1; bit_position < 13; bit_position++)
      {
        // grab next bit from data byte
        data_bit = data_byte & 0x1;
        // toggle parity bits if bit position is correct and there is another 1 in that spot
        pb1 = (bit_position & 0x1)==0x1 ? pb1 ^ data_bit : pb1;
        pb2 = (bit_position & 0x2)==0x2 ? pb2 ^ data_bit : pb2; 
        pb4 = (bit_position & 0x4)==0x4 ? pb4 ^ data_bit : pb4;
        pb8 = (bit_position & 0x8)==0x8 ? pb8 ^ data_bit : pb8;

        data_byte = data_byte >> 1;
      }
    // check if rx parity matches msg parity
      // calculated parity bits must end up as zero
      // flip bits if necessary
    flip_index = (pb8 << 3) | (pb4 << 2) | (pb2 << 1) | pb1;
    if(flip_index != 0x0)
    {
      rx_byte = rx_byte ^ (0x01 << (flip_index-1));  // shift over to the position indicated by flip index
    }
    // compress back to 1 byte uint8_tacter and add to decoded message array
      //rx_byte = 0,0,0,0,msgb8,7,6,5,pb8,msgbit4,3,2,pb4, msb1, pb2, pb1
    decoded_msg[decode_index] = ((rx_byte>>4)&0xF0) | ((rx_byte>>3)&0x0E) | ((rx_byte>>2) & 0x1);
    decode_index++;
  }

  return decoded_msg;
}
