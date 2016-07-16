/*
 +-------------------------------------------------+
 |:.      OpenWest 2016 Electronic Badges        .:|
 +-------------------------------------------------+
  \|                                             |/
   |  Code Contributors / Monkeys:               |
   |   d3c4f       d3c4f@sausage.land            |
   |     - Hardware Abstraction                  |
   |     - Text Library                          |
   |     - Nibble                                |
   |                                             |
   |  Hardware Engineering:                      |
   |   devino      devino@sausage.land           |
   |                                             |
  /|                                             |\
 +-+--------+---------------------------+--------+-+
 |:         |   Files and Information:  |         :|
 |          |  github.com/thetransistor |          |
 |          |     thetransistor.com     |          |
 |:.        |:.      openwest.org     .:|        .:|
 +----------+---------------------------+----------+
*/


// Include the librarys, and instantiate a global instance of badge
#include "ow10000-hardware.h"
#include "ow10000-text.h"
#include "ow10000-nibble.h"
#include "ow10000-tetris.h"
#include <EEPROM.h>

OW10000HAL badge;
OW10000_text scrollingText(&badge);
OW10000_tetris tetrisGame(&badge);
OW10000_nibble nibbleGame(&badge);

const unsigned int EEPROM_BYTES = 1024;      // Number of EEPROM bytes
const unsigned int E_SIGNATURE = 0;          // Offset for Signature
const unsigned int E_VERSION = 1;            // Offset for Version
const unsigned int E_BRIGHTNESS = 2;         // Offset for brightness setting
const unsigned int E_TEXT1_LENGTH = 3;       // Offset for number of Characters stored for Text1
const unsigned int E_TEXT1_START = 4;        // Offset for First Character of Text1
const unsigned int E_TEXT1_END = 260;        // Offset for Last Character of Text1
const unsigned int E_TEXT2_LENGTH = 261;     // Offset for Number of Characters stored for Text2
const unsigned int E_TEXT2_START = 262;      // Offset for First Character of Text2
const unsigned int E_TEXT2_END = 518;        // Offset for Last Character of Text2

const unsigned int SIGNATURE = 42;       // Signature
const unsigned int VERSION = 2;          // Version

String text1;  // String for button1 / default
String text2;  // String for button2
String text_temp;  // String for misc

int textSelectionPosition; // The selection location for the text mode
bool resetText;

// Initialize Stuffs. Mostly the timer interupt. Badge hardware is initialized in the badge library.
void setup() {
	// TIMER1 SETUP
	noInterrupts();           // disable all interrupts
	TCCR1A = 0;               // set entire register to 0
	TCCR1B = 0;               // set entire register to 0
	TCNT1 = 0;                // reset the 16-bit timer counter
	OCR1A = 180;               // compare match register
	TCCR1B |= (1 << WGM12);   // CTC mode (Clear timer on compare match)
	//TCCR1B |= (1 << CS10);    // CS10 and CS11 set = 64 prescaler
	TCCR1B |= (1 << CS11);    // CS11 only set = 8 prescaler
	TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
	interrupts();

	scrollingText.setBounce(false);
	scrollingText.setDiminsions(0,4,15);
	
	setupEEPROM();
	
	scrollingText.setTextString(text1);

	badge.clear();
	textSelectionPosition = 0;
	resetText = false;
}


// Read in settings from NVM to RAM
// and initialize if needed
void setupEEPROM(){
	byte temp = 0;
	
	if(EEPROM.read(E_SIGNATURE) == SIGNATURE && EEPROM.read(E_VERSION) == VERSION){
		// (Badge is all up to date)
		// It's a UNIX system! I know this!
		// Initialize
		text1 = "";
		text2 = "";
		
		// Read in Frame Drop / Brightness / Battery Save / whatever you want to call it
		badge.setDropFrames(EEPROM.read(E_BRIGHTNESS));

		// Read in Text 1
		temp = EEPROM.read(E_TEXT1_LENGTH);
		for(int x = 0; x < temp; x++){
			text1 += (char) EEPROM.read(E_TEXT1_START + x);
		}

		// Read in Text 2
		temp = EEPROM.read(E_TEXT2_LENGTH);
		for(int x = 0; x < temp; x++){
			text2 += (char) EEPROM.read(E_TEXT2_START + x);
		}
	} else {
		// New Badge, Setup
		text1 = "OpenWest 2016! ";
		text2 = "github.com/theTransistor ";
		
		// Write the current version / signature
		EEPROM.write(E_SIGNATURE, (byte) SIGNATURE);
		EEPROM.write(E_VERSION, (byte) VERSION);

		// Save the brightness
		badge.setDropFrames(0b0000010);
		EEPROM.write(E_BRIGHTNESS, (byte) 0b00000010);
		
		// Write the default text strings out
		saveText1();
		saveText2();
	}
}


// Save text1 to the EEPROM
void saveText1(){
	unsigned int length = text1.length();
	
	if(length > E_TEXT1_END - E_TEXT1_START){
		length = E_TEXT1_END - E_TEXT1_START;
	} 
	EEPROM.write(E_TEXT1_LENGTH, (byte) length);
	
	for(int x = 0; x < length; x++){
		EEPROM.write(E_TEXT1_START + x, (byte) text1[x]);
	}
}


// Save text2 to the EEPROM
void saveText2(){
	unsigned int length = text2.length();

	if(length > E_TEXT2_END - E_TEXT2_START){
		length = E_TEXT2_END - E_TEXT2_START;
	} 
	EEPROM.write(E_TEXT2_LENGTH, (byte) length);
	
	for(int x = 0; x < length; x++){
		EEPROM.write(E_TEXT2_START + x, (byte) text2[x]);
	}
}


// For diagnostics only, delete later
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


// Edit a string
String stringEditor(String newText){
	badge.clear();
	badge.setPixel (6,12,1);
	badge.setPixel (7,12,1);
	badge.setPixel (8,12,1);
	badge.setPixel (9,12,1);
	badge.setPixel (10,12,1);
	
	if(newText.length() < 1){
		newText = " ";
	}

	scrollingText.setDiminsions(0,4,15);
	scrollingText.setTextString(newText);
	unsigned int currentCharacter = 0;
	
	scrollingText.jumpToOffset((currentCharacter * 6) + 10);
	
	while(!badge.buttonA_debounce()){
		
		if( badge.buttonB_debounce() ){
			// Delete the current character
			if(newText.length() > 1){
				newText = newText.substring(0,currentCharacter) + newText.substring(currentCharacter+1);
				
				if(currentCharacter > 0){
					currentCharacter --;
				}
				scrollingText.setTextString(newText);
				scrollingText.jumpToOffset((currentCharacter * 6)+10);
			}
		}
		
		// Change Current Character - 1
		if( badge.buttonU_repeat(85) ){
			if((int)newText[currentCharacter] > 32 ){
				newText[currentCharacter] = newText[currentCharacter] - 1;
			} else {
				newText[currentCharacter] = (32 + 95);
			}
			scrollingText.setTextString(newText);
			scrollingText.jumpToOffset((currentCharacter * 6)+10);
		}
		
		// Change Current Character + 1
		if( badge.buttonD_repeat(85) ){
			if((int)newText[currentCharacter] < (32 + 95) ){
				newText[currentCharacter] = newText[currentCharacter] + 1;
			} else {
				newText[currentCharacter] = 32;
			}
			scrollingText.setTextString(newText);
			scrollingText.jumpToOffset((currentCharacter * 6)+10);
		}

		// Previous Character in String
		if( badge.buttonL_debounce(10) ){
			if(currentCharacter > 0){
				currentCharacter--;
			} else {
				currentCharacter = 0;
			}
			scrollingText.setTextString(newText);
			scrollingText.jumpToOffset((currentCharacter * 6)+10);
		}

		// Next Character in String
		if( badge.buttonR_debounce(10) ){
			currentCharacter++;
			
			// Create a new Character, as needed
			if( currentCharacter >= newText.length()){
				newText += " ";
				//currentCharacter = newText.length();
			}
			scrollingText.setTextString(newText);
			scrollingText.jumpToOffset((currentCharacter * 6)+10);
		}
	}
	
	badge.clear();
	return newText;
}

void displayBatteryLevel() {
	text_temp =  "B:" + String(badge.battery_level())  + "%  ";
	scrollingText.setTextString(text_temp);
}

void handleScrollingText() {
	if (badge.buttonA_debounce()) {
		textSelectionPosition--;
		resetText = true;
	}
	if (badge.buttonB_debounce()) {
		textSelectionPosition++;
		resetText = true;
	}

	if (textSelectionPosition < 0) {
		textSelectionPosition = 0;
	}
	if (textSelectionPosition > 3) {
		textSelectionPosition = 3;
	}

	if (resetText) {
		badge.clear();
		switch (textSelectionPosition) {
			case 0:
				scrollingText.setTextString(text1);
				break;
			case 1:
				scrollingText.setTextString(text2);
				break;
			case 2:			
				scrollingText.setTextString(text1 + "- " + text2 + "- ");
				break;
			case 3:
			default:
				displayBatteryLevel();
				break;
		}
		resetText = false;
	}
}

// Infinity...
void loop() {
	scrollingText.update();
	handleScrollingText();
	
	if(badge.buttonD_debounce()){
		badge.clear();
		displayBatteryLevel();	
	}
	
	if(badge.buttonR()){
		scrollingText.setScrollRate(35);  // Fast Scroll Rate
	} else {
		scrollingText.setScrollRate(65);  // Normal Scroll Rate
	}
	
	// Ahh yeah, Menutime!
	if(badge.buttonAB_debounce(200)) {
		switch(menu()){
			case(1):    // Customize Text 1
				text1 = stringEditor(text1);
				saveText1();
				scrollingText.setTextString(text1);
				textSelectionPosition = 0;
				break;
			
			case(2):    // Clear Text 1
				text1 = "";
				scrollingText.setTextString(text1);
				textSelectionPosition = 0;
				break;
			
			case(3):    // Customize Text 2
				text2 = stringEditor(text2);
				saveText2();
				scrollingText.setTextString(text2);
				textSelectionPosition = 1;
				break;
			
			case(4):    // Clear Text 2
				text2 = "";
				scrollingText.setTextString(text2);
				textSelectionPosition = 1;
				break;
			
			case(5):    // Play Tetris
				//tetrisGame.play();
				scrollingText.setTextString("coming soon to github.com/thetransistor update your badge! :P ");
				//scrollingText.setTextString(text1);
				break;	
				
			case(6):    // Play Nibble
				nibbleGame.play();  // Play the nibbles!
				scrollingText.setTextString(text1);
				break;
				
			case(7):    // Set Brightness
				setBadgeBright();	
				scrollingText.setTextString(text1);
				break;

			case(0):    // Back to default
			default:
				scrollingText.setTextString(text1);
				break;
			
		}
		badge.clear();
	}
}


// Menu
// 0 - Exit Menu
// 1 - Customize Text 1 (Default)
// 2 - Clear Text 1
// 3 - Customize Text 2
// 4 - Clear Text 2
// 5 - Play Tetris
// 6 - Play Nibble
// 7 - Set Brightness
unsigned int menu(){
	long tempMillis;
	
	unsigned int currentSelection = 1;
	setMenuText(currentSelection);

	// Display Menu welcome for a bit
	tempMillis = millis();
	scrollingText.setBounce(false);
	scrollingText.setDiminsions(0,4,15);

	// Main Menu Loop
	while(!badge.buttonA_debounce()){
		scrollingText.update();
		
		// Move up the menu
		if(badge.buttonU_debounce(85)){
			if(currentSelection<=0){
				currentSelection = 7;
			} else {
				currentSelection--;
			}
			setMenuText(currentSelection);
		}
		
		// Move down the menu
		if(badge.buttonD_debounce(85)){
			if(currentSelection >= 7){
				currentSelection = 0;
			} else {
				currentSelection++;
			}
			setMenuText(currentSelection);
		}

		if(badge.buttonR()){
			scrollingText.setScrollRate(35);  // Fast Scroll Rate
		} else {
			scrollingText.setScrollRate(65);  // Normal Scroll Rate
		}
	}
	
	badge.clear();
	return currentSelection;
}


// Display the correct menu text
void setMenuText(int textSelection) {
	char menuText1[] = "1) Customize Text1   ";
	char menuText2[] = "2) Clear Text1   ";
	char menuText3[] = "3) Customize Text2   ";
	char menuText4[] = "4) Clear Text2   ";
	char menuText5[] = "5) Tetris   ";
	char menuText6[] = "6) Nibble   ";
	char menuText7[] = "7) Set Brightness   ";
	char menuText0[] = "Exit Menu  ";
	
	switch (textSelection){
		case 1:
			scrollingText.setTextString(menuText1);
			break;
		case 2:
			scrollingText.setTextString(menuText2);
			break;
		case 3:
			scrollingText.setTextString(menuText3);
			break;
		case 4:
			scrollingText.setTextString(menuText4);
			break;
		case 5:
			scrollingText.setTextString(menuText5);
			break;
		case 6:
			scrollingText.setTextString(menuText6);
			break;
		case 7:
			scrollingText.setTextString(menuText7);
			break;
		case 0:
		default:
			scrollingText.setTextString(menuText0);
			break;
	}
}


// Set the badge brightness
void setBadgeBright(){
	bool selectionMade = false;
	unsigned int currentSelection = badge.getDropFrames();
	setBadgeBrightnessDisplay(currentSelection);
	
	while(!selectionMade){
		selectionMade = badge.buttonA_debounce();
		
		// Move up the menu
		if(badge.buttonD_repeat(120)){
			if(currentSelection >= 15){
				currentSelection = 15;
			} else {
				currentSelection++;
			}
			badge.setDropFrames(currentSelection);
			setBadgeBrightnessDisplay(currentSelection);
		}
		
		// Move down the menu
		if(badge.buttonU_repeat(120)){
			if(currentSelection<=0){
				currentSelection = 0;
			} else {
				currentSelection--;
			}
			badge.setDropFrames(currentSelection);
			setBadgeBrightnessDisplay(currentSelection);
		}
	}
	
	// Write to NVM
	EEPROM.write(E_BRIGHTNESS, (byte) currentSelection);
}


// Set the text for the brightness menu
void setBadgeBrightnessDisplay(unsigned int selection){
	selection = 15 - selection;
	badge.clear();
	for (int x = selection; x >= 0; x--){
		badge.frameBuffer[15-x][0] = 0xFFFF;
		badge.frameBuffer[15-x][1] = 0xFFFF;
	}
}


// Timer1 interupt: pushes the framebuffer to drive the display
ISR(TIMER1_COMPA_vect) {
	badge.processFB();
}
