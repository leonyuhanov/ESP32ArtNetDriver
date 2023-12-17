#include "artNetPacket.h"


artNetPacket::artNetPacket()
{  
  data = new byte[512];
  pIndex = 0;
  hasChanged = 0;
  
}

void artNetPacket::parseArtNetPacket(char *packetBuffer)
{
  //header
  pIndex=0;
  for(pCnt=0; pCnt<7; pCnt++)
  {
    header[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  //opcode
  pIndex++;
  for(pCnt=0; pCnt<2; pCnt++)
  {
    opcode[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  //Protocol Version
  for(pCnt=0; pCnt<2; pCnt++)
  {
    protocolVersion[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  //sequence
  sequence = packetBuffer[pIndex];
  pIndex++;
  //physical
  physical = packetBuffer[pIndex];
  pIndex++;
  //universe
  for(pCnt=0; pCnt<2; pCnt++)
  {
    universe[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  //datalengsth
  for(pCnt=0; pCnt<2; pCnt++)
  {
    dataLength[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  //data
  for(pCnt=0; pCnt<512; pCnt++)
  {
    if(data[pCnt]!=packetBuffer[pIndex] && hasChanged==0)
    {
      hasChanged=1;
    }
    data[pCnt] = packetBuffer[pIndex];
    pIndex++;
  }
  
  //check for blank
  
  pIndex=0;
  for(pCnt=0; pCnt<512; pCnt++)
  {
    pIndex+=data[pCnt];
  }
  if(pIndex==0 && hasChanged==0)
  {
    hasChanged=1;
  }
}

byte artNetPacket::dumpData(short int index)
{
   return data[index];
}

short unsigned int artNetPacket::scaleInput(unsigned short int index, unsigned short int minOutput, unsigned short int maxOutput)
{
  return ((((float)dumpData(index))/256)* ((float)(maxOutput-minOutput))) +minOutput;
}
