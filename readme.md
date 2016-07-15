# OpenWest HackCenter 2016 Electronic Badge
Thanks to everyone for another fun year! :D

##Programming The Badge 
- [Download and install the Arduino IDE](https://www.arduino.cc/en/Main/Software)
  - Arch Linux: [Arch Wiki: Arduino#Installation](https://wiki.archlinux.org/index.php/Arduino#Installation)
  - Ubuntu Linux: [Install Arduino on Ubuntu Linux](http://playground.arduino.cc/Linux/Ubuntu)
- Connect the Badge Via a MicroUSB cable (you will need a low profile cable, or you may damage the connector)
- Install the USB Drives
- Select Tools->Board->Arduino Leonardo
- [Get to programmin's!](https://www.arduino.cc/en/Tutorial/HomePage)

##Using the badge
Standard Operation:
- Main Function
    - Button 1 (Top Button): Display Scrolling Text 1
    - Button 2 (Bottom Button): Display Scrolling Text 2
    - Down button: Show battery percentage
    - Hold Right Button: Fast scrolling
    - Hold Both Buttons (1 and 2): Enter Menu Mode
- Menu
    - Select an option using the Up and Down, Press button 1 to Select
	- Current Menu options
		- 1) Customize Text 1
		- 2) Clear Text 1
		- 3) Customize Text 2
		- 4) Clear Text 2
		- 5) Tetris (under development, check github for update code)
		- 6) Nibble
		- 7) Set Brightness (Lower saves Battery-Life)
		- 0) Exit Menu
- Customize Text
	- Use Left and Right to Select a Character Space
	- Use Up and Down to Select a Character
	- Button 1 (Top Button) Saves Current Text
	- Button 2 (Bottom Button) Deletes the Current Character
- Nibble
	- Eat those blocks. Ahhhh yisss!
	- Hold Both Buttons (or die) to exit game
	- Scoring / High Scores Coming Soon
- Set Brightness
	- Use Up and Down to select the brightness you would like
	- Press Button 1 to save the selected brightness

##Troubleshooting
- Text freezes or disappears
	- Low Batteries - Replace the batteries
