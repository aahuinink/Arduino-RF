/*
* @file: "hamming.cpp"
* @brief: Source code that contains an implementation of a Hamming code encoder and decoder.
*/

#include "hamming.h"

  // public constructor
Hamming::Hamming();  // encoder/decoder
  // Encode function
char* Hamming::Encode(char *msg, int length)
  {
    int byte_position = 0;
    char byte1, byte2, data_byte, data_bit;
    //parity bit
    int pb1 = 0;
    int pb2 = 0;
    int pb4 = 0;
    char* encoded_msg = new char[length*2];

    //for each byte in the message
    for (char byte=0; byte < length; byte++)
    {
      // grab data byte for more processing
      data_byte = msg[byte];
      
      // reset parity bits
      pb1, pb2, pb4 = 0,0,0;
      // calculate parity bits
      for(char bit_position = 1; bit_position < 9; bit_position++)
      {
        // grab bit from data byte
        data_bit = data_byte & 0x01;
        // toggle parity bits if bit position is correct
        pb1 = (bit_position & 1) ? pb1 ^ data_bit : pb1;
        pb2 = (bit_position & 2) ? pb2 ^ data_bit : pb2; 
        pb4 = (bit_position & 4) ? pb4 ^ data_bit : pb4;

        // shift data byte down to grab next bit
        data_byte = data_byte >> 1;
      }

      // spread the byte over 2 bytes making room for the parity bits
        // byte1: mgsbit 5,4,3,2, pb4, msb1, pb2, pb1 
      byte1 = pb1 | (pb2 << 1) | ((msg[byte] & 0x01) << 2) | (pb4 << 3) | ((msg[byte] & 0x1E) << 3);
      byte1 = ((msg[byte] & 0x1E) << 3) | (pb4 << 3) | (msg[byte] & 0x01) | (pb2 << 1) | pb1;
        // byte2: 0,0,0,0,0,0, msgb 8,7,6
      byte2 = (msg[byte] & 0xE0) >> 5;

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
  @param length : The expected length of the message
  @returns decoded_msg : Pointer to a char array of the decoded chars
*/
char* Hamming::Decode(char* msg, int length)
  {
    char* decoded_msg = new char[length];
    return decoded_msg;
  }
