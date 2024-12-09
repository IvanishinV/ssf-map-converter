
#include "stdafx.h"

#include "Map_rhombs.h"
#include "Displayinfo.h"

void covertMapRhombs(const std::vector<uint8_t>& map_rhombs, const uint32_t mapIdentifier)
{
	std::ofstream outputFile("map.000/rhombs", std::ios::binary);
	if (!outputFile)
	{
		errorWriteFile();
		return;
	}
	//---------------------------------------------------------------------------------------------------------
	switch (mapIdentifier)
	{
	case 0:
		summerrhombs(map_rhombs, outputFile);
		break;
	case 1:
		winterrhombs(map_rhombs, outputFile);
		break;
	case 2:
		beachrhombs(map_rhombs, outputFile);
		break;
	case 3:
		desertrhombs(map_rhombs, outputFile);
		break;
	default:
		break;
	};

	outputFile.close();
	return;
}

