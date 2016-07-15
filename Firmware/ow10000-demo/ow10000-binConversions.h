/*
  Filename: ow10000-binConversions.cpp
  Purpose: Decimal and Binary conversions
  Created: July 15, 2016
*/

int btoi(byte b){
  switch(b){
    case(0b00000000):
      return 0;
      break;
    
    case(0b00000001):
      return 1;
      break;
    
    case(0b00000010):
      return 2;
      break;
    
    case(0b00000011):
      return 3;
      break;
    
    case(0b00000100):
      return 4;
      break;
    
    case(0b00000101):
      return 5;
      break;
    
    case(0b00000110):
      return 6;
      break;
    
    case(0b00000111):
      return 7;
      break;
    
    case(0b00001000):
      return 8;
      break;
    
    case(0b00001001):
      return 9;
      break;
    
    case(0b00001010):
      return 10;
      break;
    
    case(0b00001011):
      return 11;
      break;
    
    case(0b00001100):
      return 12;
      break;
    
    case(0b00001101):
      return 13;
      break;
    
    case(0b00001110):
      return 14;
      break;
    
    case(0b00001111):
      return 15;
      break; 
  }
}

byte itob(int i){
  switch(i){
    case(0):
      return 0b00000000;
      break;

    case(1):
      return 0b00000001;
      break;

    case(2):
      return 0b00000010;
      break;

    case(3):
      return 0b00000011;
      break;

    case(4):
      return 0b00000100;
      break;

    case(5):
      return 0b00000101;
      break;

    case(6):
      return 0b00000110;
      break;

    case(7):
      return 0b00000111;
      break;

    case(8):
      return 0b00001000;
      break;

    case(9):
      return 0b00001001;
      break;

    case(10):
      return 0b00001010;
      break;

    case(11):
      return 0b00001011;
      break;

    case(12):
      return 0b00001100;
      break;

    case(13):
      return 0b00001101;
      break;

    case(14):
      return 0b00001110;
      break;

    case(15):
      return 0b00001111;
      break;

    case(16):
      return 0b00010000;
      break;

    case(17):
      return 0b00010001;
      break;

    case(18):
      return 0b00010010;
      break;

    case(19):
      return 0b00010011;
      break;

    case(20):
      return 0b00010100;
      break;

    case(21):
      return 0b00010101;
      break;

    case(22):
      return 0b00010110;
      break;

    case(23):
      return 0b00010111;
      break;

    case(24):
      return 0b00011000;
      break;

    case(25):
      return 0b00011001;
      break;

    case(26):
      return 0b00011010;
      break;

    case(27):
      return 0b00011011;
      break;

    case(28):
      return 0b00011100;
      break;

    case(29):
      return 0b00011101;
      break;

    case(30):
      return 0b00011110;
      break;

    case(31):
      return 0b00011111;
      break;
  }
}

