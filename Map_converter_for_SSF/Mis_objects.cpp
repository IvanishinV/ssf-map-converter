
#include "stdafx.h"

#include "Mis_objects.h"
#include "util.h"
#include "Displayinfo.h"


void covertMisObjects(const std::vector<uint8_t>& mis_objects)
{
	//------------------------------------------------------------------------------
	std::ofstream outputFileMisObjects("map.000/mis.000/objs", std::ios::binary);
	if (!outputFileMisObjects)
	{
		errorWriteFile();
		return;
	}
	//------------------------------------------------------------------------------
	const size_t maxTries = mis_objects.size() / 2;
	for (size_t i = 0; i < maxTries; ++i)
	{
		uint8_t num1 = mis_objects[i * 2];
		uint8_t num2 = mis_objects[i * 2 + 1];
		outputFileMisObjects << num1 << num2 << GLOBALNULL << GLOBALNULL;
	}
	outputFileMisObjects.close();
	//------------------------------------------------------------------------------
	return;
}