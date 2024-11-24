#include <iostream> 
#include <sstream>
#include <fstream> 

#include "Mis_mult.h"
#include "General.h"
#include "Displayinfo.h"

void covertMisMult(std::stringstream& map_info, uint32_t maptypeheader)
{
	std::ofstream outputMapMisMult("map.000/mis.000/mismult", std::ios::binary);
	if (!outputMapMisMult)
	{
		erorbuildfile();
		return;
	}

	if(maptypeheader == 0 || maptypeheader == 1)
	{ 
		map_info.seekg(24);
		char buffer[16];
		map_info.read(buffer, 16);
		outputMapMisMult.write(buffer, 16);
	}
	else
	{
		for (uint32_t n = 0; n < 16; n++)
		{
			outputMapMisMult << GLOBALNULL;
		}
	}
	outputMapMisMult.close();
}