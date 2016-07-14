/*
  Filename: ow10000-text.cpp
  Purpose: Text Library
  Created: June 29, 2016
*/

#include "Arduino.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"
#include "bitmacros.h"

// Default Constructor
OW10000_text::OW10000_text(OW10000HAL* badgeToLinkTo) {
	badge = badgeToLinkTo;

	scrollRate = 65;     // Milliseconds between updates.
	bounce = false;      // False = "Loop", True = "Bounce"
	timesToRepeat = 0;   // Number of times to repeat. 0 = Forever. FOR-EV-ER. FOR-EV-ER.	
	brightness = 3;      // 1 = low, 2 = medium, 3 = high

	startX = 0;          // Starting X coordinate in the frameBuffer
	startY = 0;          // Starting Y coordinate in the frameBuffer
	endX = 15;           // Ending X coordinate in the frameBuffer

	reset();
}


// Reset ALL the things!
void OW10000_text::reset() {
	direction = 1;
	timesRepeated = 0;
	lastUpdate = 0;
	currentOffset = 0;
	
	// Clear the mergeBuffer
	for(int x = 0; x <= 15; x++){
		mergeBuffer[x] = 0x0000;
	}
	
	// Recalculate the clearBuffer mask
	clearBuffer = 0xFFFF;

	// Generate Clear Buffer Mask and Clear old data out of the frameBuffer (only where we want our text to go though.)
	for(int x = startX; x <= endX; x++){
		BIT_CLR(clearBuffer, x);
	}
}


// Jump to a specific offset and update the frameBuffer
void OW10000_text::jumpToOffset(unsigned int position){
	// Disable the scrollRate delay
	unsigned int oldScrollRate = scrollRate;
	scrollRate = 0;
	
	// Scroll the text in as fast as possible
	int temp = 0;
	if(position > 15) temp = position - 15;

	while( temp < position ){
		currentOffset = temp;
		update();
		temp++;
	}
	
	// Restore the original scroll rate delay
	scrollRate = oldScrollRate;
}


// Copy newText into textString
void OW10000_text::setTextString(String newText) {
	// Clear the current String
	textString = "";
	
	// Manually copy the string over
	textString = newText;
	
	// Reset ALL the things!
	reset();	
}

// Copy newText into textString
void OW10000_text::setTextString(char* newText) {
	// Clear the current String
	textString = "";
	
	// Manually copy the string over
	textString = newText;
	
	// Reset ALL the things!
	reset();
}


// Update the textBuffer, Then copy from textBuffer to Frame
// Needs to be called at least every [scrollRate] milliseconds.
void OW10000_text::update() {
	unsigned int textLength = textString.length();
	unsigned int currentCharacter = 0;
	
	if(millis() >= (lastUpdate + scrollRate) && (timesToRepeat == 0 || timesToRepeat >= timesRepeated) && textLength > 0){
		int x;

		// Direction 1 = Right to Left
		if(direction == 1){
			//Clear any extra shifted data from the mergeBuffer
			if(endX < 15){
				BIT_CLR(mergeBuffer[0], 15-endX);
				BIT_CLR(mergeBuffer[1], 15-endX);
				BIT_CLR(mergeBuffer[2], 15-endX);
				BIT_CLR(mergeBuffer[3], 15-endX);
				BIT_CLR(mergeBuffer[4], 15-endX);
				BIT_CLR(mergeBuffer[5], 15-endX);
				BIT_CLR(mergeBuffer[6], 15-endX);
				BIT_CLR(mergeBuffer[7], 15-endX);
				BIT_CLR(mergeBuffer[8], 15-endX);
				BIT_CLR(mergeBuffer[9], 15-endX);
				BIT_CLR(mergeBuffer[10], 15-endX);
				BIT_CLR(mergeBuffer[11], 15-endX);
				BIT_CLR(mergeBuffer[12], 15-endX);
				BIT_CLR(mergeBuffer[13], 15-endX);
				BIT_CLR(mergeBuffer[14], 15-endX);
				BIT_CLR(mergeBuffer[15], 15-endX);
			}
			//bit shift right
			mergeBuffer[0] = mergeBuffer[0] >> 1;
			mergeBuffer[1] = mergeBuffer[1] >> 1;
			mergeBuffer[2] = mergeBuffer[2] >> 1;
			mergeBuffer[3] = mergeBuffer[3] >> 1;
			mergeBuffer[4] = mergeBuffer[4] >> 1;
			mergeBuffer[5] = mergeBuffer[5] >> 1;
			mergeBuffer[6] = mergeBuffer[6] >> 1;
			mergeBuffer[7] = mergeBuffer[7] >> 1;
			mergeBuffer[8] = mergeBuffer[8] >> 1;
			mergeBuffer[9] = mergeBuffer[9] >> 1;
			mergeBuffer[10] = mergeBuffer[10] >> 1;
			mergeBuffer[11] = mergeBuffer[11] >> 1;
			mergeBuffer[12] = mergeBuffer[12] >> 1;
			mergeBuffer[13] = mergeBuffer[13] >> 1;
			mergeBuffer[14] = mergeBuffer[14] >> 1;
			mergeBuffer[15] = mergeBuffer[15] >> 1;

			x = startX;

		} else {
			// Direction 2 = Left to Right
			//Clear any extra shifted data from the mergeBuffer
			if(startX > 0){
				BIT_CLR(mergeBuffer[0], 15-startX);
				BIT_CLR(mergeBuffer[1], 15-startX);
				BIT_CLR(mergeBuffer[2], 15-startX);
				BIT_CLR(mergeBuffer[3], 15-startX);
				BIT_CLR(mergeBuffer[4], 15-startX);
				BIT_CLR(mergeBuffer[5], 15-startX);
				BIT_CLR(mergeBuffer[6], 15-startX);
				BIT_CLR(mergeBuffer[7], 15-startX);
				BIT_CLR(mergeBuffer[8], 15-startX);
				BIT_CLR(mergeBuffer[9], 15-startX);
				BIT_CLR(mergeBuffer[10], 15-startX);
				BIT_CLR(mergeBuffer[11], 15-startX);
				BIT_CLR(mergeBuffer[12], 15-startX);
				BIT_CLR(mergeBuffer[13], 15-startX);
				BIT_CLR(mergeBuffer[14], 15-startX);
				BIT_CLR(mergeBuffer[15], 15-startX);
			}

			//bit shift left
			mergeBuffer[0] = mergeBuffer[0] << 1;
			mergeBuffer[1] = mergeBuffer[1] << 1;
			mergeBuffer[2] = mergeBuffer[2] << 1;
			mergeBuffer[3] = mergeBuffer[3] << 1;
			mergeBuffer[4] = mergeBuffer[4] << 1;
			mergeBuffer[5] = mergeBuffer[5] << 1;
			mergeBuffer[6] = mergeBuffer[6] << 1;
			mergeBuffer[7] = mergeBuffer[7] << 1;
			mergeBuffer[8] = mergeBuffer[8] << 1;
			mergeBuffer[9] = mergeBuffer[9] << 1;
			mergeBuffer[10] = mergeBuffer[10] << 1;
			mergeBuffer[11] = mergeBuffer[11] << 1;
			mergeBuffer[12] = mergeBuffer[12] << 1;
			mergeBuffer[13] = mergeBuffer[13] << 1;
			mergeBuffer[14] = mergeBuffer[14] << 1;
			mergeBuffer[15] = mergeBuffer[15] << 1;
			
			x = endX;
		}
		
		currentCharacter = (textString[currentOffset/6]-32);
		int temp = 7-(currentOffset%6);

		if(BIT_CHECK(font[currentCharacter][0], temp)) BIT_SET(mergeBuffer[startY], 15-x);
		if(BIT_CHECK(font[currentCharacter][1], temp)) BIT_SET(mergeBuffer[startY+1], 15-x);
		if(BIT_CHECK(font[currentCharacter][2], temp)) BIT_SET(mergeBuffer[startY+2], 15-x);
		if(BIT_CHECK(font[currentCharacter][3], temp)) BIT_SET(mergeBuffer[startY+3], 15-x);
		if(BIT_CHECK(font[currentCharacter][4], temp)) BIT_SET(mergeBuffer[startY+4], 15-x);
		if(BIT_CHECK(font[currentCharacter][5], temp)) BIT_SET(mergeBuffer[startY+5], 15-x);
		if(BIT_CHECK(font[currentCharacter][6], temp)) BIT_SET(mergeBuffer[startY+6], 15-x);
	
		// Update the mergeBuffer
		if(bounce){
			// Bouncing Text
			if(direction == 1){
				if(currentOffset >= (textLength * 6) - 1){
					direction = 2;
					currentOffset = currentOffset - (endX - startX + 1);
				} else {
					currentOffset++;
				}
			} else {
				if(currentOffset <= 0){
					direction = 1;
					currentOffset = endX - startX;
				} else {
					currentOffset--;
				}
			}
		} else {
			// Looping Text
			if(direction == 1){
				currentOffset++;
				if( (currentOffset) >= (textLength * 6) ){
					currentOffset = 0;
				}
			} else {
				if( currentOffset == 0){
					currentOffset = (textLength * 6)-1;
				} else {
					currentOffset--;
				}
			}
		}
	
		// Clear old data out
		badge->frameBuffer[startY][0] &= clearBuffer;
		badge->frameBuffer[startY+1][0] &= clearBuffer;
		badge->frameBuffer[startY+2][0] &= clearBuffer;
		badge->frameBuffer[startY+3][0] &= clearBuffer;
		badge->frameBuffer[startY+4][0] &= clearBuffer;
		badge->frameBuffer[startY+5][0] &= clearBuffer;
		badge->frameBuffer[startY+6][0] &= clearBuffer;
		
		badge->frameBuffer[startY][1] &= clearBuffer;
		badge->frameBuffer[startY+1][1] &= clearBuffer;
		badge->frameBuffer[startY+2][1] &= clearBuffer;
		badge->frameBuffer[startY+3][1] &= clearBuffer;
		badge->frameBuffer[startY+4][1] &= clearBuffer;
		badge->frameBuffer[startY+5][1] &= clearBuffer;
		badge->frameBuffer[startY+6][1] &= clearBuffer;
		
		// Merge the mergeBuffer into the frameBuffer
		if(brightness == 3 || brightness == 1){
			badge->frameBuffer[startY][0] |= mergeBuffer[startY];
			badge->frameBuffer[startY+1][0] |= mergeBuffer[startY+1];
			badge->frameBuffer[startY+2][0] |= mergeBuffer[startY+2];
			badge->frameBuffer[startY+3][0] |= mergeBuffer[startY+3];
			badge->frameBuffer[startY+4][0] |= mergeBuffer[startY+4];
			badge->frameBuffer[startY+5][0] |= mergeBuffer[startY+5];
			badge->frameBuffer[startY+6][0] |= mergeBuffer[startY+6];
		}
		if(brightness == 3 || brightness == 2){
			badge->frameBuffer[startY][1] |= mergeBuffer[startY];
			badge->frameBuffer[startY+1][1] |= mergeBuffer[startY+1];
			badge->frameBuffer[startY+2][1] |= mergeBuffer[startY+2];
			badge->frameBuffer[startY+3][1] |= mergeBuffer[startY+3];
			badge->frameBuffer[startY+4][1] |= mergeBuffer[startY+4];
			badge->frameBuffer[startY+5][1] |= mergeBuffer[startY+5];
			badge->frameBuffer[startY+6][1] |= mergeBuffer[startY+6];
		}

		lastUpdate = millis();
	}
}


// Set the diminsions for the frameBuffer area that will be used for the text
void OW10000_text::setDiminsions(unsigned int newStartX, unsigned int newStartY, unsigned int newEndX) {
	if(newStartX > 15) startX = 15;
	else startX = newStartX;

	if(newEndX > 15) endX = 15;
	else endX = newEndX;

	if(newStartY > 8) startY = 8;
	else startY = newStartY;
	
	if(endX < startX) endX = startX;

	// Reset ALL the things!
	reset();
}


// Set the scrolling rate (in milliseconds) (closer to 0 is faster)
// 35ms is the theoretical minimum
void OW10000_text::setScrollRate(unsigned int newScrollRate) {
	if(newScrollRate > 9001){
		scrollRate = 9001;
	} else {
		scrollRate = newScrollRate; 
	}
}


// Set the brightness of the text
// 1 = low, 2 = medium, 3 = high
void OW10000_text::setBrightness(unsigned int newBrightness) {
	if(newBrightness > 3){
		brightness = 3;
	} else if(newBrightness < 1) {
		brightness = 1;
	} else {
		brightness = newBrightness;
	}
}


// Set the direction of scrolling
void OW10000_text::setDirection(unsigned int newDirection){
	if(newDirection == 2) direction = 2;
	else direction = 1;
}


// Set whether text "Bounces" or "Loops" (set scrollRate to 0, or don't update for static text)
void OW10000_text::setBounce(bool newBounce){
	bounce = newBounce;

	// Reset ALL the things!
	reset();
}


// Set the number of times to repeat bounce / loop
void OW10000_text::setTimesToRepeat(unsigned int newTimesToRepeat) {
	if(newTimesToRepeat > 9001){
		timesToRepeat = 9001;
	} else {
		timesToRepeat = newTimesToRepeat;
	}

	// Reset ALL the things!
	reset();
}
