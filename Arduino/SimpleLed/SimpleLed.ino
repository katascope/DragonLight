#include <Adafruit_NeoPixel.h>
#define SERIAL_BAUD_RATE    115200  //Serial baud rate
#define LED_STRIP_LENGTH    86      //Number of LED in strip
#define LED_PIN             3       //Arduino Pin for strip control
#define BRIGHTNESS          50      //Default brightness
static Adafruit_NeoPixel ledStrip = Adafruit_NeoPixel(LED_STRIP_LENGTH, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t LEDRGB(uint8_t r, uint8_t g, uint8_t b) { return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b; }

void pattern(int id)
{
  switch (id)
  {
     case 1: //Black
        for(int led=0; led<LED_STRIP_LENGTH; led++) ledStrip.setPixelColor(led, LEDRGB(0,0,0));
      break;
     case 2: //Red/Green/Blue
        for(int led=0; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(255,0,0));
        for(int led=1; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(0,255,0));
        for(int led=2; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(0,0,255));
      break;
    case 3: //Cyan/Magenta/Yellow
        for(int led=0; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(0,255,255));
        for(int led=1; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(255,0,255));
        for(int led=2; led<LED_STRIP_LENGTH; led+=3) ledStrip.setPixelColor(led, LEDRGB(255,255,0));
      break;
    case 4: //White
        for(int led=0; led<LED_STRIP_LENGTH; led++) ledStrip.setPixelColor(led, LEDRGB(255,255,255));
      break;
  }  
  ledStrip.show();
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.setTimeout(50);
  delay( 3000 ); // power-up safety delay
  ledStrip.begin();
  ledStrip.setBrightness(BRIGHTNESS);
  pattern(2);
}

void loop()
{
  while (Serial.available())
  {
      String str = Serial.readString();
      switch (str[0])
      {
        case '1': pattern(1);break;
        case '2': pattern(2);break;
        case '3': pattern(3);break;
        case '4': pattern(4);break;
      }
  }    
}
