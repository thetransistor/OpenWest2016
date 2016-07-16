/*
  Filename: ow1000-hardware.h
  Purpose: Hardware Library
  Created: June 29, 2016
*/

#ifndef OW10000HAL_H
#define OW10000HAL_H

#define BATTERY_VOLTS       A5  //P F0
#define BATTERY_ZERO		170
#define BATTERY_FULL		600
#define DEBOUNCEMILLIS      5   //Default # of milliseconds to debounce button presses

class OW10000HAL
{
	public:
		OW10000HAL(); // Default constructor
		
		unsigned int battery_level(); // Returns the current Battery Level as a percent (0-100)

		bool buttonA();  // true if A is pressed (top button)
		bool buttonB();  // true if B is pressed (bottom button)
		bool buttonAB(); // true if A AND B are pressed
		bool buttonU();  // true if Up button is pressed
		bool buttonD();  // true if Down is pressed
		bool buttonL();  // true if Left is pressed
		bool buttonR();  // true if Right is pressed

		bool buttonA_repeat(unsigned int dtime);  // True every [dtime] ms
		bool buttonB_repeat(unsigned int dtime);  // True every [dtime] ms
		bool buttonAB_repeat(unsigned int dtime); // True every [dtime] ms
		bool buttonU_repeat(unsigned int dtime);  // True every [dtime] ms
		bool buttonD_repeat(unsigned int dtime);  // True every [dtime] ms
		bool buttonL_repeat(unsigned int dtime);  // True every [dtime] ms
		bool buttonR_repeat(unsigned int dtime);  // True every [dtime] ms

		bool buttonA_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonB_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonAB_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonU_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonD_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonL_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State
		bool buttonR_debounce(unsigned int dtime = DEBOUNCEMILLIS);  // True once after [dtime] ms of High State

		bool grayscale; // Set to false for B/W (ORs both bits for each pixel), default true
		unsigned int frameBuffer[16][2]; // FrameBuffer for LED Array
		void processFB(); // Redraw the frameBuffer to the badge

		void setPixel(unsigned int x, unsigned int y, unsigned int brightness); // Set a pixel
		unsigned int getPixel(unsigned int x, unsigned int y);                  // Returns the value of a pixel
		void invert(); // Inverts the frameBuffer
		void clear();  // Clears the frameBuffer
		void setDropFrames(unsigned int newDropFrameCounter);
		unsigned int getDropFrames();
		
	private:
		int currentCol; // currentRow (0 to 15)
		unsigned int currentIteration; // dimming interation count
		
		bool repeat(bool current_value, long & lastVar, unsigned int dtime);  // True every [dtime] ms
		bool debounce(bool current_value, long & lastVar, unsigned int dtime);  // True every [dtime] ms

		long lastButtonA;  // A debounce counter
		long lastButtonB;  // B debounce counter
		long lastButtonAB; // A && B debounce counter (probably for menus, etc)
		long lastButtonU;  // Up debounce counter
		long lastButtonD;  // Down debounce counter
		long lastButtonL;  // Left debounce counter
		long lastButtonR;  // Right debounce counter

		unsigned int scanRetard;
		unsigned int dropFrameCounter;
};

#endif
