#include <Arduino.h>
#include <MIDI.h>
#include <FastLED.h>
#include <RotaryEncoder.h>

// DECLERATION OF FUNCTIONS

void myNoteOn(byte channel, byte pitch, byte velocity);
void modechange();
void patterns();
void nextPattern();
void prevPattern();
void addGlitter();
void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();

// ENCODER SETTING

#define PIN_IN1 14
#define PIN_IN2 15
#define modeButton 16

// PINS FOR LED STRIPS

#define TOM1PIN 8
#define TOM2PIN 7
#define TOM3PIN 10
#define SNAREPIN 12
#define BASSPIN 11
#define HIHATPIN 13
#define CRASHPIN 6
#define RIDEPIN 9

// NUMBER OF LEDS PER PAD

#define TOM1LEDS 34
#define TOM2LEDS 34
#define TOM3LEDS 34
#define SNARELEDS 54
#define BASSLEDS 67
#define HIHATLEDS 30
#define CRASHLEDS 37
#define RIDELEDS 37
#define NUM_LEDS TOM1LEDS + TOM2LEDS + TOM3LEDS + SNARELEDS + BASSLEDS + HIHATLEDS + CRASHLEDS + RIDELEDS // TOTAL LEDS
#define NUM_STRIPS 8                                                                                      // TOTAL STRIPS

// PATTERN SPECIFIC SETTINGS
#define FRAMES_PER_SECOND 120
#define SPARKING 190
#define COOLING 55 // TOTAL STRIPS

//CREATE MIDI AND ENCODER

MIDI_CREATE_INSTANCE(HardwareSerial, Serial5, MIDI);

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

// VARIABLES AND DEFAULTS

byte brightness = 255;
byte minBrightness = 10;

byte tom1Color = 45;
byte tom2Color = 160;
byte tom3Color = 95;
byte snareColor = 0;
byte bassColor = 210;
byte hihatColor = 45;
byte crashColor = 160;
byte rideColor = 95;

byte tom1Brightness = 255;
byte tom2Brightness = 255;
byte tom3Brightness = 255;
byte snareBrightness = 255;
byte bassBrightness = 255;
byte hihatBrightness = 255;
byte crashBrightness = 255;
byte rideBrightness = 255;

//DEBUG TESTING

bool setMode = false;

// CREATE LED STRIPS

CRGB tom1strip[TOM1LEDS];
CRGB tom2strip[TOM1LEDS];
CRGB tom3strip[TOM1LEDS];
CRGB snarestrip[SNARELEDS];
CRGB bassstrip[BASSLEDS];
CRGB hihatstrip[HIHATLEDS];
CRGB crashstrip[CRASHLEDS];
CRGB ridestrip[RIDELEDS];
CRGB leds[NUM_LEDS];
CLEDController *controllers[NUM_STRIPS];

void setup()
{
  Serial.begin(115200);
  Serial5.begin(31250);
  Serial.println("I am ALIVE and ready to send MIDI Stuff");

  pinMode(modeButton, INPUT_PULLUP);

  FastLED.addLeds<1, WS2812B, TOM1PIN, GRB>(tom1strip, TOM1LEDS);
  FastLED.addLeds<1, WS2812B, TOM2PIN, GRB>(tom2strip, TOM2LEDS);
  FastLED.addLeds<1, WS2812B, TOM3PIN, GRB>(tom3strip, TOM3LEDS);
  FastLED.addLeds<1, WS2812B, SNAREPIN, GRB>(snarestrip, SNARELEDS);
  FastLED.addLeds<1, WS2812B, BASSPIN, GRB>(bassstrip, BASSLEDS);
  FastLED.addLeds<1, WS2812B, HIHATPIN, GRB>(hihatstrip, HIHATLEDS);
  FastLED.addLeds<1, WS2812B, CRASHPIN, GRB>(crashstrip, CRASHLEDS);
  FastLED.addLeds<1, WS2812B, RIDEPIN, GRB>(ridestrip, RIDELEDS);

  Serial.println(" Testing Strips :::::: They should flash 3 times. ");
  delay(500);

  fill_solid(tom1strip, TOM1LEDS, CHSV(tom1Color, 255, tom1Brightness));
  fill_solid(tom2strip, TOM2LEDS, CHSV(tom2Color, 255, tom2Brightness));
  fill_solid(tom3strip, TOM3LEDS, CHSV(tom3Color, 255, tom3Brightness));
  fill_solid(snarestrip, SNARELEDS, CHSV(snareColor, 255, snareBrightness));
  fill_solid(bassstrip, BASSLEDS, CHSV(bassColor, 255, bassBrightness));
  fill_solid(hihatstrip, HIHATLEDS, CHSV(hihatColor, 255, hihatBrightness));
  fill_solid(crashstrip, CRASHLEDS, CHSV(crashColor, 255, crashBrightness));
  fill_solid(ridestrip, RIDELEDS, CHSV(rideColor, 255, rideBrightness));
  FastLED.show(50);
  Serial.println(" FLASH 1");
  delay(500);
  FastLED.clear();
  FastLED.show();
  delay(500);
  fill_solid(tom1strip, TOM1LEDS, CHSV(tom1Color, 255, tom1Brightness));
  fill_solid(tom2strip, TOM2LEDS, CHSV(tom2Color, 255, tom2Brightness));
  fill_solid(tom3strip, TOM3LEDS, CHSV(tom3Color, 255, tom3Brightness));
  fill_solid(snarestrip, SNARELEDS, CHSV(snareColor, 255, snareBrightness));
  fill_solid(bassstrip, BASSLEDS, CHSV(bassColor, 255, bassBrightness));
  fill_solid(hihatstrip, HIHATLEDS, CHSV(hihatColor, 255, hihatBrightness));
  fill_solid(crashstrip, CRASHLEDS, CHSV(crashColor, 255, crashBrightness));
  fill_solid(ridestrip, RIDELEDS, CHSV(rideColor, 255, rideBrightness));
  FastLED.show(50);
  Serial.println(" FLASH 2");
  delay(500);
  FastLED.clear();
  FastLED.show();
  delay(500);
  fill_solid(tom1strip, TOM1LEDS, CHSV(tom1Color, 255, tom1Brightness));
  fill_solid(tom2strip, TOM2LEDS, CHSV(tom2Color, 255, tom2Brightness));
  fill_solid(tom3strip, TOM3LEDS, CHSV(tom3Color, 255, tom3Brightness));
  fill_solid(snarestrip, SNARELEDS, CHSV(snareColor, 255, snareBrightness));
  fill_solid(bassstrip, BASSLEDS, CHSV(bassColor, 255, bassBrightness));
  fill_solid(hihatstrip, HIHATLEDS, CHSV(hihatColor, 255, hihatBrightness));
  fill_solid(crashstrip, CRASHLEDS, CHSV(crashColor, 255, crashBrightness));
  fill_solid(ridestrip, RIDELEDS, CHSV(rideColor, 255, rideBrightness));
  FastLED[0].showLeds(100);
  FastLED[1].showLeds(100);
  FastLED[2].showLeds(100);
  FastLED[3].showLeds(100);
  FastLED[4].showLeds(100);
  FastLED[5].showLeds(100);
  FastLED[6].showLeds(100);
  FastLED[7].showLeds(100);
  Serial.println(" FLASH 3");
  delay(500);
  FastLED.clear();
  FastLED.show();

  Serial.println("BASIC VERSION PATTERNS AND SET COLORS V1.5 (usbMIDI) ");

  MIDI.setHandleNoteOn(myNoteOn);
  MIDI.begin(10);
  //usbMIDI.setHandleNoteOn(myNoteOn);
  attachInterrupt(digitalPinToInterrupt(modeButton), modechange, CHANGE); // Mode Change Interrupt
}
// PATTERN SETTING
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
    rainbow,
    rainbowWithGlitter,
    confetti,
    sinelon,
    juggle,
    bpm,
};
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;                  // rotating "base color" used by many of the patterns

void loop()
{
  //usbMIDI.read();
  MIDI.read();
  encoder.tick();

  if (setMode == true)
  {
    patterns();
  }
  static int pos = 10;
  byte newPos = encoder.getPosition();
  if (pos != newPos)
  {
    pos = newPos;
    minBrightness = newPos;
    Serial.println(newPos);
  }
  EVERY_N_MILLISECONDS(5)
  {
    if (tom1Brightness > minBrightness)
    {
      tom1Brightness--;
      // fill_solid(tom1strip, TOM1LEDS, CHSV(tom1Color, 255, tom1Brightness));
      FastLED[0].showLeds(tom1Brightness);
    }
    if (tom2Brightness > minBrightness)
    {
      tom2Brightness--;
      //fill_solid(tom2strip, TOM2LEDS, CHSV(tom2Color, 255, tom2Brightness));
      FastLED[1].showLeds(tom2Brightness);
    }
    if (tom3Brightness > minBrightness)
    {
      tom3Brightness--;
      // fill_solid(tom3strip, TOM3LEDS, CHSV(tom3Color, 255, tom3Brightness));
      FastLED[2].showLeds(tom3Brightness);
    }
    if (snareBrightness > minBrightness)
    {
      snareBrightness--;
      //fill_solid(snarestrip, SNARELEDS, CHSV(snareColor, 255, snareBrightness));
      FastLED[3].showLeds(snareBrightness);
    }
    if (bassBrightness > minBrightness)
    {
      bassBrightness--;
      //fill_solid(bassstrip, BASSLEDS, CHSV(bassColor, 255, bassBrightness));
      FastLED[4].showLeds(bassBrightness);
    }
    if (hihatBrightness > minBrightness)
    {
      hihatBrightness--;
       //fill_solid(hihatstrip, HIHATLEDS, CHSV(hihatColor, 255, hihatBrightness));
      FastLED[5].showLeds(hihatBrightness);
    }
    if (crashBrightness > minBrightness)
    {
      crashBrightness--;
      //fill_solid(crashstrip, CRASHLEDS, CHSV(crashColor, 255, crashBrightness));
      FastLED[6].showLeds(crashBrightness);
    }
    if (rideBrightness > minBrightness)
    {
      rideBrightness--;
      //fill_solid(ridestrip, RIDELEDS, CHSV(rideColor, 255, rideBrightness));
      FastLED[7].showLeds(rideBrightness);
    }
    //FastLED.show();
  }
}
void patterns()
{
  RotaryEncoder::Direction dir = encoder.getDirection();
  if (dir == RotaryEncoder::Direction::CLOCKWISE)
  {
    nextPattern();
   // Serial.println(gCurrentPatternNumber);
  }
  if (dir == RotaryEncoder::Direction::COUNTERCLOCKWISE)
  {
    prevPattern();
    //Serial.println(gCurrentPatternNumber);
  }
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
  //FastLED[0].showLeds(tom1Brightness);
  //FastLED[1].showLeds(tom2Brightness);
  //FastLED[2].showLeds(tom3Brightness);
  //FastLED[3].showLeds(snareBrightness);
  //FastLED[4].showLeds(bassBrightness);
  //FastLED[5].showLeds(hihatBrightness);
  //FastLED[6].showLeds(crashBrightness);
  //FastLED[7].showLeds(rideBrightness);
  
  // do some periodic updates
  EVERY_N_MILLISECONDS(20);
  {
    gHue++; // slowly cycle the "base color" through the rainbow
  }
}
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}
void prevPattern()
{

  gCurrentPatternNumber = (gCurrentPatternNumber - 1);
  if (gCurrentPatternNumber > ARRAY_SIZE(gPatterns))
  {
    gCurrentPatternNumber = ARRAY_SIZE(gPatterns) - 1;
  }
}
void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow(tom1strip, TOM1LEDS, gHue, 7);
  fill_rainbow(tom2strip, TOM2LEDS, gHue, 7);
  fill_rainbow(tom3strip, TOM3LEDS, gHue, 7);
  fill_rainbow(snarestrip, SNARELEDS, gHue, 7);
  fill_rainbow(bassstrip, BASSLEDS, gHue, 7);
  fill_rainbow(hihatstrip, HIHATLEDS, gHue, 7);
  fill_rainbow(crashstrip, CRASHLEDS, gHue, 7);
  fill_rainbow(ridestrip, RIDELEDS, gHue, 7);
}
void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}
void addGlitter(fract8 chanceOfGlitter)
{
  if (random8() < chanceOfGlitter)
  {
    tom1strip[random16(TOM1LEDS)] += CRGB::White;
    tom2strip[random16(TOM2LEDS)] += CRGB::White;
    tom3strip[random16(TOM3LEDS)] += CRGB::White;
    snarestrip[random16(SNARELEDS)] += CRGB::White;
    bassstrip[random16(BASSLEDS)] += CRGB::White;
    hihatstrip[random16(HIHATLEDS)] += CRGB::White;
    crashstrip[random16(CRASHLEDS)] += CRGB::White;
    ridestrip[random16(RIDELEDS)] += CRGB::White;
  }
}
void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(tom1strip, TOM1LEDS, 10);
  fadeToBlackBy(tom2strip, TOM2LEDS, 10);
  fadeToBlackBy(tom3strip, TOM3LEDS, 10);
  fadeToBlackBy(snarestrip, SNARELEDS, 10);
  fadeToBlackBy(bassstrip, BASSLEDS, 10);
  fadeToBlackBy(hihatstrip, HIHATLEDS, 10);
  fadeToBlackBy(crashstrip, CRASHLEDS, 10);
  fadeToBlackBy(ridestrip, RIDELEDS, 10);

  int pos1 = random16(TOM1LEDS);
  int pos2 = random16(TOM2LEDS);
  int pos3 = random16(TOM3LEDS);
  int pos4 = random16(SNARELEDS);
  int pos5 = random16(BASSLEDS);
  int pos6 = random16(HIHATLEDS);
  int pos7 = random16(CRASHLEDS);
  int pos8 = random16(RIDELEDS);

  tom1strip[pos1] += CHSV(gHue + random8(64), 200, tom1Brightness);
  tom2strip[pos2] += CHSV(gHue + random8(64), 200, tom2Brightness);
  tom3strip[pos3] += CHSV(gHue + random8(64), 200, tom3Brightness);
  snarestrip[pos4] += CHSV(gHue + random8(64), 200, snareBrightness);
  bassstrip[pos5] += CHSV(gHue + random8(64), 200, bassBrightness);
  hihatstrip[pos6] += CHSV(gHue + random8(64), 200, hihatBrightness);
  crashstrip[pos7] += CHSV(gHue + random8(64), 200, crashBrightness);
  ridestrip[pos8] += CHSV(gHue + random8(64), 200, rideBrightness);
}
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy(tom1strip, TOM1LEDS, 20);
  fadeToBlackBy(tom2strip, TOM2LEDS, 20);
  fadeToBlackBy(tom3strip, TOM3LEDS, 20);
  fadeToBlackBy(snarestrip, SNARELEDS, 20);
  fadeToBlackBy(bassstrip, BASSLEDS, 20);
  fadeToBlackBy(hihatstrip, HIHATLEDS, 20);
  fadeToBlackBy(crashstrip, CRASHLEDS, 20);
  fadeToBlackBy(ridestrip, RIDELEDS, 20);
  int pos1 = beatsin16(13, 0, TOM1LEDS - 1);
  int pos2 = beatsin16(13, 0, TOM2LEDS - 1);
  int pos3 = beatsin16(13, 0, TOM3LEDS - 1);
  int pos4 = beatsin16(13, 0, SNARELEDS - 1);
  int pos5 = beatsin16(13, 0, BASSLEDS - 1);
  int pos6 = beatsin16(13, 0, HIHATLEDS - 1);
  int pos7 = beatsin16(13, 0, CRASHLEDS - 1);
  int pos8 = beatsin16(13, 0, RIDELEDS - 1);
  tom1strip[pos1] += CHSV(gHue, 255, 192);
  tom2strip[pos2] += CHSV(gHue, 255, 192);
  tom3strip[pos3] += CHSV(gHue, 255, 192);
  snarestrip[pos4] += CHSV(gHue, 255, 192);
  bassstrip[pos5] += CHSV(gHue, 255, 192);
  hihatstrip[pos6] += CHSV(gHue, 255, 192);
  crashstrip[pos7] += CHSV(gHue, 255, 192);
  ridestrip[pos8] += CHSV(gHue, 255, 192);
}
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for (int i = 0; i < TOM1LEDS; i++)
  { //9948
    tom1strip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    tom2strip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    tom3strip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  for (int i = 0; i < SNARELEDS; i++)
  { //9948
    snarestrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  for (int i = 0; i < BASSLEDS; i++)
  { //9948
    bassstrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  for (int i = 0; i < HIHATLEDS; i++)
  { //9948
    hihatstrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  for (int i = 0; i < CRASHLEDS; i++)
  { //9948
    crashstrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
  for (int i = 0; i < RIDELEDS; i++)
  { //9948
    ridestrip[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}
void juggle()
{
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy(tom1strip, TOM1LEDS, 20);
  fadeToBlackBy(tom2strip, TOM2LEDS, 20);
  fadeToBlackBy(tom3strip, TOM3LEDS, 20);
  fadeToBlackBy(snarestrip, SNARELEDS, 20);
  fadeToBlackBy(bassstrip, BASSLEDS, 20);
  fadeToBlackBy(hihatstrip, HIHATLEDS, 20);
  fadeToBlackBy(crashstrip, CRASHLEDS, 20);
  fadeToBlackBy(ridestrip, RIDELEDS, 20);
  byte dothue = 0;
  for (int i = 0; i < 8; i++)
  {
    tom1strip[beatsin16(i + 7, 0, TOM1LEDS - 1)] |= CHSV(dothue, 200, tom1Brightness);
    tom2strip[beatsin16(i + 7, 0, TOM2LEDS - 1)] |= CHSV(dothue, 200, tom2Brightness);
    tom3strip[beatsin16(i + 7, 0, TOM3LEDS - 1)] |= CHSV(dothue, 200, tom3Brightness);
    snarestrip[beatsin16(i + 7, 0, SNARELEDS - 1)] |= CHSV(dothue, 200, snareBrightness);
    bassstrip[beatsin16(i + 7, 0, BASSLEDS - 1)] |= CHSV(dothue, 200, bassBrightness);
    hihatstrip[beatsin16(i + 7, 0, HIHATLEDS - 1)] |= CHSV(dothue, 200, hihatBrightness);
    crashstrip[beatsin16(i + 7, 0, CRASHLEDS - 1)] |= CHSV(dothue, 200, crashBrightness);
    ridestrip[beatsin16(i + 7, 0, RIDELEDS - 1)] |= CHSV(dothue, 200, rideBrightness);
    dothue += 32;
  }
}
void myNoteOn(byte channel, byte pitch, byte velocity)
{
  if (velocity >85)
  {
    velocity = 85;
    }
  velocity = map(velocity, 1, 85, 1, 255);
  brightness = velocity;
  Serial.println (velocity);
  switch (pitch)
  {
  case 36: //bass
    if (setMode == true)
    {
      bassBrightness = brightness;
      Serial.println("bass pattern");
      break;
    }
    bassBrightness = brightness;
    fill_solid(bassstrip, BASSLEDS, CHSV(bassColor, 255, bassBrightness));
    FastLED[4].showLeds();
    Serial.println("BASS NORMAL");
    break;
  case 38: //SNARE
  case 40: //SNARE RIM
  case 37: //SNARE X-stick
    if (setMode == true)
    {
      snareBrightness = brightness;
      break;
    }
    snareBrightness = brightness;
    fill_solid(snarestrip, SNARELEDS, CHSV(snareColor, 255, snareBrightness));
    FastLED[3].showLeds();
    break;
  case 48: //TOM 1 HEAD
  case 50: //TOM 1 RIM
     if (setMode == true)
    {
     tom1Brightness = brightness;
      break;
    }
   
    tom1Brightness = brightness;
    fill_solid(tom1strip, TOM1LEDS, CHSV(tom1Color, 255, tom1Brightness));
    FastLED[0].showLeds();
    break;
  case 45: //TOM 2 HEAD
  case 47: //TOM 2 RIM
    if (setMode == true)
    {
      tom2Brightness = brightness;
      break;
    }

    tom2Brightness = brightness;
    fill_solid(tom2strip, TOM2LEDS, CHSV(tom2Color, 255, tom2Brightness));
    FastLED[1].showLeds();
    break;
  case 43: //TOM 3 HEAD
  case 58: //TOM 3 RIM
    if (setMode == true)
    {
      tom3Brightness = brightness;
      break;
    }
    tom3Brightness = brightness;
    fill_solid(tom3strip, TOM3LEDS, CHSV(tom3Color, 255, tom3Brightness));
    FastLED[2].showLeds();
    break;
  case 46: //HH OPEN BOW
  case 26: //HH OPEN EDGE
  case 42: //HH CLOSED BOW
  case 22: //HH CLOSED EDGE
  case 44: // HH PEDAL
    if (setMode == true)
    {
      hihatBrightness = brightness;
      break;
    }
    hihatBrightness = brightness;
    fill_solid(hihatstrip, HIHATLEDS, CHSV(hihatColor, 255, hihatBrightness));
    FastLED[5].showLeds();
    break;
  case 49: // CRASH 1 BOW
  case 55: // CRASH 1 EDGE
    if (setMode == true)
    {
      crashBrightness = brightness;
      break;
    }
    crashBrightness = brightness;
    fill_solid(crashstrip, CRASHLEDS, CHSV(crashColor, 255, crashBrightness));
    FastLED[6].showLeds();
    break;
  case 51: //RIDE BOW
  case 59: //RIDE EDGE
    if (setMode == true)
    {
      rideBrightness = brightness;
      break;
    }
    rideBrightness = brightness;
    fill_solid(ridestrip, RIDELEDS, CHSV(rideColor, 255, rideBrightness));
    FastLED[7].showLeds();
    break;
  }
}
//mode change interrupt
void modechange()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    if (setMode == true)
    {
      setMode = false;
    }
    else
    {
      setMode = true;
    }
    //Serial.println("Mode Counter is");
    Serial.println(setMode);
  }
  last_interrupt_time = interrupt_time;
}
