/**
 * Microcontroller Simon
 *
 * A simple Arduino/MSP430 version of the classic game
 * Simon that lives entirely on the microcontroller.
 * 
 * by Salvatore Testa (sal.testa@gmail.com)
 */
// pins corresponding to the LED anode
int leds[] = {8, 9, 10, 11};
// pins corresponding to the button
int buttons[] = {1, 2, 3, 4};
int NUM_BUTTONS = sizeof(leds)/sizeof(int);
// longest game possible
const int MAX_PATTERNS = 100;
int pattern[MAX_PATTERNS];
// last assigned pattern index
int finalIndex;
// pattern index being guessed
int currentIndex;
// a button's possible states
enum button {
	DOWN,
	UP
} button = DOWN;

/**
 * Initialize all the pins and play the opening sequence
 */
void setup() {
	randomSeed(analogRead(0));
	for(int i = 0; i < NUM_BUTTONS; i++) {
		pinMode(leds[i], OUTPUT);     
		pinMode(buttons[i], INPUT_PULLUP);       
	}
	start();
}

void start() {
	resetGame();
	intro();
	showPattern();
}

void loop() {
	for(int i = 0; i < NUM_BUTTONS; i++) {
		int readVal = digitalRead(buttons[i]);
		// Button not pressed
		if(readVal == HIGH){
			off(i);
			button = UP;
		// Button pressed
		} else if(readVal == LOW) {
			on(i);
			if(button == UP){
			 	button = DOWN;
			 	/**
			 	 * Give the player time to remove their finger.
			 	 */
				delay(1000);
			 	guess(i);
			}
		}   
	}
}

void guess(int button) {
	// correct guess
	if(button == pattern[currentIndex]){
	    currentIndex++;
		if(currentIndex > finalIndex) {
			currentIndex = 0;
			finalIndex++;
			showPattern();
		}
	// wrong guess
	} else {
		start(); // start over
	}
}

void intro() {
	// Cycle through all the LEDS
	for(int j=0; j<3; j++){
		for(int i = 0; i < NUM_BUTTONS; i++) {
			blink(i, 100);   
		}
	}
	// Turn them all on
	for(int i = 0; i < NUM_BUTTONS; i++) {
		on(i);   
	}
	delay(1000);
	for(int i = 0; i < NUM_BUTTONS; i++) {
		off(i);   
	}

}

/**
 * Show the Simon sequence
 */
void showPattern() {
	for(int i = 0; i < NUM_BUTTONS; i++) {
		off(i);   
	}
	delay(1000);
	for(int i=0; i <= finalIndex; i++){
	    blink(pattern[i]);
	    delay(1000);
	}
}

/**
 * Re-initialize all the values
 */
void resetGame() {
	finalIndex = 0;
	currentIndex = 0;
	for(int i=0; i<MAX_PATTERNS; i++){
	    pattern[i] = random(0, NUM_BUTTONS);
	}
}

void blink(int led) {
	blink(led, 1000);
}

void on(int led) {
	digitalWrite(leds[led], HIGH);
}

void off(int led) {
	digitalWrite(leds[led], LOW);
}

void blink(int led, int delaySpeed) {
	on(led);
	delay(delaySpeed);
	off(led);
}
