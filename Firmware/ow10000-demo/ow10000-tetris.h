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

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 16
#define SOLID_ROW ((1 << TETRIS_WIDTH) - 1)

class Block {
    public:
        unsigned char num_rotations, width, height;
        unsigned short rotations[4];
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
        int next_block;
        long lastUpdate; // Game timer
        long gameSpeed;

        bool check_position(int x, int y, int rotation);
        void settle();
        void paint();
        void draw_next();
        void draw_block(int draw_x, int draw_y, int block_num, int rotation_num);
};


#endif
