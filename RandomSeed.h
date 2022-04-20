#include "Arduino.h"
#pragma once

class RandomSeed
{
  private:
    RandomSeed() = default;

   public:
    static RandomSeed &getInstance();
    RandomSeed(const RandomSeed &) = delete;
    RandomSeed &operator=(const RandomSeed &) = delete;
    
    void reseedRandom( uint32_t* address );
    inline void reseedRandom( unsigned short address );
    void reseedRandomInit( uint32_t* address, uint32_t value );
    inline void reseedRandomInit( unsigned short address, uint32_t value);
};

extern RandomSeed &RandomSeed;
