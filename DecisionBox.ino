#include <SimpleTimer.h>
#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN 2        // pin IO used for pilot the Neopixel led
#define BUTTON_PIN 12      // pin IO used for the button
#define ANALOG_PIN 0       // pin IO used for obtain the number to initialize the pseudo-random sequence
#define PIXEL_COUNT 5      // number Neopixel led present in strip
#define NUMBER_PIXEL_ON 5  // number of pixels to be turned on

// numbers to define the colors of the led strip
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define OFF 0

int brightness = 30;            // brightness of the leds (min=0,max=255)
int delayChangeColor = 250;      // delay between one color and another 
int timeElaboration = 3000;      // time to process the answer
int timeReadResponse = 2000;     // response display time
int timeWaitingResponse = 1500;  // waiting time before the answer
long timerId;                    // variable to assign the timer id
int buttonState = 0;             // variable for reading the pushbutton status
int input = 0;                   // variable to store the value read

// create the object strip of type Adafruit_NeoPixel
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ400);
// the timer object
SimpleTimer timer;

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  randomSeed(analogRead(ANALOG_PIN));
  // Initialize all pixels to 'off
  strip.begin();
  strip.setBrightness(brightness);
  strip.show();
}

void loop() {
  timer.run();
  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) {
    if(!timer.isEnabled(timerId)){
      // start the repetition of colors (processing)
      timerId = timer.setInterval(delayChangeColor*3,elaboration);
      // set the call to the 'stopElaboration' function after 'timeElaboration' (3 seconds) 
      timer.setTimeout(timeElaboration,stopElaboration);
    }
  }
}

// set the three colors on the led strip in sequence
void elaboration(){
    setColor(COLOR_RED);
    delay(delayChangeColor);
    setColor(COLOR_GREEN);
    delay(delayChangeColor);
    setColor(COLOR_YELLOW);
    delay(delayChangeColor);
}

// stop processing and display of the answer
void stopElaboration(){
  timer.disable(timerId);
  setColor(OFF);
  delay(timeWaitingResponse);
  // read the input pin
  input = analogRead(ANALOG_PIN);
  setResponse(input);
  setColor(OFF);
}

// set the led strip with the response obtained randomly with the value of the digital input
void setResponse(int input){
  setColor(random(1,4));
  delay(timeReadResponse);
}

// set the color to the single led based on the entire value passed
void setColor(int color){
  switch(color){
    case COLOR_RED:
      setColorStrip(255,0,0,NUMBER_PIXEL_ON);
      break;
    case COLOR_GREEN:
      setColorStrip(0,255,0,NUMBER_PIXEL_ON);
      break;
    case COLOR_YELLOW:
      setColorStrip(255,255,0,NUMBER_PIXEL_ON);
      break;
    case OFF:
      setColorStrip(0,0,0,NUMBER_PIXEL_ON);
      break;
  }
}

// set the color chosen for all the LEDs
void setColorStrip(int r, int g, int b, int numberPixel){
  for(int i=0; i<numberPixel; i++){
    strip.setPixelColor(i,strip.Color(r,g,b));
  }
  strip.show();
}
