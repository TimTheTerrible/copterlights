#include <Adafruit_NeoPixel.h>

#define PIN            6

#define NUMPIXELS      8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define MODE_DEFAULT  0
#define MODE_HOVER    1
#define MODE_ASCEND   2
#define MODE_DESCEND  3
#define NUM_MODES     4

int mode;
int mark;

// Define some easy-to-use colors...
int dark = pixels.Color(0,0,0);

int dimWhite = pixels.Color(64,64,64);
int medWhite = pixels.Color(128,128,128);
int brtWhite = pixels.Color(196,196,196);

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
int brtYellow = pixels.Color(196,192,0);

// Accelerometer Stuff
const int xInput = A0;
const int yInput = A1;
const int zInput = A2;
const int buttonPin = 2;

int xRawMin = 407;
int xRawMax = 609;

int yRawMin = 407;
int yRawMax = 608;

int zRawMin = 408;
int zRawMax = 618;

const int sampleSize = 10;

// Utility Functions
void setAll( int color ) {
  for ( int i=0; i < NUMPIXELS; i++ ){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, color);    
    pixels.show(); // This sends the updated pixel color to the hardware.
  }
}

void playDefault () {
  // set the light colors...
  pixels.setPixelColor(0, dimWhite);
  pixels.setPixelColor(1, dimWhite);
  pixels.setPixelColor(2, dimRed);
  pixels.setPixelColor(3, dimYellow);
  pixels.setPixelColor(4, dimYellow);
  pixels.setPixelColor(5, dimGreen);
  pixels.show();
  delay(2000);
  // blink the side lights...
  pixels.setPixelColor(2, brtRed);
  pixels.setPixelColor(5, brtGreen);
  pixels.show();
  delay(50);
}

void playHover () {
  for ( int i=0; i < NUMPIXELS; i++ ){
    setAll( dimWhite );
    pixels.setPixelColor(i, brtWhite);
    pixels.show();
    delay(50);
  }
}

void playAscend () {
  for ( int i = 0; i < 255; i++ ) {
    setAll( pixels.Color(i,i,i));
  }
  delay(250);
}

void playDescend () {
  for ( int i = 255; i > 0; i-- ) {
    setAll( pixels.Color(i,i,i) );
  }
  delay(250);
}

int ReadAxis(int axisPin)
{
  long reading = 0;
  analogRead(axisPin);
  delay(1);
  for (int i = 0; i < sampleSize; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/sampleSize;
}

void readAccel() {
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

  Serial.print("Accel ");
  Serial.print(xAccel);
  Serial.print("G, ");
  Serial.print(yAccel);
  Serial.print("G, ");
  Serial.print(zAccel);
  Serial.println("G");  
  
  if ( xAccel < -0.2 or xAccel > 0.2 or yAccel < -0.2 or yAccel > 0.2 ) {
    Serial.println("Setting mode to MODE_DEFAULT");  
    mode = MODE_DEFAULT;
  }
  else if ( zAccel > 0.98 ) {
    Serial.println("Setting mode to MODE_ASCEND");  
    mode = MODE_ASCEND;
  }
  else if ( zAccel < 0.85 ) {
    Serial.println("Setting mode to MODE_DESCEND");  
    mode = MODE_DESCEND;
  }
  else {
    Serial.println("Setting mode to MODE_HOVER");  
    mode = MODE_HOVER;
  }
}    

void setup() {
  // set the analog reference to external before calling analogRead()...
  analogReference(EXTERNAL);
  
  // Start the pixels on white...
  pixels.begin();
  setAll( dimWhite );

  // Set the starting mode to Hover...
  mode = MODE_HOVER;
  
  pinMode(2, INPUT);
}

// Main Program
void loop() {
  // Read the control pin...
  int cmd = pulseIn(2, HIGH);

  if ( cmd < 1200 ) {
    setAll(dark);
  }
  else if ( cmd > 1200 and cmd < 1600 ) {
    setAll(dimWhite);
  }
  else {
    // Read the accelerometer...
    readAccel();
    
    // Run the approriate pattern...
    switch ( mode ) {
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
  
}
