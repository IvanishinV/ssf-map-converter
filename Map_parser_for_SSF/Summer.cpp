#include <iostream> 
#include <sstream>
#include <fstream> 
#include "Summer.h"
#include "General.h"

void summerrhombs(std::stringstream& map_rhombs, std::ofstream& outputFile)
{
	char buffer[2];
	while (map_rhombs.read(buffer, sizeof(buffer)))
	{
		uint8_t num1 = *(uint8_t*)(buffer + 0);
		uint8_t num2 = *(uint8_t*)(buffer + 1);

		// Tile_col это байт €ркости
		uint8_t Tile_col = (uint16_t)num2 / 8;
		if (num2 % 2 == 0)
		{
			if (num1 >= 0 && num1 <= 255)
			{
				//GRASS1
				//---------------------------------------------------------------------------------------------------------
				if (num1 >= 0 && num1 <= 17)
				{
					uint8_t Tile_num = 0x00;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//GRASS2
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 30 && num1 <= 38)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 19)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 26)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 21)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 27)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 20)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 29)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 18)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 28)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 24)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 22)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 23)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 25)
				{
					uint8_t Tile_num = 0x14;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//FIELD
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 == 39 || num1 == 40 || num1 == 41 || num1 >= 54 && num1 <= 60)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 43 || num1 == 62 || num1 == 74 || num1 == 86)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 50 || num1 == 69 || num1 == 81 || num1 == 93)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 42 || num1 == 61 || num1 == 73 || num1 == 85)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 52 || num1 == 71 || num1 == 83 || num1 == 95)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 44 || num1 == 63 || num1 == 75 || num1 == 87)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 51 || num1 == 70 || num1 == 82 || num1 == 94)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 45 || num1 == 64 || num1 == 76 || num1 == 88)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 53 || num1 == 72 || num1 == 84 || num1 == 96)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 49 || num1 == 68 || num1 == 80 || num1 == 92)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 46 || num1 == 65 || num1 == 77 || num1 == 89)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 48 || num1 == 67 || num1 == 79 || num1 == 91)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 47 || num1 == 66 || num1 == 78 || num1 == 90)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//SWAMP
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 157 && num1 <= 176)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 98 || num1 == 110 || num1 == 122 || num1 == 134 || num1 == 146)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 105 || num1 == 117 || num1 == 129 || num1 == 141 || num1 == 153)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 97 || num1 == 109 || num1 == 121 || num1 == 133 || num1 == 145)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 107 || num1 == 119 || num1 == 131 || num1 == 143 || num1 == 155)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 99 || num1 == 111 || num1 == 123 || num1 == 135 || num1 == 147)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 106 || num1 == 118 || num1 == 130 || num1 == 142 || num1 == 154)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 100 || num1 == 112 || num1 == 124 || num1 == 136 || num1 == 148)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 108 || num1 == 120 || num1 == 132 || num1 == 144 || num1 == 156)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 104 || num1 == 116 || num1 == 128 || num1 == 140 || num1 == 152)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 101 || num1 == 113 || num1 == 125 || num1 == 137 || num1 == 149)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 103 || num1 == 115 || num1 == 127 || num1 == 139 || num1 == 151)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 102 || num1 == 114 || num1 == 126 || num1 == 138 || num1 == 150)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//GROUND1
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 189 && num1 <= 200)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 179)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 181)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 177)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 182)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 178)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 184)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 180)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 183)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 187)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 185)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 186)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 188)
				{
					uint8_t Tile_num = 0x46;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//GROUND2
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 201 && num1 <= 209)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 211 || num1 == 223 || num1 == 235 || num1 == 247)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 218 || num1 == 230 || num1 == 242 || num1 == 254)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 210 || num1 == 222 || num1 == 234 || num1 == 246)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 220 || num1 == 232 || num1 == 244)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 212 || num1 == 224 || num1 == 236 || num1 == 248)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 219 || num1 == 231 || num1 == 243 || num1 == 255)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 213 || num1 == 225 || num1 == 237 || num1 == 249)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 221 || num1 == 233 || num1 == 245)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 217 || num1 == 229 || num1 == 241 || num1 == 253)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 214 || num1 == 226 || num1 == 238 || num1 == 250)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 216 || num1 == 228 || num1 == 240 || num1 == 252)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 215 || num1 == 227 || num1 == 239 || num1 == 251)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
			}
		}
		else
		{
			if (num1 >= 0 && num1 <= 172)
			{
				//GROUND2
				//---------------------------------------------------------------------------------------------------------	
				if (num1 == 3)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 10)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 2)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 0 || num1 == 12)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 4)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 11)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 5)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 1 || num1 == 13)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 9)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 6)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 8)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 7)
				{
					uint8_t Tile_num = 0x55;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}

				//WATER
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 54 && num1 <= 61)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;

				}
				else
				if (num1 == 15 || num1 == 27 || num1 == 39 || num1 == 51 || num1 == 71 || num1 == 83 || num1 == 94 || num1 == 104)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 22 || num1 == 34 || num1 == 46 || num1 == 66 || num1 == 78 || num1 == 90 || num1 == 100 || num1 == 110)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 14 || num1 == 26 || num1 == 38 || num1 == 50 || num1 == 70 || num1 == 82 || num1 == 103)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 24 || num1 == 36 || num1 == 48 || num1 == 68 || num1 == 80 || num1 == 92)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 16 || num1 == 28 || num1 == 40 || num1 == 52 || num1 == 72 || num1 == 84 || num1 == 95 || num1 == 105)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 23 || num1 == 35 || num1 == 47 || num1 == 67 || num1 == 79 || num1 == 91 || num1 == 101 || num1 == 111)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 17 || num1 == 29 || num1 == 41 || num1 == 53 || num1 == 73 || num1 == 85 || num1 == 96)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 25 || num1 == 37 || num1 == 49 || num1 == 69 || num1 == 81 || num1 == 93 || num1 == 102)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 21 || num1 == 33 || num1 == 45 || num1 == 65 || num1 == 77 || num1 == 89 || num1 == 109)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 18 || num1 == 30 || num1 == 42 || num1 == 62 || num1 == 74 || num1 == 86 || num1 == 97 || num1 == 106)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 20 || num1 == 32 || num1 == 44 || num1 == 64 || num1 == 76 || num1 == 88 || num1 == 99 || num1 == 108)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 19 || num1 == 31 || num1 == 43 || num1 == 63 || num1 == 75 || num1 == 87 || num1 == 98 || num1 == 107)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//SAND
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 112 && num1 <= 123 || num1 == 172)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 125 || num1 == 137 || num1 == 149 || num1 == 161)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 132 || num1 == 144 || num1 == 156 || num1 == 168)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 124 || num1 == 136 || num1 == 148 || num1 == 160)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 134 || num1 == 146 || num1 == 158 || num1 == 170)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 126 || num1 == 138 || num1 == 150 || num1 == 162)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 133 || num1 == 145 || num1 == 157 || num1 == 169)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 127 || num1 == 139 || num1 == 151 || num1 == 163)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 135 || num1 == 147 || num1 == 159 || num1 == 171)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 131 || num1 == 143 || num1 == 155 || num1 == 167)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 128 || num1 == 140 || num1 == 152 || num1 == 164)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 130 || num1 == 142 || num1 == 154 || num1 == 166)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 129 || num1 == 141 || num1 == 153 || num1 == 165)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
			}
		}
	}
}