/*
* @file: "hamming.h"
* @brief: Header file for "hamming.cpp" that contains an implementation of a Hamming code encoder and decoder.
*/

#ifndef HAMMING_H
#define HAMMING_H

class Hamming
{
  public:
    Hamming(); //the constructor
    char* Encode(char* msg, int length);
    char* Decode(char* msg, int length);
};//end of class Hamming

#endif