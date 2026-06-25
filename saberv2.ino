#include<Adafruit_NeoPixel.h>

#define ledPin 6 // LED Strip Data pin via the logic shifter
#define numLeds 144 // Strip Led length


#define buttonPin 2 // Push button pin

// ------Debouncing parameters ------
int lastbuttonState = HIGH;
unsigned long lastDebounce = 0; // DO NOT CHANGE
unsigned long debounceDelay = 150; // Time before which click will not be registered in ms
// ------ END of Debouncing Parameters  ------


static int currentPixel = 0; // DO NOT CHANGE
static unsigned long pixelDelay = 0; // DO NOT CHANGE.    Both of these are initial states of time and pixel count for the strip


Adafruit_NeoPixel pixels(numLeds, ledPin, NEO_GRB + NEO_KHZ800);

enum State {
			OFF, // Default state - Blade is off
			IGNITION, // Blade lights up
			RETRACTION}; // Blade animates down
			
State currentState, nextState; 



// unsigned long stateStartTime = 0;    // Did not use it.



unsigned long bladeDelay = 1; // Adjust blade light up and down timing (ms/pixel)



void setup()
{
	pinMode(buttonPin, INPUT_PULLUP);
	
	pixels.begin();
	pixels.clear();
	pixels.show();
	delay(10);
	currentState = OFF;
	
}


void loop()
{
	int currentButton = digitalRead(buttonPin);
	
	if (currentButton != lastbuttonState)   // Debounce logic
	{
		if(currentButton == LOW && lastbuttonState == HIGH) // Only detect falling edge. 
		{
			if(millis() - lastDebounce > debounceDelay)
				{
          
					currentState = nextState;    // Current state logic "sequential"
					lastDebounce = millis();
          
				}
		}
	}
	
	lastbuttonState = currentButton;
	
	switch(currentState)    // Next state logic " Combinational"
	{
		case OFF: 

      currentPixel = 0;
			nextState = IGNITION;
			break;
		
		case IGNITION:
			nextState = RETRACTION;      
			break;
			
		case RETRACTION:      
			nextState = OFF;
			break;
	}
	
	
	switch(currentState)   // Output logic "combinational"
	{
		case OFF:
			pixels.clear();
			pixels.show();
			break;
			
			
		case IGNITION:
        
				if(currentPixel < numLeds)
				{
					if(millis() - pixelDelay >= bladeDelay)
					{
						pixels.setPixelColor(currentPixel, 0,20,40);
						pixels.show();
						currentPixel++;
						pixelDelay = millis();
					}
				}
				break;
			
		case RETRACTION:
      
			if(currentPixel >= 0)
				{
					if(millis() - pixelDelay >= bladeDelay)
					{
						pixels.setPixelColor(currentPixel, 0,0,0);
						pixels.show();
						currentPixel--;
						pixelDelay = millis();
					}
				}	
      if(currentPixel == 0)
        currentState = OFF;  // Force the blade to OFF state, if removed - the blade goes to OFF state only when the button is pressed after Retracting making it look like
                             // button press did not do anything
			break;
			
	}
	
}
