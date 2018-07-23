/*
  Writen for the "NODE 32S" Dev Board

  SPI details for this dev board:
    SPI DATA  pin 23 - > connects to the DATA IN pin of the ws2812 or sk6812 pixels
    
 */

//Wifi Library
#include <WiFi.h>
//UPD libarry to read UDP packets from the network
#include <WiFiUdp.h>
//SPI libarry to enable us to use the SPI device to write data to the pixels
#include "SPI.h"
//A custome writen Pixel driver for the APA102/SK9822 pixels
#include "NeoViaSPI.h"
//A custom written cross platform ArtNet driver
#include "artNetPacket.h"

//Networking 
const char * ssid = "ArtNet";
const char * password = "megapixels";
unsigned int artNetPort = 6454;
const short int maxPacketBufferSize = 530;
char packetBuffer[maxPacketBufferSize];
WiFiUDP udp;
short int packetSize=0;
artNetPacket dmxData;
const byte universeRange[2] = {0,1};
const unsigned short int bytesPerFixture = 7*14*3; // Width x Height x 3 bytes(RGB)

//LED Stuff
const unsigned short int numberOfPixelsPerPanel = 14*14;;
const byte bytesPerLed=3, maxBrightness=255;
NeoViaSPI leds = NeoViaSPI(numberOfPixelsPerPanel+1); // I assume you are using PIXEL 0 as the NULL pixel
byte tempColour[3];

//Counters
unsigned short int pixelIndex=0, dmxCounter=0;

void setup()
{
  //Start WIFI using the SSID & Hostname above
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setHostname("ESP32ANNode");
  while (WiFi.status() != WL_CONNECTED)
  {
        delay(100);
  }
  //Start the UDP subsytem
  udp.begin(artNetPort);  

  //Start the SPI device
  SPI.begin();
  SPI.setBitOrder(MSBFIRST); 
  SPI.setFrequency(3333333); 

  renderLEDs();
}

void loop()
{
  //check to see if any data is ready to be read form the UDP buffer
  pollDMX();
  //If the flag dmxData.has Changed is 1 data has been read in form the network to the dmxData object and data is ready
  if(dmxData.hasChanged)
  {
    //reset the flag for th enext read
    dmxData.hasChanged=0;
    //Translate the data inside the dmxData object to the LEDs
    artNetToSPI();
  }
  yield();

}

void pollDMX()
{
     //check to see if any data is available via UDP
     packetSize = udp.parsePacket();
     //If a udb packet is ready to read, check that its the size of an ArtNet Packet 530Bytes
     if(packetSize==maxPacketBufferSize)
     {
        //Read maxPacketBufferSize bytesfrom the UDP buffer to the packetBuffer array
        udp.read(packetBuffer, maxPacketBufferSize);
        //Check that the packets are in the universe we need
        //byte inside packetBuffer[14] is the unvirse ID of this packet
        if(packetBuffer[14]>=universeRange[0] && packetBuffer[14]<=universeRange[1])
        {
          //Store packetBuffer into the dmxData object for ease of use
          dmxData.parseArtNetPacket(packetBuffer);
        }
     }
     udp.flush();
}

void artNetToSPI()
{
  
  //If the current packets Universe is 0 we have data for the left part of the panel
  if(dmxData.universe[0]==0)
  {
    //set the start pixel for the panel to be pixel 0
    pixelIndex = 0;
  }
  //If the current packets Universe is 1 we have data for the right part of the panel
  else if(dmxData.universe[0]==1)
  {
    //set the start pixel for the panel to be pixel 98
    pixelIndex = 98;
  }
  
  for(dmxCounter=0; dmxCounter<bytesPerFixture; dmxCounter+=bytesPerLed-1)
  {
    //copy the RGB values from dmxData.data[dmxCounter into the tempColour object
    memcpy(&tempColour, &dmxData.data[dmxCounter], bytesPerLed-1);
    //Set pixel pixelIndex to color tempColour
    leds.setPixel(pixelIndex, tempColour);
    //Increment ot next pixel
    pixelIndex++;
  }
  //render the data to the LEDs
  renderLEDs();
}

void renderLEDs()
{
  //Write leds._frameLength number of bytes from the array leds.neoBits
  leds.encode();
  SPI.writeBytes(leds.neoBits, leds._NeoBitsframeLength); 
  delayMicroseconds(80);
}
