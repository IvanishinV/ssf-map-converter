#include <iostream>
#include <fstream>

#include "Mis_desc.h"
#include "Displayinfo.h"

void covertMisDesc()
{
	std::ofstream outputFileMisDesc("map.000/mis.000/desc", std::ios::binary);
	if (!outputFileMisDesc)
	{
		erorbuildfile();
		return;
	}
	outputFileMisDesc << "good convert mission";
	outputFileMisDesc.close();
}