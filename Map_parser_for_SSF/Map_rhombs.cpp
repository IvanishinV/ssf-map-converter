#include <iostream> 
#include <sstream>
#include <fstream> 
#include <string>

#include "Map_rhombs.h"
#include "Displayinfo.h"

void covertMapRhombs(std::stringstream& map_rhombs, uint32_t mapIdentifier)
{
	std::ofstream outputFile("map.000/rhombs", std::ios::binary);
	if (!outputFile)
	{
		erorbuildfile();
		return;
	}
		//---------------------------------------------------------------------------------------------------------
		if(mapIdentifier == 0)
		{
			summerrhombs (map_rhombs, outputFile);
		}
		else
		if (mapIdentifier == 1)
		{
			winterrhombs(map_rhombs, outputFile);
		}
		else
		if (mapIdentifier == 2)
		{
			beachrhombs (map_rhombs, outputFile);
		}
		else
		if (mapIdentifier == 3)
		{
			desertrhombs(map_rhombs, outputFile);
		}
	
	outputFile.close();
	return;
}

