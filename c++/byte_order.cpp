
#include <string.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <limits>

void byte_alignment()
{
  // A short is represented as 16 bits 
  // HEX 0x0000 = 0000 | 0000 | 0000 | 0000 
  //
  // HEX 0x0102 = 0000 | 0001        |  0000 | 0010
  //              ------------ 
  //              High Order Byte       Low Order Byte
  //                 
  // Does the system store the bytes for low to high 

  union
  {
    short  s;
    char   c[sizeof(short)];
  } un;

  un.s = 0x0102;

  if ( sizeof(short) != 2 ) return;

  if ( un.c[0] == 1 && un.c[1] == 2 )
    {
      // the memory goes high order byte to low order byte
      // the lower order byte is after the higher order byte
      std::cout << "big-endian x0" << std::hex << un.s 
		<< " represented as " << (int)un.c[0] << " " << (int)un.c[1] << std::endl;
    }
  else if ( un.c[0] == 2 && un.c[1] == 1 )
    {
      // the meory goes from low order byte to high order byte
      std::cout << "little-endian x0" << std::hex << un.s 
		<< " represented as " << (int)un.c[0] << " " << (int)un.c[1] << std::endl;
      std::cout << std::setiosflags( std::ios_base::dec )<< std::endl;
    }
}

void bit_store()
{
  // this example show how the bits are represented for a short
    union
  {
    short s;
    char  c[sizeof(short)];
  } un ;

  // Positive numbers
  //
  // Range 0x7FFF to 0x0001
  // 
  // 0111 | 1111 | 1111 | 1111 = 0x7FFF = 32767
  // 0100 | 0000 | 0000 | 0000 = 0x4000 = 16384
  // 0010 | 0000 | 0000 | 0000 = 0x2000 =  8192
  // 0001 | 0000 | 0000 | 0000 = 0x1000 =  4096
  // 0000 | 1111 | 1111 | 1111 = 0x0FFF =  4095
  // 0000 | 0000 | 0000 | 0001 = 0x0001 =     1
    
  std::cout << "positive number range from 1 to " << std::numeric_limits<short>::max() << std::endl;
  un.s = 0x7FFF;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x7000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x4000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x2000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x1000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x0FFF;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x000F;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0x0001;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;


  // Negative Numbers
  // Range = 0x8000 to 0xFFFF
  //
  // 1000 | 0000 | 0000 | 0000 = 0x8000 = -32768
  // 1100 | 0000 | 0000 | 0000 = 0xC000 = -16384
  // 1111 | 1110 | 0000 | 0000 = 0xFE00 = -512
  // 1111 | 1111 | 0000 | 0000 = 0xFF00 = -256
  // 1111 | 1111 | 1111 | 0000 = 0xFFF0 = -16
  // 1111 | 1111 | 1111 | 0001 = 0xFFF1 = -15
  // 1111 | 1111 | 1111 | 1000 = 0xFFF8 = -8
  // 1111 | 1111 | 1111 | 1100 = 0xFFFC = -4
  // 1111 | 1111 | 1111 | 1101 = 0xFFFD = -3
  // 1111 | 1111 | 1111 | 1111 = 0xFFFF = -1

  std::cout << std::endl;
  std::cout << "negative number range from " << std::numeric_limits<short>::min() << " to -1" << std::endl;

  un.s = 0x8000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xC000;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFE00;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFF00;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFF0;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFF1;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFF8;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFFC;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFFD;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;

  un.s = 0xFFFF;
  std::cout << "0x" << std::hex << un.s << " = " << std::dec << un.s << std::endl;
}


int main( int , char ** )
{
  std::cout << "determine byte alignment" << std::endl;
  byte_alignment();

  std::cout << "show bit representation" << std::endl;
  bit_store();
  return 0;
}
