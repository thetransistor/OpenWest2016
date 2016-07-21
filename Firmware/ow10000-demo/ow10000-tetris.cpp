/*
  Filename: ow10000-tetris.cpp
  Purpose: Tetris Game
  Created: June 29, 2016
*/

#include "Arduino.h"
#include "ow10000-tetris.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"
#include "bitmacros.h"

// Default Constructor
OW10000_tetris::OW10000_tetris(OW10000HAL* badgeToLinkTo) {
    badge = badgeToLinkTo;
    randomSeed(millis()); // See the "random" number generator
    //I block
    blocks[0] = {2, 4, 2, {0b1111, 0b0100010001000100}};

    //J block
    blocks[1] = {4, 3, 2, {0b10001110, 0b010001001100, 0b11100010, 0b110010001000}};

    //L block
    blocks[2] = {4, 3, 2, {0b11101000, 0b100010001100, 0b00101110, 0b110001000100}};

    //O block
    blocks[3] = {1, 2, 2, {0b11001100}};

    //S block
    blocks[4] = {2, 3, 2, {0b01101100, 0b100011000100}};

    //Z block
    blocks[5] = {2, 3, 2, {0b11000110, 0b010011001000}};

    //T block
    blocks[6] = {4, 3, 2, {0b01001110, 0b010011000100, 0b11100100, 0b100011001000}};
}


// Reset the game
void OW10000_tetris::reset() {
    //Initialize screen
    for (int y = 0; y < TETRIS_HEIGHT; y++) {
        rows[y] = 0;
        badge->setPixel(TETRIS_WIDTH, y, 2);
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
        if(badge->buttonA_debounce()){
            new_rotation = rotation == 0 ? blocks[block].num_rotations - 1 : rotation - 1;
            pressed = true;
        }
        if(badge->buttonD_debounce()){
            new_y++;
            pressed = true;
        }
        if(badge->buttonB_debounce()){
            while (check_position(new_x, new_y + 1, new_rotation)) {
                new_y++;
            }
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
            if (BIT_CHECK(rows[y], x))
                badge->setPixel(x, y, 3);
            else
                badge->setPixel(x, y, 0);
        }
    }
    //turn on pixels for current block in play
    draw_block(block_x, block_y, block, rotation);
}

void OW10000_tetris::draw_block(int draw_x, int draw_y, int block_num, int rotation_num)
{
    unsigned short b = blocks[block_num].rotations[rotation_num];
    int w = (rotation_num & 1) ? blocks[block_num].height : blocks[block_num].width;
    int h = (rotation_num & 1) ? blocks[block_num].width : blocks[block_num].height;
    for (int y = 0; y < h; y++) {
        unsigned short row = (b >> (y * 4)) & 0xF;
        for (int x = 0; x < w; x++) {
            if (BIT_CHECK(row, 3 - x))
                badge->setPixel(x + draw_x, y + draw_y, 3);
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
    draw_block(TETRIS_WIDTH + 2, 1, next_block, 0);
}

//Validate that a block can occupy the specified position
bool OW10000_tetris::check_position(int test_x, int test_y, int test_rotation)
{
    unsigned short b = blocks[block].rotations[test_rotation];
    int w = (test_rotation & 1) ? blocks[block].height : blocks[block].width;
    int h = (test_rotation & 1) ? blocks[block].width : blocks[block].height;
    for (int y = 0; y < h; y++) {
        unsigned short row = (b >> (y * 4)) & 0xF;
        for (int x = 0; x < w; x++) {
            if (!BIT_CHECK(row, 3 - x)) continue;
            if (x + test_x < 0 || x + test_x >= TETRIS_WIDTH) return false;
            if (y + test_y < 0 || y + test_y >= TETRIS_HEIGHT) return false;
            if (BIT_CHECK(rows[test_y + y], test_x + x)) return false;
        }
    }
    return true;
}


void OW10000_tetris::settle()
{
    //Mark the spaces where the current block in play sits
    unsigned short b = blocks[block].rotations[rotation];
    int w = (rotation & 1) ? blocks[block].height : blocks[block].width;
    int h = (rotation & 1) ? blocks[block].width : blocks[block].height;
    for (int y = 0; y < h; y++) {
        unsigned short row = (b >> (y * 4)) & 0xF;
        for (int x = 0; x < w; x++) {
            if (BIT_CHECK(row, 3 - x)) {
                BIT_SET(rows[y + block_y], x + block_x);
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
