/*
    
*/
#include "SPI.h"
#include <WiFi.h>
#include <AsyncUDP.h>
#include "apa102LEDStrip.h"
#include "artNetPacket.h"

//networking
const char * ssid = "SSID";
const char * password = "WIFIKEY";
unsigned int artNetPort = 6454;
const short int maxPacketBufferSize = 530;
char packetBuffer[maxPacketBufferSize];
AsyncUDP udp;
short int packetSize=0;
artNetPacket dmxData;

//DMX Config - This example is set up for 3 Universes with ids 0,1 & 2
const byte numberOfDMXUniverses = 3;
const unsigned short int universeRange[2] = {0,2};  //  [Starting Universe ID, Ending Universe ID] (inclusive)
const unsigned short int artNetFrames[numberOfDMXUniverses][3] = {{0,170,0},{1,170,170},{2,80,340}};
//modify for dynmic
unsigned short int dmxIndex=0, frameCntBreak=0, frameTicks=0, frameTCnt=0, innerFTCnt=0, startPixel=0, pxIndex=0;
const unsigned short int numLeds = 420;
const byte bytesPerLed = 4;
apa102LEDStrip leds = apa102LEDStrip(numLeds, bytesPerLed, 255);
byte tempColour[3] = {0,0,0};

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\n\r\n");

  //Eable WIFI
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("ArtNetNode");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
        delay(100);
        Serial.print(".");
  }
  Serial.print("\r\nONLINE\t");
  Serial.print(WiFi.localIP());

  //Set up UDP
  
  udp.listen(artNetPort);
  udp.onPacket(pollDMX);
   
  //Init SPI for physical Pixel Driver
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setFrequency(5000000);

  //clear all pixels
  renderLEDs();
}

void renderLEDs()
{
  SPI.writeBytes(leds.LEDs, leds._frameLength);
}

//ARTNET STUFF
void pollDMX(AsyncUDPPacket &packet)
{
     packetSize = packet.length();
     if(packetSize==maxPacketBufferSize)
     {
        memcpy(packetBuffer, packet.data(), maxPacketBufferSize);
        //packetBuffer[14] is the UNIVERSE byte check that it is within the range cinfigured above
        if(packetBuffer[14]>=universeRange[0] && packetBuffer[14]<=universeRange[1])
        {
          dmxData.parseArtNetPacket(packetBuffer);
          artNetToSPI(dmxData.universe[0]);
        }
     }
}

void artNetToSPI(byte panelID)
{
  startPixel = artNetFrames[panelID][2];
  dmxIndex = 0;

  for(pxIndex=startPixel; pxIndex<startPixel+artNetFrames[panelID][1]; pxIndex++)
  {
    tempColour[0] = dmxData.data[dmxIndex];
    tempColour[1] = dmxData.data[dmxIndex+1];
    tempColour[2] = dmxData.data[dmxIndex+2];
    leds.setPixel(pxIndex, tempColour);
    dmxIndex+=3;
  }
 
}

void loop()
{    
    /*
    if(dmxData.hasChanged)
    {
      //Serial.printf("\r\n\t\tProcessing DMX DATA For U\t[%d.%d]", dmxData.universe[0],dmxData.universe[1]);
      dmxData.hasChanged = 0;
      artNetToSPI(dmxData.universe[0]);
    }
    */
    renderLEDs();
    yield(); 
}
