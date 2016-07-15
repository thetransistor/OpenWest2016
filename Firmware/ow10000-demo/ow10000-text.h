/*
  Filename: ow10000-text.h
  Purpose: Text Library
  Created: June 29, 2016
*/

#ifndef OW10000TEXT_H
#define OW10000TEXT_H

#include "Arduino.h"
#include "ow10000-hardware.h"


class OW10000_text {
	public:
		OW10000_text(OW10000HAL* badgeToLinkTo); // Constructor
		void update(); // Update the textBuffer, Then copy from textBuffer to Frame
		
		void setScrollRate(unsigned int newScrollRate);
		void setBounce(bool newBounce);
		void setTimesToRepeat(unsigned int newTimesToRepeat);
		void setBrightness(unsigned int newBrightness);
		void setDirection(unsigned int newDirection);
		void setDiminsions(unsigned int newStartX, unsigned int newStartY, unsigned int newEndX);
		void jumpToOffset(unsigned int position);
		void reset();
		
		void setTextString(char* newText);
		void setTextString(String newText);
		
	private:
		OW10000HAL* badge;            // Pointer to the badge instance
		
		String textString;            // Text to display

		unsigned int scrollRate;      // Milliseconds between allowed updates
		bool bounce;                  // False = loop, True = "Bounce"
		unsigned int timesToRepeat;   // Number of times to repeat. 0 = Forever. FOR-EV-ER. FOR-EV-ER.
		unsigned int brightness;      // 1 = low, 2 = medium, 3 = high
		unsigned int direction;       // 1 = Right to Left, 2 = Left to Right
		
		long lastUpdate;              // millis() of the last update
		unsigned int timesRepeated;   // Number of times "Looped" or "Bounced"
		
		unsigned int startX;          // Starting X coordinate in the frameBuffer
		unsigned int startY;          // Starting Y coordinate in the frameBuffer
		unsigned int endX;            // Ending X coordinate in the frameBuffer
		int currentOffset;            // Current pixel (x) position into imaginary textBuffer, relative to frameBuffer:(x)=0
		
		unsigned int mergeBuffer[16]; // buffer for merging text into frameBuffer for LED Array
		unsigned int clearBuffer;     // buffer mask
};


// 5 x 7 pixel font. Offset 32 (DEC) from ASCII
const byte font[95][7]={
{	0b00000000,	// 00 - Space
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b00100000,	// 01 - !
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00000000,
	0b00100000},
{	0b00101000,	// 02 - "
	0b00101000,
	0b01010000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b01010000,	// 03 - #
	0b01010000,
	0b11111000,
	0b01010000,
	0b11111000,
	0b01010000,
	0b01010000},
{	0b00100000,	// 04 - $
	0b01111000,
	0b10000000,
	0b01110000,
	0b00001000,
	0b11110000,
	0b00100000},
{	0b10001000,	// 05 - %
	0b10010000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01001000,
	0b10001000},
{	0b00100000,	// 06 - &
	0b01010000,
	0b00100000,
	0b01101000,
	0b10110000,
	0b10010000,
	0b01101000},
{	0b00100000,	// 07 - '
	0b00100000,
	0b01000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b00110000,	// 08 - (
	0b01000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b01000000,
	0b00110000},
{	0b11000000,	// 09 - )
	0b00100000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00100000,
	0b11000000},
{	0b00000000,	// 10 - *
	0b00000000,
	0b10001000,
	0b01110000,
	0b10001000,
	0b00000000,
	0b00000000},
{	0b00000000,	// 11 - +
	0b00100000,
	0b00100000,
	0b11111000,
	0b00100000,
	0b00100000,
	0b00000000},
{	0b00000000,	// 12 - ,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b01000000},
{	0b00000000,	// 13 - -
	0b00000000,
	0b00000000,
	0b11111000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b00000000,	// 14 - .
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00100000},
{	0b00001000,	// 15 - /
	0b00010000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b01000000,
	0b10000000},
{	0b01110000,	// 16 - 0
	0b10001000,
	0b10011000,
	0b10101000,
	0b11001000,
	0b10001000,
	0b01110000},
{	0b00100000,	// 17 - 1
	0b01100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b11111000},
{	0b01110000,	// 18 - 2
	0b10001000,
	0b00001000,
	0b00110000,
	0b01000000,
	0b10001000,
	0b11111000},
{	0b01110000,	// 19 - 3
	0b10001000,
	0b00001000,
	0b00110000,
	0b00001000,
	0b10001000,
	0b01110000},
{	0b00011000,	// 20 - 4
	0b00101000,
	0b01001000,
	0b10001000,
	0b11111000,
	0b00001000,
	0b00001000},
{	0b11111000,	// 21 - 5
	0b10000000,
	0b11110000,
	0b00001000,
	0b00001000,
	0b10001000,
	0b01110000},
{	0b00110000,	// 22 - 6
	0b01000000,
	0b10000000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b11111000,	// 23 - 7
	0b10001000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b00100000,
	0b00100000},
{	0b01110000,	// 24 - 8
	0b10001000,
	0b10001000,
	0b01110000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b01110000,	// 25 - 9
	0b10001000,
	0b10001000,
	0b01111000,
	0b00001000,
	0b00010000,
	0b01100000},
{	0b00000000,	// 26 - :
	0b00100000,
	0b00100000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b00000000},
{	0b00000000,	// 27 - ;
	0b00100000,
	0b00100000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b01000000},
{	0b00010000,	// 28 - <
	0b00100000,
	0b01000000,
	0b10000000,
	0b01000000,
	0b00100000,
	0b00010000},
{	0b00000000,	// 29 - =
	0b00000000,
	0b11111000,
	0b00000000,
	0b00000000,
	0b11111000,
	0b00000000},
{	0b10000000,	// 30 - >
	0b01000000,
	0b00100000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000},
{	0b01110000,	// 31 - ?
	0b10001000,
	0b00001000,
	0b00010000,
	0b00100000,
	0b00000000,
	0b00100000},
{	0b01110000,	// 32 - @
	0b10001000,
	0b10101000,
	0b10101000,
	0b10111000,
	0b10000000,
	0b01110000},
{	0b01110000,	// 33 - A
	0b10001000,
	0b10001000,
	0b11111000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b11110000,	// 34 - B
	0b10001000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11110000},
{	0b01110000,	// 35 - C
	0b10001000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10001000,
	0b01110000},
{	0b11110000,	// 36 - D
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b11110000},
{	0b11111000,	// 37 - E
	0b10000000,
	0b11100000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b11111000},
{	0b11111000,	// 38 - F
	0b10000000,
	0b11100000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000},
{	0b01111000,	// 39 - G
	0b10000000,
	0b10111000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b10001000,	// 40 - H
	0b10001000,
	0b11111000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b01110000,	// 41 - I
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b01110000},
{	0b00001000,	// 42 - J
	0b00001000,
	0b00001000,
	0b00001000,
	0b00001000,
	0b10001000,
	0b01110000},
{	0b10001000,	// 43 - K
	0b10010000,
	0b11100000,
	0b10010000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b10000000,	// 44 - L
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b11111000},
{	0b10001000,	// 45 - M
	0b11011000,
	0b10101000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b10001000,	// 46 - N
	0b11001000,
	0b10101000,
	0b10011000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b01110000,	// 47 - O
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b11110000,	// 48 - P
	0b10001000,
	0b11110000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b10000000},
{	0b01110000,	// 49 - Q
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10010000,
	0b01101000},
{	0b11110000,	// 50 - R
	0b10001000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b01111000,	// 51 - S
	0b10000000,
	0b01110000,
	0b00001000,
	0b00001000,
	0b10001000,
	0b01110000},
{	0b11111000,	// 52 - T
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000},
{	0b10001000,	// 53 - U
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b10001000,	// 54 - V
	0b10001000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b01010000,
	0b00100000},
{	0b10001000,	// 55 - W
	0b10001000,
	0b10001000,
	0b10001000,
	0b10101000,
	0b11011000,
	0b10001000},
{	0b10001000,	// 56 - X
	0b01010000,
	0b00100000,
	0b01010000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b10001000,	// 57 - Y
	0b01010000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000},
{	0b11111000,	// 58 - Z
	0b00001000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b10000000,
	0b11111000},
{	0b01110000,	// 59 - [
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01110000},
{	0b10000000,	// 60 - \        //
	0b01000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00010000,
	0b00001000},
{	0b01110000,	// 61 - ]
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b00010000,
	0b01110000},
{	0b00000000,	// 62 - ^
	0b00100000,
	0b01010000,
	0b10001000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b00000000,	// 63 - _
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b11111000},
{	0b11000000,	// 64 - `
	0b01000000,
	0b00100000,
	0b00000000,
	0b00000000,
	0b00000000,
	0b00000000},
{	0b00000000,	// 65 - a
	0b00000000,
	0b01110000,
	0b00001000,
	0b01111000,
	0b10001000,
	0b01111000},
{	0b10000000,	// 66 - b 
	0b10000000,
	0b10110000,
	0b11001000,
	0b10001000,
	0b10001000,
	0b11110000},
{	0b00000000,	// 67 - c
	0b00000000,
	0b01110000,
	0b10000000,
	0b10000000,
	0b10000000,
	0b01110000},
{	0b00001000,	// 68 - d
	0b00001000,
	0b01101000,
	0b10011000,
	0b10001000,
	0b10001000,
	0b01111000},
{	0b00000000,	// 69 - e
	0b00000000,
	0b01110000,
	0b10001000,
	0b11111000,
	0b10000000,
	0b01111000},
{	0b00110000,	// 70 - f
	0b01000000,
	0b11110000,
	0b01000000,
	0b01000000,
	0b01000000,
	0b01000000},
{	0b00000000,	// 71 - g
	0b01111000,
	0b10001000,
	0b10001000,
	0b01111000,
	0b00001000,
	0b11110000},
{	0b10000000,	// 72 - h
	0b10000000,
	0b10110000,
	0b11001000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b00000000,	// 73 - i
	0b00100000,
	0b00000000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000},
{	0b00001000,	// 74 - j
	0b00000000,
	0b00001000,
	0b00001000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b10000000,	// 75 - k
	0b10000000,
	0b10010000,
	0b10100000,
	0b11000000,
	0b10100000,
	0b10010000},
{	0b00100000,	// 76 - l
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00010000},
{	0b00000000,	// 77 - m
	0b00000000,
	0b11010000,
	0b10101000,
	0b10101000,
	0b10001000,
	0b10001000},
{	0b00000000,	// 78 - n
	0b00000000,
	0b11110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000},
{	0b00000000,	// 79 - o
	0b00000000,
	0b01110000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01110000},
{	0b00000000,	// 80 - p
	0b10110000,
	0b11001000,
	0b10001000,
	0b11110000,
	0b10000000,
	0b10000000},
{	0b00000000,	// 81 - q
	0b01101000,
	0b10011000,
	0b10001000,
	0b01111000,
	0b00001000,
	0b00001000},
{	0b00000000,	// 82 - r
	0b00000000,
	0b10110000,
	0b11001000,
	0b10000000,
	0b10000000,
	0b10000000},
{	0b00000000,	// 83 - s
	0b00000000,
	0b01111000,
	0b10000000,
	0b01110000,
	0b00001000,
	0b11110000},
{	0b00100000,	// 84 - t
	0b00100000,
	0b01110000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00010000},
{	0b00000000,	// 85 - u
	0b00000000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01111000},
{	0b00000000,	// 86 - v
	0b00000000,
	0b10001000,
	0b10001000,
	0b10001000,
	0b01010000,
	0b00100000},
{	0b00000000,	// 87 - w
	0b00000000,
	0b10001000,
	0b10001000,
	0b10101000,
	0b10101000,
	0b01111000},
{	0b00000000,	// 88 - x
	0b00000000,
	0b10001000,
	0b01010000,
	0b00100000,
	0b01010000,
	0b10001000},
{	0b00000000,	// 89 - y
	0b00000000,
	0b10001000,
	0b10001000,
	0b01111000,
	0b00001000,
	0b11110000},
{	0b00000000,	// 90 - z
	0b00000000,
	0b11111000,
	0b00010000,
	0b00100000,
	0b01000000,
	0b11111000},
{	0b00110000,	// 91 - {
	0b01000000,
	0b01000000,
	0b10000000,
	0b01000000,
	0b01000000,
	0b00110000},
{	0b00100000,	// 92 - |
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000,
	0b00100000},
{	0b11000000,	// 93 - }
	0b00100000,
	0b00100000,
	0b00010000,
	0b00100000,
	0b00100000,
	0b11000000},
{	0b00000000,	// 94 - ~
	0b00000000,
	0b01101000,
	0b10010000,
	0b00000000,
	0b00000000,
	0b00000000}
};

#endif
