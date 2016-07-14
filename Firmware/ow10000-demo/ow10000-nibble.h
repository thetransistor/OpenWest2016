/*
  Filename: ow10000-nibble.h
  Purpose: Nibble Game
  Created: June 29, 2016
*/

#ifndef OW10000NIBBLE_H
#define OW10000NIBBLE_H

#include "Arduino.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"

class OW10000_nibble {
	public:
		OW10000_nibble(OW10000HAL* badgeToLinkTo); // Constructor
		
		void play();  // Start a game
		void reset(); // Reset variables
		void gameOver(); // Thy game is over.
		
		void getNextApple(); // Get the next apple location
		bool snakeCollision(byte collision_location); // Check location for possible collision
		void addNewLocationToStack(); // Add the new Snake Location to the 'top' of the stack, update as needed

	private:	
		OW10000HAL* badge;     // Pointer to the badge instance
		
		byte nibbleStack[256]; // up to 256 locations. High 4 bits are X-axis, and low 4 bits are Y-Axis
		unsigned int nibbleStack_length; // Length of locations on the nibbleStack
		unsigned int snake_length; // Length of the snake, including hidden portions after picking up an apple
		unsigned int snakeSpeed; // Current snakeSpeed, lower is faster
		unsigned int score; // Current score
		char direction; // Current Direction
		byte apple_location; // Current Apple Location
		byte snake_location; // Current Snake Location
		long lastUpdate; // Game timer
		bool stillPlaying; // Are you still playing?
};

#endif