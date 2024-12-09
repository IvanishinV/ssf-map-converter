
#include "stdafx.h"

#include "Map_cflags.h"
#include "General.h"
#include "Displayinfo.h"

bool covertMapCflags(const std::vector<uint8_t>& map_flags)
{
	std::ofstream outputMapCflags("map.000/cflags", std::ios::binary);
	if (!outputMapCflags)
	{
		errorWriteFile();
		return false;
	}

	const size_t length = map_flags.size();
	size_t offset{ 0 };
	size_t vOffset{ 0 };

	std::vector<uint8_t> mapFlags(length / 2, 0);

	while (offset < length)
	{
		const uint8_t num1 = map_flags[offset];
		const uint8_t num2 = map_flags[offset + 1];
		const uint8_t num3 = map_flags[offset + 2];
		const uint8_t num4 = map_flags[offset + 3];
		offset += 4;

		//Тернарные операторы
		const uint8_t zoneBlocker = ((num1 & 0xF0) > 0 ? GLOBALNULL : (num1 & 0x0F));
		const uint8_t waves = ~num2 >> 1;
		const uint8_t noPantones = ((num2 & 0x0F) == 6 ? ((num3 & 0x0F) > 0 ? 0 : 1) : 0);

		//Работа с битами
		const uint8_t cflags = noPantones << 7 | (waves & 0x40) | (num2 & 0x30) | zoneBlocker;

		mapFlags[vOffset] = cflags;
		//mapFlags[vOffset + 1] = 0;	// not needed
	}
	outputMapCflags.write((char*)mapFlags.data(), mapFlags.size());// << mapFlags.str();
	outputMapCflags.close();

	return true;
}
