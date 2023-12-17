#ifndef artNetPacket_h
#define artNetPacket_h
#include "Arduino.h"

class artNetPacket
{
  public:
    artNetPacket();
    void parseArtNetPacket(char *packetBuffer);
    short unsigned int scaleInput(unsigned short int index, unsigned short int minOutput, unsigned short int maxOutput);
    byte dumpData(short int index);

    char header[7];             //0-6
    byte opcode[2];             //8-9
    byte protocolVersion[2];    //10-11
    byte sequence;              //12
    byte physical;              //13
    byte universe[2];           //14-15
    byte dataLength[2];         //16-17
    byte* data;                 //18-530
    byte hasChanged;
    
    short int pCnt;
    short int pIndex;
    
  private:
};

#endif
