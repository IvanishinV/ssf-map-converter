#include <iostream> 
#include <sstream>
#include <fstream> 
#include "Desert.h"
#include "General.h"

void desertrhombs(std::stringstream& map_rhombs, std::ofstream& outputFile)
{
	char buffer[2];
	while (map_rhombs.read(buffer, sizeof(buffer)))
	{
		uint8_t num1 = *(uint8_t*)(buffer + 0);
		uint8_t num2 = *(uint8_t*)(buffer + 1);

		//Tile_col это байт €ркости
		uint8_t Tile_col = num2 / 8;
		if (num2 % 2 == 0)
		{
			if (num1 >= 0 && num1 <= 255)
			{
				//GRASS1+++++++
				//---------------------------------------------------------------------------------------------------------
				if (num1 >= 0 && num1 <= 9)
				{
					uint8_t Tile_num = 0x00;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//GRASS2
				//---------------------------------------------------------------------------------------------------------
				//FIELD+++++++
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 == 26 || num1 == 27 || num1 == 28 || num1 >= 41 && num1 <= 47)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 30 || num1 == 49 || num1 == 61 || num1 == 73)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 37 || num1 == 56 || num1 == 68 || num1 == 80)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 29 || num1 == 48 || num1 == 60 || num1 == 72)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 39 || num1 == 58 || num1 == 70 || num1 == 82)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 31 || num1 == 50 || num1 == 62 || num1 == 74)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 38 || num1 == 57 || num1 == 69 || num1 == 81)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 32 || num1 == 51 || num1 == 63 || num1 == 75)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 40 || num1 == 59 || num1 == 71 || num1 == 83)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 36 || num1 == 55 || num1 == 67 || num1 == 79)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 33 || num1 == 52 || num1 == 64 || num1 == 76)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 35 || num1 == 54 || num1 == 66 || num1 == 78)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 34 || num1 == 53 || num1 == 65 || num1 == 77)
				{
					uint8_t Tile_num = 0x28;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//SWAMP+++++++
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 144 && num1 <= 163)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 85 || num1 == 97 || num1 == 109 || num1 == 121 || num1 == 133)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 92 || num1 == 114 || num1 == 116 || num1 == 128 || num1 == 140)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 84 || num1 == 96 || num1 == 108 || num1 == 120 || num1 == 132)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 94 || num1 == 106 || num1 == 118 || num1 == 130 || num1 == 142)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 86 || num1 == 98 || num1 == 110 || num1 == 122 || num1 == 134)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 93 || num1 == 105 || num1 == 117 || num1 == 129 || num1 == 141)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 87 || num1 == 99 || num1 == 111 || num1 == 123 || num1 == 135)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 95 || num1 == 107 || num1 == 119 || num1 == 131 || num1 == 143)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 91 || num1 == 113 || num1 == 115 || num1 == 127 || num1 == 139)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 88 || num1 == 100 || num1 == 112 || num1 == 124 || num1 == 136)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 90 || num1 == 102 || num1 == 114 || num1 == 126 || num1 == 138)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 89 || num1 == 101 || num1 == 113 || num1 == 125 || num1 == 137)
				{
					uint8_t Tile_num = 0x37;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//GROUND1
				//---------------------------------------------------------------------------------------------------------
				//GROUND2
				//---------------------------------------------------------------------------------------------------------
				else
				{
					uint8_t Tile_num = 0x00;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
			}
		}
		else
		{
			if (num1 >= 0 && num1 <= 147)
			{
				//GROUND2
				//---------------------------------------------------------------------------------------------------------	
				//WATER+++++++
				//---------------------------------------------------------------------------------------------------------
				if (num1 >= 33 && num1 <= 40)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 42 || num1 == 54 || num1 == 66 || num1 == 78 || num1 == 90) 
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 49 || num1 == 61 || num1 == 73 || num1 == 85 || num1 == 97)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 41 || num1 == 53 || num1 == 65 || num1 == 77 || num1 == 89)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 51 || num1 == 63 || num1 == 75 || num1 == 87 || num1 == 99)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 43 || num1 == 55 || num1 == 67 || num1 == 79 || num1 == 91)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 50 || num1 == 62 || num1 == 74 || num1 == 86 || num1 == 98)
				{
				uint8_t Tile_num = 0x66;
				uint8_t Tile_type = 0x06;
				outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 44 || num1 == 56 || num1 == 68 || num1 == 80 || num1 == 92)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 52 || num1 == 64 || num1 == 76 || num1 == 88 || num1 == 100)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 48 || num1 == 60 || num1 == 72 || num1 == 84 || num1 == 96)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 45 || num1 == 57 || num1 == 69 || num1 == 81 || num1 == 93)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 47 || num1 == 59 || num1 == 71 || num1 == 83 || num1 == 95)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 46 || num1 == 58 || num1 == 70 || num1 == 82 || num1 == 94)
				{
					uint8_t Tile_num = 0x66;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				//SAND+++++++
				//---------------------------------------------------------------------------------------------------------
				else
				if (num1 >= 103 && num1 <= 112 || num1 == 145 || num1 == 146 || num1 == 147)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 101 || num1 == 102 || num1 == 119)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x01;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 123 || num1 == 141 || num1 == 142) 
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x02;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 113 || num1 == 114 || num1 == 115) 
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x03;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 130 || num1 == 144)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x04;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 120 || num1 == 121 || num1 == 122)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x05;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 124 || num1 == 139 || num1 == 140)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x06;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 116 || num1 == 117 || num1 == 118)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x07;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 129 || num1 == 143)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x08;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 126 || num1 == 135 || num1 == 136)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x09;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 128 || num1 == 131 || num1 == 132)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0A;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 125 || num1 == 137 || num1 == 138)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0B;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				if (num1 == 127 || num1 == 133 || num1 == 134)
				{
					uint8_t Tile_num = 0x78;
					uint8_t Tile_type = 0x0C;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
				else
				{
					uint8_t Tile_num = 0x00;
					uint8_t Tile_type = 0x00;
					outputFile << Tile_num << Tile_type << Tile_col << GLOBALNULL;
				}
			}
		}
	}

}