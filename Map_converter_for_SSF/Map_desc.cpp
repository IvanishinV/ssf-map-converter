
#include "stdafx.h"

#include "Map_desc.h"
#include "Displayinfo.h"

void covertMapDesc()
{
	std::ofstream outputFileMapDesc("map.000/desc", std::ios::binary);
	if (!outputFileMapDesc)
	{
		errorWriteFile();
		return;
	}
	outputFileMapDesc.close();
}