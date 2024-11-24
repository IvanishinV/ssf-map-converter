
#include "stdafx.h"

#include "Mis_objects.h"
#include "General.h"
#include "Displayinfo.h"


void covertMisObjects(const std::vector<uint8_t>& mis_objects)
{
	//------------------------------------------------------------------------------
	std::ofstream outputFileMisObjects("map.000/mis.000/objs", std::ios::binary);
	if (!outputFileMisObjects)
	{
		errorBuildFile();
		return;
	}
	//------------------------------------------------------------------------------
	const size_t maxTries = mis_objects.size() / 2;
	size_t curTry{ 0 };

	while (curTry < maxTries)
	{
		uint8_t num1 = mis_objects[curTry * 2];
		uint8_t num2 = mis_objects[curTry * 2 + 1];
		outputFileMisObjects << num1 << num2 << GLOBALNULL << GLOBALNULL;
	}
	outputFileMisObjects.close();
	//------------------------------------------------------------------------------
	return;
}