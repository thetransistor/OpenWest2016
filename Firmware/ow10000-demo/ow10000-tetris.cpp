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
    randomSeed(millis()); // See the "random" number generator
    //I block
    blocks[0].num_rotations = 2;
    blocks[0].rotations[0] = {4, 1, {0b1111}};
    blocks[0].rotations[1] = {2, 4, {0b10,
                                     0b10,
                                     0b10,
                                     0b10}};
    //J block
    blocks[1].num_rotations = 4;
    blocks[1].rotations[0] = {3, 2, {0b111,
                                     0b001}};
    blocks[1].num_rotations = 4;
    blocks[1].rotations[1] = {2, 3, {0b01,
                                     0b01,
                                     0b11}};
    blocks[1].num_rotations = 4;
    blocks[1].rotations[2] = {3, 2, {0b100,
                                     0b111}};
    blocks[1].num_rotations = 4;
    blocks[1].rotations[3] = {2, 3, {0b11,
                                     0b10,
                                     0b10}};
    //L block
    blocks[2].num_rotations = 4;
    blocks[2].rotations[0] = {3, 2, {0b111,
                                     0b100}};
    blocks[2].num_rotations = 4;
    blocks[2].rotations[1] = {2, 3, {0b10,
                                     0b10,
                                     0b11}};
    blocks[2].num_rotations = 4;
    blocks[2].rotations[2] = {3, 2, {0b001,
                                     0b111}};
    blocks[2].num_rotations = 4;
    blocks[2].rotations[3] = {2, 3, {0b11,
                                     0b01,
                                     0b01}};
    //O block
    blocks[3].num_rotations = 1;
    blocks[3].rotations[0] = {2, 2, {0b11,
                                     0b11}};
    //S block
    blocks[4].num_rotations = 2;
    blocks[4].rotations[0] = {3, 2, {0b011,
                                     0b110}};
    blocks[4].rotations[1] = {2, 3, {0b10,
                                     0b11,
                                     0b01}};
    //Z block
    blocks[5].num_rotations = 2;
    blocks[5].rotations[0] = {3, 2, {0b110,
                                     0b011}};
    blocks[5].rotations[1] = {2, 3, {0b01,
                                     0b11,
                                     0b10}};
    //T block
    blocks[6].num_rotations = 4;
    blocks[6].rotations[0] = {3, 2, {0b010,
                                     0b111}};
    blocks[6].rotations[1] = {2, 3, {0b10,
                                     0b11,
                                     0b10}};
    blocks[6].rotations[2] = {3, 2, {0b111,
                                     0b010}};
    blocks[6].rotations[3] = {2, 3, {0b01,
                                     0b11,
                                     0b01}};
}


// Reset the game
void OW10000_tetris::reset() {
    //Initialize screen
    for (int y = 0; y < TETRIS_HEIGHT; y++) {
        rows[y] = 0;
        badge->setPixel(TETRIS_WIDTH, y, 1);
        for (int x = TETRIS_WIDTH + 1; x < 16; x++) {
            badge->setPixel(x, y, 0);
        }
    }
    block_x = TETRIS_WIDTH / 2 - 1;
    block_y = 0;
    rotation = 0;
    block = random(7);
    next_block = random(7);
    draw_next();
    gameSpeed = 1000; //1 second to start with
}


// Play the game
void OW10000_tetris::play() {
    reset();
    // If both buttons are pressed, leave the game
    while(!badge->buttonAB_debounce(200)) {
        // Main game loop
        
        // check the buttons
        int new_x = block_x;
        int new_y = block_y;
        int new_rotation = rotation;
        bool pressed = false;
        if(badge->buttonL_debounce()){
            new_x--;
            pressed = true;
        }
        if(badge->buttonR_debounce()){
            new_x++;
            pressed = true;
        }
        if(badge->buttonU_debounce()){
            new_rotation = (rotation + 1) % blocks[block].num_rotations;
            pressed = true;
        }
        if(badge->buttonD_debounce()){
            new_y++;
            pressed = true;
        }
        if (pressed && check_position(new_x, new_y, new_rotation)) {
            block_x = new_x;
            block_y = new_y;
            rotation = new_rotation;
            paint();
        }

        // Update stuff
        if(millis() > lastUpdate + gameSpeed){
            lastUpdate = millis(); // Update the game timer

            if (check_position(block_x, block_y + 1, rotation)) {
                block_y++;
            }
            else {
                settle();

                //Get new block ready
                block_x = TETRIS_WIDTH / 2 - 1;
                block_y = 0;
                block = next_block;
                next_block = random(7);
                draw_next();
                rotation = 0;
                if (!check_position(block_x, block_y, rotation)) {
                    //Game Over
                    break;
                }
            }
            paint();
            if (gameSpeed > 100) gameSpeed--;
        }
    }
}

void OW10000_tetris::paint()
{
    //turn on pixels for occupied spaces
    for (int y = 0; y < TETRIS_HEIGHT; y++) {
        for (int x = 0; x < TETRIS_WIDTH; x++) {
            if (rows[y] & (1 << x))
                badge->setPixel(x, y, 3);
            else
                badge->setPixel(x, y, 0);
        }
    }

    //turn on pixels for current block in play
    BlockRotation &b = blocks[block].rotations[rotation];
    for (int x = 0; x < b.width; x++) {
        if (x + block_x >= TETRIS_WIDTH) break;
        for (int y = 0; y < b.height; y++) {
            if (y + block_y >= TETRIS_HEIGHT) break;
            if (b.map[y] & (1 << x))
                badge->setPixel(x + block_x, y + block_y, 3);
        }
    }
}

void OW10000_tetris::draw_next()
{
    //clear old one
    for (int x = TETRIS_WIDTH + 1; x < 16; x++) {
        for (int y = 1; y < 5; y++) {
            badge->setPixel(x, y, 0);
        }
    }

    //draw new one
    BlockRotation &b = blocks[next_block].rotations[0];
    for (int x = 0; x < b.width; x++) {
        for (int y = 0; y < b.height; y++) {
            if (b.map[y] & (1 << x))
                badge->setPixel(x + TETRIS_WIDTH + 2, y + 1, 3);
        }
    }
}

//Validate that a block can occupy the specified position
bool OW10000_tetris::check_position(int test_x, int test_y, int test_rotation)
{
    BlockRotation &b = blocks[block].rotations[test_rotation];
    for (int x = 0; x < b.width; x++) {
        for (int y = 0; y < b.height; y++) {
            if (!(b.map[y] & (1 << x))) continue;
            if (x + test_x < 0 || x + test_x >= TETRIS_WIDTH) return false;
            if (y + test_y < 0 || y + test_y >= TETRIS_HEIGHT) return false;
            if (rows[test_y + y] & (1 << (test_x + x))) return false;
        }
    }
    return true;
}


void OW10000_tetris::settle()
{
    //Mark the spaces where the current block in play sits
    BlockRotation &b = blocks[block].rotations[rotation];
    for (int x = 0; x < b.width; x++) {
        for (int y = 0; y < b.height; y++) {
            if (b.map[y] & (1 << x)) {
                rows[y + block_y] |= 1 << (x + block_x);
            }
        }
    }

    //Look for and delete solid rows
    bool done = false;
    while (!done) {
        done = true;
        for (int y = TETRIS_HEIGHT - 1; y >= 0; y--) {
            if (rows[y] == SOLID_ROW) {
                //This is where we would keep score if there were anywhere to display it
                done = false;
                for (int i = y; i > 0; i--) {
                    rows[i] = rows[i - 1];
                }
                rows[0] = 0;
                break;
            }
        }
    }
}
