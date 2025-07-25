
#include "stdafx.h"

#include "Map_info.h"
#include "util.h"
#include "Displayinfo.h"

#pragma pack(push, 1)
struct MAP_INFO_HEADER
{
	MAP_INFO_HEADER(uint32_t mapIdentifier, uint32_t mapSizeU, uint32_t mapSizeV)
	{
		this->mapIdentifier = mapIdentifier;
		this->mapSizeU = mapSizeU;
		this->mapSizeV = mapSizeV;
		unknown_1 = GLOBALNULL;
		unknown_2 = GLOBALNULL;
		unknown_3 = GLOBALNULL;
	}

private:
	uint32_t mapIdentifier;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
	uint8_t unknown_1;
	uint8_t unknown_2;
	uint8_t unknown_3;
};
#pragma pack (pop)

void covertMapInfo(uint32_t mapIdentifier, uint32_t mapSizeU, uint32_t mapSizeV)
{
	std::ofstream outputMapInfo("map.000/info", std::ios::binary);
	if (!outputMapInfo)
	{
		errorWriteFile();
		return;
	}
	MAP_INFO_HEADER part1(mapIdentifier, mapSizeU, mapSizeV);
	outputMapInfo.write((char*)&part1, sizeof(part1));
	outputMapInfo.close();
}