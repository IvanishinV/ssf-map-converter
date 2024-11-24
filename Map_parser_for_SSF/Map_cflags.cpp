#include <iostream> 
#include <sstream>
#include <fstream> 

#include "Map_cflags.h"
#include "General.h"
#include "Displayinfo.h"

void covertMapCflags(std::stringstream& map_flags)
{
	std::ofstream outputMapCflags("map.000/cflags", std::ios::binary);
	if (!outputMapCflags)
	{
		erorbuildfile();
		return;
	}
	std::stringstream mapFlags;

	uint8_t buffer[4];
	while (map_flags.read((char*)buffer, sizeof(buffer)))
	{
		uint8_t num1 = *(uint8_t*)(buffer + 0);
		uint8_t num2 = *(uint8_t*)(buffer + 1);
		uint8_t num3 = *(uint8_t*)(buffer + 2);
		uint8_t num4 = *(uint8_t*)(buffer + 3);

		//Тернарные операторы
		uint8_t zoneBlocker = ((num1 & 0xF0) > 0 ? GLOBALNULL : (num1 & 0x0F));
		uint8_t waves = ~num2 >> 1;
		uint8_t noPantones = ((num2 & 0x0F) == 6 ? ((num3 & 0x0F) > 0 ? 0 : 1) : 0);

		//Работа с битами
		uint8_t cflags = noPantones << 7 | (waves & 0x40) | (num2 & 0x30) | zoneBlocker;
		mapFlags << cflags << (uint8_t)GLOBALNULL;
	}
	outputMapCflags << mapFlags.str();
	mapFlags.str("");
	outputMapCflags.close();
}