
#include "stdafx.h"

#include "Mis_info.h"
#include "General.h"
#include "Displayinfo.h"


#pragma pack(push, 1)
struct MIS_INFO_HEADER
{
	private:
	uint32_t unknown_1;
	uint32_t unknown_2;
	uint32_t mapSizeU;
	uint32_t mapSizeV;
	public:
	MIS_INFO_HEADER(uint32_t mapSizeU, uint32_t mapSizeV)
	{
		unknown_1 = GLOBALNULL;
		unknown_2 = GLOBALNULL;
		this->mapSizeU = mapSizeU;
		this->mapSizeV = mapSizeV;
	}
};
#pragma pack (pop)


void covertMisInfo(uint32_t mapSizeU, uint32_t mapSizeV)
{
	std::ofstream outputMisInfo("map.000/mis.000/info", std::ios::binary);
	if (!outputMisInfo)
	{
		errorBuildFile();
		return;
	}

	MIS_INFO_HEADER part1(mapSizeU, mapSizeV);
	outputMisInfo.write((char*)&part1, sizeof(part1));
	outputMisInfo.close();
}