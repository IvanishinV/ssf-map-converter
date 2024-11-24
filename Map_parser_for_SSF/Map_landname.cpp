#include <sstream>
#include <fstream> 

#include "Map_landname.h"
#include "Displayinfo.h"

void covertMapLandname(std::stringstream& map_landname)
{
	std::ofstream outputMapLandname("map.000/landname", std::ios::binary);
	if (!outputMapLandname)
	{
		erorbuildfile();
		return;
	}
	outputMapLandname << map_landname.str();
	outputMapLandname.close();
}