
#include "stdafx.h"

#include "Mis_mult.h"
#include "General.h"
#include "Displayinfo.h"

void covertMisMult(const std::vector<uint8_t>& map_info, const uint32_t maptypeheader)
{
	std::ofstream outputMapMisMult("map.000/mis.000/mismult", std::ios::binary);
	if (!outputMapMisMult)
	{
		errorBuildFile();
		return;
	}

	if(maptypeheader == 0 || maptypeheader == 1)
	{ 
		outputMapMisMult.write((char*)map_info.data() + 24, 16);
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