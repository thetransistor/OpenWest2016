/*
  Filename: ow10000-tetris.h
  Purpose: Tetris Game
  Created: June 29, 2016
*/

#ifndef OW10000TETRIS_H
#define OW10000TETRIS_H

#include "Arduino.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"

class OW10000_tetris{
	public:
		OW10000_tetris(OW10000HAL* badgeToLinkTo); // Constructor

		void play();  // Start a game
		void reset(); // Reset variables, and start a new game
			
	private:	
		OW10000HAL* badge;     // Pointer to the badge instance
};


#endif