#include <Adafruit_NeoPixel.h>

// Define the pin to which the LED serial line is attached...
#define LED_PIN        6
// Tell the pixel library how many pixels there are...
#define NUMPIXELS      8
// Set up the pixel library...
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Define the LED modes...
#define MODE_DEFAULT  0
#define MODE_HOVER    1
#define MODE_ASCEND   2
#define MODE_DESCEND  3
#define NUM_MODES     4

// Define some easy-to-use colors...
int dark = pixels.Color(0,0,0);

int dimWhite = pixels.Color(64,64,64);
int medWhite = pixels.Color(128,128,128);
int brtWhite = pixels.Color(255,255,255);

int dimRed = pixels.Color(64,0,0);
int medRed = pixels.Color(128,0,0);
int brtRed = pixels.Color(255,0,0);

int dimGreen = pixels.Color(0,64,0);
int medGreen = pixels.Color(0,128,0);
int brtGreen = pixels.Color(0,255,0);

int dimBlue = pixels.Color(0,0,64);
int medBlue = pixels.Color(0,0,128);
int brtBlue = pixels.Color(0,0,255);

int dimYellow = pixels.Color(48,32,0);
int medYellow = pixels.Color(128,128,0);
int brtYellow = pixels.Color(192,192,0);

// Accelerometer Inputs
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;

// Calibration values
// Find these for your particular 335 by running the calibration code found at
// https://learn.adafruit.com/adafruit-analog-accelerometer-breakouts
int xRawMin = 407;
int xRawMax = 609;

int yRawMin = 407;
int yRawMax = 608;

int zRawMin = 408;
int zRawMax = 618;

const int sampleSize = 10;

// Control Input
#define CONTROL_PIN  2

#define CTRL_OFF     0
#define CTRL_ON      1
#define CTRL_AUTO    2

/*
  LED Functions
*/
  
// Set all the lights to the given color
void setAll( int color ) {
  for ( int i=0; i < NUMPIXELS; i++ ){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, color);    
    // This sends the updated pixel color to the hardware.
    pixels.show();
  }
}

// Play normal aircraft running lights (white in front, yellow behind, red to port and green to starboard)
void playDefault () {
  // set the light colors...
  pixels.setPixelColor(0, dimWhite);
  pixels.setPixelColor(1, dimWhite);
  pixels.setPixelColor(2, dimGreen);
  pixels.setPixelColor(3, dimYellow);
  pixels.setPixelColor(4, dimYellow);
  pixels.setPixelColor(5, dimRed);
  pixels.show();
  delay(2000);
  // blink the side lights...
  pixels.setPixelColor(2, brtGreen);
  pixels.setPixelColor(5, brtRed);
  pixels.show();
  delay(50);
}

// Play a UFO-like effect of all dim white lights with one bright strobe going around
void playHover () {
  for ( int i=0; i < NUMPIXELS; i++ ) {
    setAll( dimWhite );
    pixels.setPixelColor(i, brtWhite);
    pixels.show();
    delay(50);
  }
}

// Play all dim white lights that gradually brighten
void playAscend () {
  for ( int i = 0; i < 255; i++ ) {
    setAll( pixels.Color(i,i,i));
  }
  delay(250);
}

// Play all bright white lights that gradually dim
void playDescend () {
  for ( int i = 255; i > 0; i-- ) {
    setAll( pixels.Color(i,i,i) );
  }
  delay(250);
}

// Play all white with a brief, bright red strobe
void playRedStrobe () {
  setAll( dimWhite );
  delay(500);
  setAll(brtRed);
  delay(50);
}

// Read the accelerometer and play the appropriate pattern
void playAuto() {
  // Read the accelerometer...
  int ledMode = readAccel();
  
  // RunPlay the appropriate pattern...
  switch ( ledMode ) {
    case MODE_DEFAULT:
      playDefault();
      break;
    case MODE_HOVER:
      playHover();
      break;
    case MODE_ASCEND:
      playAscend();
      break;   
    case MODE_DESCEND:
      playDescend();
      break;
  }
}

/*
  Acellerometer Functions
*/

// Read an axis and return the sampled value
int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
    delay(1);
  }
  return reading/sampleSize;
}

// Read the accelerometer and return the flight mode
int readAccel() {
  // Read the axes...
  int xRaw = ReadAxis(xInput);
  int yRaw = ReadAxis(yInput);
  int zRaw = ReadAxis(zInput);
  
  // Convert raw values to 'milli-Gs"
  long xScaled = map(xRaw, xRawMin, xRawMax, -1000, 1000);
  long yScaled = map(yRaw, yRawMin, yRawMax, -1000, 1000);
  long zScaled = map(zRaw, zRawMin, zRawMax, -1000, 1000);

  // re-scale to fractional Gs
  float xAccel = xScaled / 1000.0;
  float yAccel = yScaled / 1000.0;
  float zAccel = zScaled / 1000.0;

  // Check for lateral motion...
  if ( xAccel < -0.2 or xAccel > 0.2 or yAccel < -0.2 or yAccel > 0.2 ) {
    return MODE_DEFAULT;
  }
  // Check for upward motion..
  else if ( zAccel > 0.99 ) {
    return MODE_ASCEND;
  }
  // Check for downward motion...
  else if ( zAccel < 0.85 ) {
    return MODE_DESCEND;
  }
  // Must be sitting still, then...
  else {
    return MODE_HOVER;
  }
}    

// Read the control pin and return the control mode
int readControl() { 
  int ctrlPin = pulseIn(CONTROL_PIN, HIGH);

  if ( ctrlPin < 1200 ) {
    return CTRL_OFF;
  }
  else if ( ctrlPin > 1200 and ctrlPin < 1600 ) {
    return CTRL_ON;
  }
  else {
    return CTRL_AUTO;
  }
}

/*
  Main Program
*/

// Set everything up
void setup() {
  // Set the analog reference to external before calling analogRead()...
  analogReference(EXTERNAL);
  
  // Set up the control pin...
  pinMode(CONTROL_PIN, INPUT_PULLUP);

  // Start the pixels on white...
  pixels.begin();
  setAll( dimWhite );
}

// Do the main loop
void loop() {
  
  // Read the control pin...
  int ctrlPin = readControl();

  switch ( ctrlPin ) {
    case CTRL_OFF:
      setAll(dark);
      break;
    case CTRL_ON:
      playRedStrobe();
      break;
    case CTRL_AUTO:
      playAuto();
      break;
  }      
}
