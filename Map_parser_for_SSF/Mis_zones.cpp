
#include "stdafx.h"

#include "Mis_zones.h"
#include "General.h"
#include "Displayinfo.h"

void covertMisZones(const std::vector<uint8_t>& mis_zones, const uint32_t mapSizeU)
{
	std::ofstream outputFileMisZones("map.000/mis.000/locations", std::ios::binary);
	if (!outputFileMisZones)
	{
		errorBuildFile();
		return;
	}

	std::vector<uint8_t> misZones(LOCATIONSSIZE, 0);

	const size_t maxTries = mis_zones.size() / mapSizeU;
	size_t i{ 0 };
	while (i < maxTries)
	{
		std::copy(mis_zones.cbegin() + i * mapSizeU, mis_zones.cbegin() + i * mapSizeU + mapSizeU, misZones.begin() + 512 * i);
	}

	outputFileMisZones.write((char*)misZones.data(), misZones.size());
	outputFileMisZones.close();
}



