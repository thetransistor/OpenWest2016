/*
  Filename: ow10000-tetris.cpp
  Purpose: Tetris Game
  Created: June 29, 2016
*/

#include "Arduino.h"
#include "ow10000-tetris.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"

// Default Constructor
OW10000_tetris::OW10000_tetris(OW10000HAL* badgeToLinkTo) {
	badge = badgeToLinkTo;
	
	reset();
}


// Reset the game
void OW10000_tetris::reset() {

}


// Play the game
void OW10000_tetris::play() {
	// If both buttons are pressed, leave the game
	while(!badge->buttonAB_debounce(200)) {
		// Main game loop
		
		// check the buttons
		if(badge->buttonA_debounce()){
			//
		}

		// reading the bottom button 
		if(badge->buttonB_debounce()){
			//
		}
		
	}
}