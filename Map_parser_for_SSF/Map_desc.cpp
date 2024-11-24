#include <fstream>

#include "Map_desc.h"
#include "Displayinfo.h"

void covertMapDesc(const std::string& convertMapName)
{
	std::ofstream outputFileMapDesc("map.000/desc", std::ios::binary);
	if (!outputFileMapDesc)
	{
		erorbuildfile();
		return;
	}
	outputFileMapDesc << convertMapName;
	outputFileMapDesc.close();
}