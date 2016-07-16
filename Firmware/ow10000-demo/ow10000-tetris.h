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

#define TETRIS_WIDTH 16
#define TETRIS_HEIGHT 16

class BlockRotation {
    public:
        int width, height;
        unsigned short map[4];
};

class Block {
    public:
        int num_rotations;
        BlockRotation rotations[4];
};

class OW10000_tetris{
    public:
        OW10000_tetris(OW10000HAL* badgeToLinkTo); // Constructor

        void play();  // Start a game
        void reset(); // Reset variables, and start a new game
            
    private:    
        OW10000HAL* badge;     // Pointer to the badge instance
        Block blocks[7];
        unsigned short rows[TETRIS_HEIGHT];
        int block_x, block_y, block, rotation;
        long lastUpdate; // Game timer
        long gameSpeed;

        bool check_position(int x, int y, int rotation);
        void settle();
        void paint();
};


#endif
