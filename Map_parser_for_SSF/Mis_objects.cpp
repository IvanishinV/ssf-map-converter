#include <iostream>
#include <sstream>
#include <fstream> 

#include "Mis_objects.h"
#include "General.h"
#include "Displayinfo.h"


void covertMisObjects(std::stringstream& mis_objects)
{
	//------------------------------------------------------------------------------
	std::ofstream outputFileMisObjects("map.000/mis.000/objs", std::ios::binary);
	if (!outputFileMisObjects)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	char buffer[2];
	while (mis_objects.read(buffer, sizeof(buffer)))
	{
		uint8_t num1 = *(uint8_t*)(buffer + 0);
		uint8_t num2 = *(uint8_t*)(buffer + 1);
		outputFileMisObjects << num1 << num2 << GLOBALNULL << GLOBALNULL;
	}
	outputFileMisObjects.close();
	//------------------------------------------------------------------------------
	return;
}