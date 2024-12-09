
#include "stdafx.h"

#include "Map_objects.h"
#include "Displayinfo.h"

void covertMapObjects(const std::vector<uint8_t>& map_objects)
{
	std::ofstream outputMapObjects("map.000/objects", std::ios::binary);
	if (!outputMapObjects)
	{
		errorWriteFile();
		return;
	}
	outputMapObjects.write((char*)map_objects.data(), map_objects.size());
//	outputMapObjects << map_objects.view();
	outputMapObjects.close();
}