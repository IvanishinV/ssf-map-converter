#include <sstream>
#include <fstream> 

#include "Map_objects.h"
#include "Displayinfo.h"

void covertMapObjects(std::stringstream& map_objects)
{
	std::ofstream outputMapObjects("map.000/objects", std::ios::binary);
	if (!outputMapObjects)
	{
		erorbuildfile();
		return;
	}
	outputMapObjects << map_objects.str();
	outputMapObjects.close();
}