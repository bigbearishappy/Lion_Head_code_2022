#include "Adafruit_NeoPixel.h"
#include "KT403A_Player.h"

//--------------------------------------code of led---------------------------------------------------
#ifdef __AVR__
    #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            6

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      144

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 3; // delay for half a second
//---------------------------------------------------------------------------------------------------


//-----------------------------------code of mp3 player----------------------------------------------
#ifdef __AVR__
    #include <SoftwareSerial.h>
    SoftwareSerial SSerial(2, 3); // RX, TX
    #define COMSerial SSerial
    #define ShowSerial Serial

    KT403A<SoftwareSerial> Mp3Player;
#endif

#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define COMSerial Serial1
    #define ShowSerial SerialUSB

    KT403A<Uart> Mp3Player;
#endif

#ifdef ARDUINO_ARCH_STM32F4
    #define COMSerial Serial
    #define ShowSerial SerialUSB

    KT403A<HardwareSerial> Mp3Player;
#endif

static uint8_t recv_cmd[8] = {};
//------------------------------------------------------------------------------------------------

static uint8_t power_on = 0;

void setup() {
  // initialize digital pin
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, INPUT);

  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) {
      clock_prescale_set(clock_div_1);
  }
  #endif
  // End of trinket special code
  pixels.setBrightness(255);
  pixels.begin(); // This initializes the NeoPixel library.

  //mp3 player initialization
  ShowSerial.begin(9600);
  COMSerial.begin(9600);
  while (!ShowSerial);
  while (!COMSerial);
  Mp3Player.init(COMSerial);
}

// the loop function runs over and over again forever
void loop() {
  for(int i = 0;i < 5; ++i){
    digitalWrite(4, HIGH);
    digitalWrite(8, HIGH);
    delay(200);           
    digitalWrite(4, LOW); 
    digitalWrite(8, LOW);
    delay(200);           
  }

  Mp3Player.play();
  delay(200);

  for (int i = 0; i < NUMPIXELS; i++) {
      int r=random(0,250);
      int g=random(0,250);
      int b=random(0,250);
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
  }

  delay(1000);
    
  for (int i = 0; i < NUMPIXELS; i++) {
      int r=random(0,250);
      int g=random(0,250);
      int b=random(0,250);
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(delayval); // Delay for a period of time (in milliseconds).
  }
  delay(1000);
}
