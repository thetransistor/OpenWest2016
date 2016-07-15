/*
  Filename: ow10000-helicopter.cpp
  Purpose: Helicopter Game
  Created: July 14, 2016
*/

#include "Arduino.h"
#include "ow10000-helicopter.h"
#include "ow10000-hardware.h"
#include "ow10000-text.h"
#include "ow10000-binConversions.h"

// Default Constructor
OW10000_helicopter::OW10000_helicopter(OW10000HAL* badgeToLinkTo) {
  badge = badgeToLinkTo;
  randomSeed(millis()); // See the "random" number generator

  reset();
}


// Reset the game
void OW10000_helicopter::reset() {
  level = 1;
  score = 0;
  stillPlaying = true;
  helicopterLocation = 0b00110111;
  roof[0] = 0b00000000;
  roof[1] = 0b00010010;
  roof[2] = 0b00100011;
  roof[3] = 0b00110100;
  ground[0] = 0b00001111;
  ground[1] = 0b00011100;
  ground[2] = 0b00101011;
  ground[3] = 0b00111010;
  for (int i = 4; i < 16; i++) {
    generateCave(i);
  }
}


// Generate a roof and ground at the given index
// Prerequisite: index > 0
void OW10000_helicopter::generateCave(int index) {
  tempRoof = roof[index - 1];
  tempGround = ground[index - 1];

  // Move duplicate to the right
  tempByte = ((tempRoof >> 4) + 1) << 4;
  tempRoof = (tempRoof & 0b00001111) | tempByte;
  tempByte = ((tempGround >> 4) + 1) << 4;
  tempGround = (tempGround & 0b00001111) | tempByte;
  
  // Randomly move the new roof up or down
  if ((random() % 2) == 1) {
    // Check upper boundaries
    if ((tempRoof & 0b00001111) == 0b00000000) {
      tempRoof = tempRoof + 1;
    } else {
      tempRoof = tempRoof - 1;
    }
  } else {
    tempRoof = tempRoof + 1;
  }

  // Randomly move the new ground up or down
  if ((random() % 2) == 1) {
    // Check lower boundaries
    if ((tempGround & 0b00001111) == 0b00001111) {
      tempGround = tempGround - 1;
    } else {
      tempGround = tempGround + 1;
    }
  } else {
    tempGround = tempGround - 1;
  }

  // Ensure reasonable gap
  while ((btoi(tempGround & 0b00001111) - btoi(tempRoof & 0b00001111)) < 4) {  // While the gap is too small    
    if (btoi(tempRoof & 0b00001111) < ((btoi(0b00001111) - btoi(tempGround & 0b00001111)))) {  // If roof is closer to the boundary
      // Move ground down
      tempGround++;
    } else {
      // Move roof up
      tempRoof--;
    }
  }

  roof[index] = tempRoof;
  ground[index] = tempGround;
}


// Shift the cave forward and generate a new piece
void OW10000_helicopter::advanceCave() {
  for (int i = 0; i < 15; i++) {
    roof[i] = roof[i + 1];
    ground[i] = ground[i + 1];

    // Move left
    tempByte = ((roof[i] >> 4) - 1) << 4;
    roof[i] = (roof[i] & 0b00001111) | tempByte;
    tempByte = ((ground[i] >> 4) - 1) << 4;
    ground[i] = (ground[i] & 0b00001111) | tempByte;
  }
  generateCave(15);

  // Print location
  badge->clear();
  printCave();
  badge->setPixel((helicopterLocation >> 4), (helicopterLocation & 0b00001111), 3);
}


/**
   There's a horrible flashing problem. It seems to be clearing and printing every CPU tick...
*/
// Print
void OW10000_helicopter::printCave() {
  for (int i = 0; i < 16; i++) {
    tempRoof = roof[i];
    tempGround = ground[i];

    // Cave filling
    while ((tempRoof & 0b00001111) != 0b00000000) {
      badge->setPixel((tempRoof >> 4), (tempRoof & 0b00001111), 1);
      tempRoof = tempRoof - 1;
    }
    badge->setPixel((tempRoof >> 4), (tempRoof & 0b00001111), 1); // This fills the border pixel
    
    while ((tempGround & 0b00001111) != 0b00001111) {
      badge->setPixel((tempGround >> 4), (tempGround & 0b00001111), 1);
      tempGround = tempGround + 1;
    }
    badge->setPixel((tempGround >> 4), (tempGround & 0b00001111), 1);

    // Cave lining
    badge->setPixel((roof[i] >> 4), (roof[i] & 0b00001111), 2); // Overwrite the filling
    badge->setPixel((ground[i] >> 4), (ground[i] & 0b00001111), 2);
  }
}


// Check for collisions with the walls
bool OW10000_helicopter::helicopterCollision(byte collisionLocation) {
  if (btoi(collisionLocation & 0b00001111) <= btoi(roof[3] & 0b00001111) || btoi(collisionLocation & 0b00001111) >= btoi(ground[3] & 0b00001111)) {
    return true;
  } else {
    return false;
  }
}


// Play the game
void OW10000_helicopter::play() {
  // Starting location
  reset();
  badge->setPixel((helicopterLocation >> 4), (helicopterLocation & 0b00001111), 3);
  printCave();

  // If both buttons are pressed, leave the game
  while (!badge->buttonAB_debounce(200) && stillPlaying) {
    // Main game loop

    // reading the top or up button
    if (badge->buttonA_debounce() || badge->buttonU_debounce()) {
      // Move the helicopter up
      helicopterLocation--;

      // Print location
      badge->clear();
      printCave();
      badge->setPixel((helicopterLocation >> 4), (helicopterLocation & 0b00001111), 3);
    }

    // reading the bottom or down button
    if (badge->buttonB_debounce() || badge->buttonD_debounce()) {
      // Move the helicopter down
      helicopterLocation++;

      // Print location
      badge->clear();
      printCave();
      badge->setPixel((helicopterLocation >> 4), (helicopterLocation & 0b00001111), 3);
    }

    // Check collisions
    if (helicopterCollision(helicopterLocation)) {
      gameOver(); // Hit a wall
    }


    // Advance the cave
    if (counter == floor(250 / level)) {
      advanceCave();
      counter = 0;
      level = level + 0.02;
      score++;
    }

    delay(1);
    counter++;
  }
}

void OW10000_helicopter::gameOver() {
  // Finish
  stillPlaying = false;
}

int OW10000_helicopter::getScore() {
  return score;
}

