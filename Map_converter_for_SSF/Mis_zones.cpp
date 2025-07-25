
#include "stdafx.h"

#include "Mis_zones.h"
#include "util.h"
#include "Displayinfo.h"

void covertMisZones(const std::vector<uint8_t>& mis_zones, const uint32_t mapSizeU)
{
	std::ofstream outputFileMisZones("map.000/mis.000/locations", std::ios::binary);
	if (!outputFileMisZones)
	{
		errorWriteFile();
		return;
	}

	std::vector<uint8_t> misZones(LOCATIONSSIZE, 0);

	const size_t maxTries = mis_zones.size() / mapSizeU;
	for (size_t i = 0; i < maxTries; ++i)
	{
		std::copy(mis_zones.cbegin() + i * mapSizeU, mis_zones.cbegin() + i * mapSizeU + mapSizeU, misZones.begin() + 512 * i);
	}

	outputFileMisZones.write((char*)misZones.data(), misZones.size());
	outputFileMisZones.close();
}



