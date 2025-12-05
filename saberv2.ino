#include<Adafruit_NeoPixel.h>

#define ledPin 6
#define numLeds 144

// === BUTTUONS ====
#define buttonPin 2
#define button2Pin 3

//=== Debounce Parameters 1===
int lastbuttonState = HIGH;
unsigned int lastDebounce = 0;
unsigned int debounceDelay = 150;

//=== Debounce Parameters 2===
int lastbutton2State = HIGH;
unsigned int lastDebounce2 = 0;
unsigned int debounceDelay2 = 150;


// === Delay per pixel for animation ===
static unsigned long pixelDelay = 0;


Adafruit_NeoPixel pixels(numLeds, ledPin, NEO_GRB + NEO_KHZ800); // Creating "pixels" object



// ===  COLOUR CONTROL ===
uint32_t red = pixels.Color(100, 0,0);
uint32_t green = pixels.Color(0,100,0);
uint32_t blue = pixels.Color(0,0,100);
uint32_t cyan = pixels.Color(0,20,100);

int colorLimit = 4; // number of colours
// === END of COLOUR CONTROL ===





static int colorCount = 0;
static int currentPixel = 0;
uint32_t bladeColor;

enum State {
OFF,
IGNITION,
RETRACTION};

State currentState, nextState;



unsigned long stateStartTime = 0;
unsigned long bladeDelay = 2;



void setup()
{
pinMode(buttonPin, INPUT_PULLUP);
pinMode(button2Pin, INPUT_PULLUP);

pixels.begin();
pixels.clear();
pixels.show();
delay(10);
bladeColor = cyan;
currentState = OFF;

}


void loop()
{
int currentButton = digitalRead(buttonPin);
int currentButton2 = digitalRead(button2Pin);

// ====== Debounce Logic for Colour button - Button 2 ======

if (currentButton2 != lastbutton2State)
{
if(currentButton2 == LOW && lastbutton2State == HIGH)
{
if(millis() - lastDebounce2 > debounceDelay2)
{
if (currentPixel == numLeds){
colorChange();
pixels.show();
colorCount++;
}


if(colorCount >= colorLimit)
colorCount = 0;

lastDebounce2 = millis();

}
}
}
// ====== END of Debounce Logic for Colour button - Button 2 ======

// ====== Debounce Logic for State change button - Button 2 ======
if (currentButton != lastbuttonState)
{
if(currentButton == LOW && lastbuttonState == HIGH)
{
if(millis() - lastDebounce > debounceDelay)
{
currentState = nextState;
lastDebounce = millis();
}
}
}

lastbuttonState = currentButton;
lastbutton2State = currentButton2;
// ====== END Debounce Logic for State change button - Button 2 ======


// ====== Next State Logic ======
switch(currentState)
{
case OFF:
nextState = IGNITION;
break;

case IGNITION:
nextState = RETRACTION;
break;

case RETRACTION:
nextState = OFF;
break;
}

// ====== END of Next State Logic ======


// ====== Output Logic ======
switch(currentState)
{
case OFF:
currentPixel = 0;
pixels.clear();
pixels.show();
break;


case IGNITION:
if(currentPixel < numLeds)
{
if(millis() - pixelDelay >= bladeDelay)
{
pixels.setPixelColor(currentPixel, bladeColor);
pixels.show();
currentPixel++;
pixelDelay = millis();
}
}
break;

case RETRACTION:
if(currentPixel >0)
{
if(millis() - pixelDelay >= bladeDelay)
{
pixels.setPixelColor(currentPixel -1, 0,0,0);
pixels.show();
currentPixel--;
pixelDelay = millis();

}
}
else
currentState = OFF;
break;

}

// ====== END of Output Logic ======

} // Void loop ends


void colorChange()  // Colour change function
{
switch(colorCount)
{

case 0: bladeColor = red;
break;

case 1: bladeColor = blue;
break;

case 2: bladeColor = green;
break;

case 3: bladeColor = cyan;
break;

default: bladeColor = cyan;
break;

}


} // END OF Colour Change Function
