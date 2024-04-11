/*
* @file: "hamming.h"
* @brief: Header file for "hamming.cpp" that contains an implementation of a Hamming code encoder and decoder.
*/

#ifndef HAMMING_H
#define HAMMING_H
#include <stdint.h>
class Hamming
{
  public:
    Hamming(); //the constructor
    uint8_t* Encode(uint8_t* msg, int length);
    uint8_t* Decode(uint8_t* msg, int length);
};//end of class Hamming
#endif