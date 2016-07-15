/*
  Filename: ow10000-helicopter.h
  Purpose: Helicopter Game
  Created: July 14, 2016
*/

#ifndef OW10000HELICOPTER_H
#define OW10000HELICOPTER_H

#include "Arduino.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"

class OW10000_helicopter{
	public:
		OW10000_helicopter(OW10000HAL* badgeToLinkTo); // Constructor

		void play();  // Start a game
		void reset(); // Reset variables
    void gameOver();
    
    bool helicopterCollision(byte collisionLocation);
    void generateCave(int index);   // Generate a roof and ground at the given index
    void advanceCave();
    void printCave();
    int getScore();
    int getHighScore();
    
	private:	
		OW10000HAL* badge;     

    //OW10000_text* scoreboard;   I couldn't get this to work
    //long score;
    double level;
    int counter;
    bool stillPlaying;
    byte helicopterLocation; // High 4 bits are X-axis, and low 4 bits are Y-Axis
    byte roof[16];
    byte ground[16];
    byte tempRoof;
    byte tempGround;
    byte tempByte;      
    int score;
};


#endif
