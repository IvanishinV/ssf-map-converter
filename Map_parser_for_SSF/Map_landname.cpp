
#include "stdafx.h"

#include "Map_landname.h"
#include "Displayinfo.h"

void covertMapLandname(const std::vector<uint8_t>& map_landname)
{
	std::ofstream outputMapLandname("map.000/landname", std::ios::binary);
	if (!outputMapLandname)
	{
		errorBuildFile();
		return;
	}

	outputMapLandname.write((char*)map_landname.data(), map_landname.size());
	outputMapLandname.close();
}