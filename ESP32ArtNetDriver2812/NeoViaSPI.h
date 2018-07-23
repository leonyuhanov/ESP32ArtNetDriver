#ifndef NeoViaSPI_h
#define NeoViaSPI_h
#include "Arduino.h"

class NeoViaSPI
{
  public:
    NeoViaSPI(unsigned short int numLEDs);
    void setPixel(short int pixelIndex, byte *pixelColour);
    void getPixel(short int pixelIndex, byte *pixelColour);
    void encode();
    
    byte* LEDs;
    byte* neoBits;
    
    unsigned short int _rainbowSize;
    unsigned short int _LEDframeLength;
    unsigned short int _NeoBitsframeLength;
    unsigned short int _numLEDs;
    unsigned short int _counter;
    unsigned short int _nCounter;
    byte _colCounter;
    byte _bitCounter;
    
  private:
    byte _blankValue;
    byte _tmpValue;
    byte _blankBlock;
    byte _testValues[4][2];
};

#endif
