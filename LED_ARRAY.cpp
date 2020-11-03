#include <APA102.h>

const uint8_t dataPin = 24;
const uint8_t clockPin = 25;

APA102<dataPin, clockPin> ledStrip;

const uint16_t ledCount = 12;

rgb_color colors[ledCount] ;

const uint8_t brightness = 1;//31 is max

void RGB_Clear()
{
  for (uint16_t j = 0; j < ledCount; j++)
    colors[j] = rgb_color(0, 0, 0);
}

void setup()
{
}

void loop()
{
  for (uint16_t i = 0 ; i < 4; i++)
    for (uint16_t j = 0; j < ledCount; j++)
    {
      RGB_Clear();
      switch (i)
      {
      case 0:
        colors[j] = rgb_color(255, 0, 0);
        break;
      case 1:
        colors[j] = rgb_color(0, 255, 0);
        break;
      case 2:
        colors[j] = rgb_color(0, 0, 255);
        break;
      case 3:
        colors[j] = rgb_color(255, 255, 255);
        break;
      default:
        break;
      }
     
     ledStrip.write(colors, ledCount, brightness);
     delay(250);
    }

     
    /*
    RGB_Clear();
    colors[0] = rgb_color(255, 255, 255);
    ledStrip.write(colors, ledCount, brightness);
    delay(1000);
    RGB_Clear();
    colors[1] = rgb_color(255, 255, 255);
    ledStrip.write(colors, ledCount, brightness);
    delay(1000);
    RGB_Clear();
    colors[2] = rgb_color(255, 255, 255);
    ledStrip.write(colors, ledCount, brightness);
    delay(1000);  */  
  
  
}

