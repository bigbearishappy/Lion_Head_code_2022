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

static uint8_t active_cnt = 0;

void setup() {
  // initialize digital pin
  pinMode(5, OUTPUT);//left eye
  pinMode(6, OUTPUT);//right eye
  pinMode(7, INPUT);
  pinMode(2, INPUT);//PIR sensor input

  // initialize the pwm of eye
  analogWrite(5, 255);
  analogWrite(9, 255);

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
  bool state = digitalRead(2); // read from PIR sensor
  if (state == 1) {
    for(int i = 0;i < 2; ++i){
      analogWrite(5, 0);
      analogWrite(9, 0);
      delay(200);     
      analogWrite(5, 255);
      analogWrite(9, 255);
      delay(200);                 

      int r=random(0,250);
      int g=random(0,250);
      int b=random(0,250);

      if (active_cnt >= NUMPIXELS)
        active_cnt = 0;
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(active_cnt++, pixels.Color(r, g, b)); // Moderately bright green color.

      pixels.show(); // This sends the updated pixel color to the hardware.
    }
  } else {
    analogWrite(5, 0);
    analogWrite(9, 0);
  }
  delay(10);
}
