
#include "stdafx.h"

#include "Mis_mines.h"
#include "General.h"
#include "Displayinfo.h"

void convertMisMines(const std::vector<uint8_t>& map_mines, const uint32_t mapSizeU)
{
	std::ofstream outputFileMisMines("map.000/mis.000/mines", std::ios::binary);
	if (!outputFileMisMines)
	{
		errorWriteFile();
		return;
	}

	std::vector<uint8_t> mapMines(LOCATIONSSIZE, 0);

	const size_t maxTries = map_mines.size() / mapSizeU;
	for (size_t i = 0; i < maxTries; ++i)
		std::copy(map_mines.cbegin() + i * mapSizeU, map_mines.cbegin() + i * mapSizeU + mapSizeU, mapMines.begin() + 512 * i);

	outputFileMisMines.write((char*)mapMines.data(), mapMines.size());
	outputFileMisMines.close();
}