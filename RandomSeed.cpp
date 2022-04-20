#include "RandomSeed.h"

#include "Arduino.h"
#include <avr/eeprom.h>

void RandomSeed::reseedRandom( uint32_t* address ){
  static const uint32_t HappyPrime = 127807 /*937*/;
  uint32_t raw;
  unsigned long seed;

  // Read the previous raw value from EEPROM
  raw = eeprom_read_dword( address );

  // Loop until a seed within the valid range is found
  do{
    // Incrementing by a prime (except 2) every possible raw value is visited
    raw += HappyPrime;

    // Park-Miller is only 31 bits so ignore the most significant bit
    seed = raw & 0x7FFFFFFF;
  }
  while ( (seed < 1) || (seed > 2147483646) );

  // Seed the random number generator with the next value in the sequence
  srandom( seed );  

  // Save the new raw value for next time
  eeprom_write_dword( address, raw );
}

inline void RandomSeed::reseedRandom( unsigned short address ){
  reseedRandom( (uint32_t*)(address) );
}

void RandomSeed::reseedRandomInit( uint32_t* address, uint32_t value ){
  eeprom_write_dword( address, value );
}

inline void RandomSeed::reseedRandomInit( unsigned short address, uint32_t value ){
  reseedRandomInit( (uint32_t*)(address), value );
}
