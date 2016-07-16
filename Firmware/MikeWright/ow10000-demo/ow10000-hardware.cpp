/*
  Filename: ow1000-hardware.cpp
  Purpose: Hardware Library
  Created: June 29, 2016
*/

#include "Arduino.h"
#include "ow10000-hardware.h"
#include "bitmacros.h"

// Initialize Hardware (default constructor)
OW10000HAL::OW10000HAL()
{
	scanRetard = 0;
	currentCol = 0;
	currentIteration = 0;
	grayscale = true;
	lastButtonA = 0;
	lastButtonB = 0;
	dropFrameCounter = 0;

	clear(); // Clear the frameBuffer

	// Setup the Digital Inputs
	DDRD &= 0b11010000; // Set Input : D0,D1,D2,D3,D5
	DDRE &=  0b10111111; // Set Input : E6
	PORTD |= 0b00101111; // Set Pull-Up : D0,D1,D2,D3,D5
	PORTE |= 0b01000000; // Set Pull-Up : E6

	// Setup Analog Input : F0
	DDRF &= 0b00000001;

	// Setup Digital Outputs
	DDRB |= 0b01110000; // Set Output : B4,B5,B6
	DDRC |= 0b11000000; // Set Output : C6,C7
	DDRD |= 0b11010000; // Set Output : D4,D6,D7
	DDRF |= 0b11000000; // Set Output : F6,F7

	// Initialize the Shift Registers ==== START ====
	
	// Set OE/ High (Enable -> Inactive) : B4 & F6
	BIT_SET(PORTB, 4);
	BIT_SET(PORTF, 6);

	// Set the Resets to High ( High==No Reset ;) C6,D4)
	BIT_SET(PORTC, 6);
	BIT_SET(PORTD, 4);

	// Set Data, Clock, Strobe Low : B5,B6,C7,D6,D7,F7
	BIT_CLR(PORTB, 5);
	BIT_CLR(PORTB, 6);
	BIT_CLR(PORTC, 7);
	BIT_CLR(PORTD, 6);
	BIT_CLR(PORTD, 7);
	BIT_CLR(PORTF, 7);

	// Reset the Shift Registers : C6,D4 (Active Low)
	BIT_CLR(PORTC, 6);
	BIT_CLR(PORTD, 4);
	BIT_SET(PORTC, 6);
	BIT_SET(PORTD, 4);

	// Set OE/ Low (Enable -> Active) : B4 & F6
	BIT_CLR(PORTB, 4);
	BIT_CLR(PORTF, 6);

	// Initialize the Shift Registers ==== END ====
}


// Return the current Battery Level as a percent (0-100)
// Could be updated. Batteries are linear, yo
unsigned int OW10000HAL::battery_level() {
	int curReading = analogRead(BATTERY_VOLTS);

	Serial.println(curReading);

	if(curReading <= BATTERY_ZERO) return 0;
	if(curReading >= BATTERY_FULL) return 100;

	float curPercent = ( (float)(curReading - BATTERY_ZERO) / (float)(BATTERY_FULL - BATTERY_ZERO) ) * 100.00;

	return ((unsigned int)curPercent);
}


// Return the state of Button A
// true if button A is pressed (top button, D0)
bool OW10000HAL::buttonA() {
	return(!BIT_CHECK(PIND,0));
}


// Return the state of Button B
// true if button B is pressed (bottom button, D1)
bool OW10000HAL::buttonB() {
	return(!BIT_CHECK(PIND,1));
}


// Return the state of Button A AND B
// true if button A AND B are pressed (D0 && D1)
bool OW10000HAL::buttonAB() {
	return((!BIT_CHECK(PIND,0)) && (!BIT_CHECK(PIND,1)));
}


// Return the state of the Up Button
// true if button is pressed (D5)
bool OW10000HAL::buttonU() {
	return(!BIT_CHECK(PIND,5));
}


// Return the state of the Down Button
// true if button is pressed (D2)
bool OW10000HAL::buttonD() {
	return(!BIT_CHECK(PIND,2));
}


// Return the state of the Left Button
// true if button is pressed (E6)
bool OW10000HAL::buttonL() {
	return(!BIT_CHECK(PINE,6));
}


// Return the state of the Right Button
// true if button is pressed (D3)
bool OW10000HAL::buttonR() {
	return(!BIT_CHECK(PIND,3));
}


// Helper funtion for Debounced, Repeating Input Reads
bool OW10000HAL::repeat(bool current_value, long & lastVar, unsigned int dtime) {
	if(current_value){
		if(lastVar > 0){
			if(millis() > lastVar + dtime) {
				lastVar = millis();
				return true;
			} else {
				return false;
			}
		} else {
			lastVar = millis();
			return false;
		}
	} else {
		lastVar = 0; // Reset / Stage for next press
		return false;
	}
}


// Returns True every [dtime] ms that button is held (A -> D0)
bool OW10000HAL::buttonA_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PIND,0), lastButtonA, dtime);
}


// Returns True every [dtime] ms that button is held (B -> D1)
bool OW10000HAL::buttonB_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PIND,1), lastButtonB, dtime);
}


// Returns True every [dtime] ms that button is held (A -> D0 and B -> D1)
bool OW10000HAL::buttonAB_repeat(unsigned int dtime) {
	return OW10000HAL::repeat((!BIT_CHECK(PIND,0)) && (!BIT_CHECK(PIND,1)), lastButtonAB, dtime);
}


// Returns True every [dtime] ms that button is held (Up -> D5)
bool OW10000HAL::buttonU_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PIND,5), lastButtonU, dtime);
}


// Returns True every [dtime] ms that button is held (Down -> D2)
bool OW10000HAL::buttonD_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PIND,2), lastButtonD, dtime);
}


// Returns True every [dtime] ms that button is held (Left -> E6)
bool OW10000HAL::buttonL_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PINE,6), lastButtonL, dtime);
}


// Returns True every [dtime] ms that button is held (Right -> D3)
bool OW10000HAL::buttonR_repeat(unsigned int dtime) {
	return OW10000HAL::repeat(!BIT_CHECK(PIND,3), lastButtonR, dtime);
}


// Helper funtion for Debounced Input Reads
bool OW10000HAL::debounce(bool current_value, long & lastVar, unsigned int dtime) {
	if(current_value){
		if(lastVar > 0){
			if(millis() > lastVar + dtime) {
				lastVar = 0; // Stop reporting button pressed, until button is reset
				return true;
			} else {
				return false;
			}
		} else {
			if(lastVar == -1) lastVar = millis(); // Wait for reset, before counting again...
			return false;
		}
	} else {
		lastVar = -1; // Reset / Stage for next press
		return false;
	}
}


// Return the state of Button A (D0)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonA_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PIND,0), lastButtonA, dtime);
}


// Return the state of Button B (D1)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonB_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PIND,1), lastButtonB, dtime);
}


// Return the state of A AND B (D0,D1)
// true if buttons are pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonAB_debounce(unsigned int dtime) {
	return OW10000HAL::debounce((!BIT_CHECK(PIND,0)) && (!BIT_CHECK(PIND,1)), lastButtonAB, dtime);
}


// Return the state of Up (D5)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonU_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PIND,5), lastButtonU, dtime);
}


// Return the state of Down (D2)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonD_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PIND,2), lastButtonD, dtime);
}


// Return the state of Left (E6)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonL_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PINE,6), lastButtonL, dtime);
}


// Return the state of Right (D3)
// true if button is pressed for more then DEBOUNCEMILLIS
bool OW10000HAL::buttonR_debounce(unsigned int dtime) {
	return OW10000HAL::debounce(!BIT_CHECK(PIND,3), lastButtonR, dtime);
}


// Set an individual pixel
void OW10000HAL::setPixel(unsigned int x, unsigned int y, unsigned int brightness) {
	if(x < 16 && y < 16 && brightness < 4){
		switch(brightness){
			case (1): // 33% brightness
				BIT_SET(frameBuffer[y][0], x);
				BIT_CLR(frameBuffer[y][1], x);
				break;
			case (2): // 66% brightness
				BIT_CLR(frameBuffer[y][0], x);
				BIT_SET(frameBuffer[y][1], x);
				break;
			case(3): // 100% brightness
				BIT_SET(frameBuffer[y][0], x);
				BIT_SET(frameBuffer[y][1], x);
				break;
			default: // 0% brightness
				BIT_CLR(frameBuffer[y][0], x);
				BIT_CLR(frameBuffer[y][1], x);
				break;
		}
	}
}


// Returns the value of an individual pixel
unsigned int OW10000HAL::getPixel(unsigned int x, unsigned int y){
	if(x < 16 && y < 16){
		if(BIT_CHECK(frameBuffer[y][0], x) && BIT_CHECK(frameBuffer[y][1], x)) return 3;
		if(BIT_CHECK(frameBuffer[y][0], x)) return 1;
		if(BIT_CHECK(frameBuffer[y][1], x)) return 2;
	}
	return 0;
}


// Set the number of frames to drop (makes overall dimmer, and saves battery)
void OW10000HAL::setDropFrames(unsigned int newDropFrameCounter){
	if(newDropFrameCounter > 15){
		newDropFrameCounter = 15;
	}
	dropFrameCounter = newDropFrameCounter;
}


// Return the dropFrameCounter
unsigned int OW10000HAL::getDropFrames(){
	return dropFrameCounter;
}


// Invert the frameBuffer
void OW10000HAL::invert() {
		frameBuffer[0][0] = frameBuffer[0][0] ^ 0xFFFF; frameBuffer[0][1] = frameBuffer[0][1] ^ 0xFFFF;
		frameBuffer[1][0] = frameBuffer[1][0] ^ 0xFFFF; frameBuffer[1][1] = frameBuffer[1][1] ^ 0xFFFF;
		frameBuffer[2][0] = frameBuffer[2][0] ^ 0xFFFF; frameBuffer[2][1] = frameBuffer[2][1] ^ 0xFFFF;
		frameBuffer[3][0] = frameBuffer[3][0] ^ 0xFFFF; frameBuffer[3][1] = frameBuffer[3][1] ^ 0xFFFF;
		frameBuffer[4][0] = frameBuffer[4][0] ^ 0xFFFF; frameBuffer[4][1] = frameBuffer[4][1] ^ 0xFFFF;
		frameBuffer[5][0] = frameBuffer[5][0] ^ 0xFFFF; frameBuffer[5][1] = frameBuffer[5][1] ^ 0xFFFF;
		frameBuffer[6][0] = frameBuffer[6][0] ^ 0xFFFF; frameBuffer[6][1] = frameBuffer[6][1] ^ 0xFFFF;
		frameBuffer[7][0] = frameBuffer[7][0] ^ 0xFFFF; frameBuffer[7][1] = frameBuffer[7][1] ^ 0xFFFF;
		frameBuffer[8][0] = frameBuffer[8][0] ^ 0xFFFF; frameBuffer[8][1] = frameBuffer[8][1] ^ 0xFFFF;
		frameBuffer[9][0] = frameBuffer[9][0] ^ 0xFFFF; frameBuffer[9][1] = frameBuffer[9][1] ^ 0xFFFF;
		frameBuffer[10][0] = frameBuffer[10][0] ^ 0xFFFF; frameBuffer[10][1] = frameBuffer[10][1] ^ 0xFFFF;
		frameBuffer[11][0] = frameBuffer[11][0] ^ 0xFFFF; frameBuffer[11][1] = frameBuffer[11][1] ^ 0xFFFF;
		frameBuffer[12][0] = frameBuffer[12][0] ^ 0xFFFF; frameBuffer[12][1] = frameBuffer[12][1] ^ 0xFFFF;
		frameBuffer[13][0] = frameBuffer[13][0] ^ 0xFFFF; frameBuffer[13][1] = frameBuffer[13][1] ^ 0xFFFF;
		frameBuffer[14][0] = frameBuffer[14][0] ^ 0xFFFF; frameBuffer[14][1] = frameBuffer[14][1] ^ 0xFFFF;
		frameBuffer[15][0] = frameBuffer[15][0] ^ 0xFFFF; frameBuffer[15][1] = frameBuffer[15][1] ^ 0xFFFF;
}


// Clear the frameBuffer
void OW10000HAL::clear() {
		frameBuffer[0][0] = 0x0000; frameBuffer[0][1] = 0x0000;
		frameBuffer[1][0] = 0x0000; frameBuffer[1][1] = 0x0000;
		frameBuffer[2][0] = 0x0000; frameBuffer[2][1] = 0x0000;
		frameBuffer[3][0] = 0x0000; frameBuffer[3][1] = 0x0000;
		frameBuffer[4][0] = 0x0000; frameBuffer[4][1] = 0x0000;
		frameBuffer[5][0] = 0x0000; frameBuffer[5][1] = 0x0000;
		frameBuffer[6][0] = 0x0000; frameBuffer[6][1] = 0x0000;
		frameBuffer[7][0] = 0x0000; frameBuffer[7][1] = 0x0000;
		frameBuffer[8][0] = 0x0000; frameBuffer[8][1] = 0x0000;
		frameBuffer[9][0] = 0x0000; frameBuffer[9][1] = 0x0000;
		frameBuffer[10][0] = 0x0000; frameBuffer[10][1] = 0x0000;
		frameBuffer[11][0] = 0x0000; frameBuffer[11][1] = 0x0000;
		frameBuffer[12][0] = 0x0000; frameBuffer[12][1] = 0x0000;
		frameBuffer[13][0] = 0x0000; frameBuffer[13][1] = 0x0000;
		frameBuffer[14][0] = 0x0000; frameBuffer[14][1] = 0x0000;
		frameBuffer[15][0] = 0x0000; frameBuffer[15][1] = 0x0000;
}


// Process frameBuffer to the badge
void OW10000HAL::processFB(){

	// Select the row we are manipulating this time
	if(currentCol > 15) {

		// Frame Scan Retarding
		scanRetard++;
		if(scanRetard >= dropFrameCounter) {
			scanRetard = 0;
			BIT_CLR(PORTB, 4); // Set Enable to Low (active)
			BIT_CLR(PORTF, 6); // Set Enable to Low (active)
		} else {
			BIT_SET(PORTB, 4); // Set Enable to High (inactive)
			BIT_CLR(PORTF, 6); // Set Enable to High (inactive)
		}
	
		currentCol = 0;
		BIT_CLR(PORTB, 5); // Set COL DATA Low
	
		// Current Iteration for Shading support
		currentIteration++;
		if(currentIteration > 20) currentIteration = 0;

	} else {
		BIT_SET(PORTB, 5); // Set COL DATA High
	}

	BIT_SET(PORTD, 6); // Set COL CLOCK High
	BIT_CLR(PORTD, 6); // Set COL CLOCK Low

	// Shift in the data for this column
	if(currentIteration < 15 && grayscale) {
		for(int i = 0; i < 16; i++) {
			if(BIT_CHECK(frameBuffer[i][0], currentCol) && BIT_CHECK(frameBuffer[i][1], currentCol)) {
				BIT_SET(PORTB, 6);  // Set ROW DATA High (LED On)
			} else {
				BIT_CLR(PORTB, 6); // Set ROW DATA Low (LED Off)
			}
			BIT_SET(PORTC, 7);  // Set ROW CLOCK High
			BIT_CLR(PORTC, 7); // Set ROW CLOCK Low
		}

	} else if (currentIteration > 16 && grayscale) {
		for(int i = 0; i < 16; i++) {
			if(BIT_CHECK(frameBuffer[i][1], currentCol)) {
				BIT_SET(PORTB, 6);  // Set ROW DATA High (LED On)
			} else {
				BIT_CLR(PORTB, 6); // Set ROW DATA Low (LED Off)
			}
			BIT_SET(PORTC, 7);  // Set ROW CLOCK High
			BIT_CLR(PORTC, 7); // Set ROW CLOCK Low
		}

	} else {
		for(int i = 0; i < 16; i++) {
			if(BIT_CHECK(frameBuffer[i][0], currentCol) || BIT_CHECK(frameBuffer[i][1], currentCol)) {
				BIT_SET(PORTB, 6);  // Set ROW DATA High (LED On)
			} else {
				BIT_CLR(PORTB, 6); // Set ROW DATA Low (LED Off)
			}
			BIT_SET(PORTC, 7);  // Set ROW CLOCK High
			BIT_CLR(PORTC, 7); // Set ROW CLOCK Low
		}
	}

	// Strobe
	BIT_SET(PORTD, 7); // Set COL LATCH (STCP) high
	BIT_CLR(PORTD, 7); // Set COL LATCH (STCP) low
	BIT_SET(PORTF, 7); // Set ROW LATCH (STCP) high
	BIT_CLR(PORTF, 7); // Set ROW LATCH (STCP) low

	currentCol++;
}
