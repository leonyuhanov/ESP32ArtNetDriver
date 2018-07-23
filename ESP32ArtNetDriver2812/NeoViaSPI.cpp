#include "NeoViaSPI.h"

NeoViaSPI::NeoViaSPI(unsigned short int numLEDs)
{  
  _numLEDs = numLEDs;
  _LEDframeLength = _numLEDs*3;
  _NeoBitsframeLength = _LEDframeLength*4;
  
  LEDs = new byte[_LEDframeLength];
  neoBits = new byte[_NeoBitsframeLength];

  _blankValue = 0;
  _blankBlock = 192;
  
  //set up test values;
  _testValues[0][0] = 0;
  _testValues[0][1] = 136;
  _testValues[1][0] = 64;
  _testValues[1][1] = 140;
  _testValues[2][0] = 128;
  _testValues[2][1] = 200;
  _testValues[3][0] = 192;
  _testValues[3][1] = 204;
}

void NeoViaSPI::setPixel(short int pixelIndex, byte *pixelColour)
{
  _counter = 3*pixelIndex;
  LEDs[ _counter ] = pixelColour[1];
  LEDs[ _counter + 1 ] = pixelColour[0];
  LEDs[ _counter + 2 ] = pixelColour[2];
}

void NeoViaSPI::getPixel(short int pixelIndex, byte *pixelColour)
{
   _counter = 3*pixelIndex;
   pixelColour[0] = LEDs[ _counter + 1 ];
   pixelColour[1] = LEDs[ _counter ];
   pixelColour[2] = LEDs[ _counter + 2 ];
}

void NeoViaSPI::encode()
{
  _nCounter = 0;
  //Pixel Loop
  for(_counter=0; _counter<_LEDframeLength; _counter+=3)
  {
    //Colour Loop
    for(_colCounter=0; _colCounter<3; _colCounter++)
    {
      //Bit Loop
      _tmpValue = LEDs[_counter+_colCounter];
      for(_bitCounter=0; _bitCounter<4; _bitCounter++)
      {
        _blankValue = (_tmpValue & _blankBlock);
        if( _blankValue == _testValues[0][0] )
        {
          neoBits[_nCounter] = _testValues[0][1];
        }
        else if( _blankValue == _testValues[1][0] )
        {
          neoBits[_nCounter] = _testValues[1][1];
        }
        else if( _blankValue == _testValues[2][0] )
        {
          neoBits[_nCounter] = _testValues[2][1];
        }
        else if( _blankValue == _testValues[3][0] )
        {
          neoBits[_nCounter] = _testValues[3][1];
        }
        _tmpValue = _tmpValue<<2;
        _nCounter++;
      }
    }
  }
}

