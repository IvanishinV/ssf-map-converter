#include <sstream>
#include <fstream> 
#include <iostream>

#include "Mis_zones.h"
#include "General.h"
#include "Displayinfo.h"

void covertMisZones(std::stringstream& mis_zones, uint32_t mapSizeU)
{
	std::ofstream outputFileMisZones("map.000/mis.000/locations", std::ios::binary);
	if (!outputFileMisZones)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	std::stringstream MisZones;
	//------------------------------------------------------------------------------
	char* buffer = new char[mapSizeU];
	while (mis_zones.read(buffer, mapSizeU))
	{
		for (uint32_t i = 0; i < mapSizeU; i++)
		{
			uint8_t num1 = *(uint8_t*)(buffer + i);
			MisZones << num1;
		}
		uint32_t dummyarray1 = 512 - mapSizeU;
		for (uint32_t n = 0; n < dummyarray1; n++)
		{
			MisZones << GLOBALNULL;
		}
	}
	delete[] buffer;
	//------------------------------------------------------------------------------
	uint32_t dummyarray2 = LOCATIONSSIZE - 512 * mapSizeU;
	for (uint32_t n = 0; n < dummyarray2; n++)
	{
		MisZones << GLOBALNULL;
	}
	//------------------------------------------------------------------------------
	outputFileMisZones << MisZones.str();
	MisZones.str("");
	outputFileMisZones.close();
	//------------------------------------------------------------------------------
	return;
}



