#include <iostream>
#include <sstream>
#include <fstream> 
#include <windows.h>

#include "Mis_mines.h"
#include "General.h"
#include "Displayinfo.h"

void convertMisMines(std::stringstream& map_mines, uint32_t mapSizeU)
{
	std::ofstream outputFileMisMines("map.000/mis.000/mines", std::ios::binary);
	if (!outputFileMisMines)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	std::stringstream MisMines;
	//------------------------------------------------------------------------------
	char* buffer = new char[mapSizeU];
	while (map_mines.read(buffer, mapSizeU))
	{
		for (uint32_t i = 0; i < mapSizeU; i++)
		{
			uint8_t mines = *(uint8_t*)(buffer + i);
			MisMines << mines;
		}
		uint32_t dummyarray1 = 512 - mapSizeU;
		for (uint32_t n = 0; n < dummyarray1; n++)
		{
			MisMines << GLOBALNULL;
		}
	}
	delete[] buffer;
	//------------------------------------------------------------------------------
	uint32_t dummyarray2 = LOCATIONSSIZE - 512 * mapSizeU;
	for (uint32_t n = 0; n < dummyarray2; n++)
	{
		MisMines << GLOBALNULL;
	}
	//------------------------------------------------------------------------------
	outputFileMisMines << MisMines.str();
	MisMines.str("");
	outputFileMisMines.close();
	//------------------------------------------------------------------------------
	return;
}