
#include "stdafx.h"

#include "Mis_players.h"
#include "Displayinfo.h"

void convertMisPlayers(const std::vector<uint8_t>& mis_players)
{
	std::ofstream outputMisPlayers("map.000/mis.000/players", std::ios::binary);
	if (!outputMisPlayers)
	{
		errorWriteFile();
		return;
	}
	//------------------------------------------------------------------------------
	const size_t maxTries = mis_players.size() / 353;
	for (size_t curTry = 0; curTry < maxTries; ++curTry)
	{
		const uint8_t* buffer = mis_players.data() + curTry * 353;
		outputMisPlayers << "Player " << curTry << '\n';
		std::string_view name{ (const char*)buffer };
		outputMisPlayers << " name=" << '\"' << name << '\"' << '\n';
		uint8_t team = *(uint8_t*)(buffer + 32);
		outputMisPlayers << " team=" << (uint16_t)team << '\n';
		uint16_t RGB_Color = *(uint16_t*)(buffer + 33);
		uint8_t nation = *(uint8_t*)(buffer + 35);
		uint8_t planesdir = *(uint8_t*)(buffer + 36);
		outputMisPlayers << " nation=" << (uint16_t)nation << '\n';
		{
			unsigned short red = (RGB_Color >> 11) & 0x1F;
			unsigned short green = (RGB_Color >> 5) & 0x3F;
			unsigned char blue = RGB_Color & 0x1F;
			outputMisPlayers << " color=" << (uint16_t)red * 8 << " " << (uint16_t)green * 4 << " " << (uint16_t)blue * 8 << '\n';
		}
		//------------------------------------------------------------------------------
		outputMisPlayers << " bomb" << '\n';
		std::string_view namebomb{ (const char*)buffer + 37 };
		outputMisPlayers << "  ID=" << namebomb << '\n';
		uint32_t Numberbomb = *(uint32_t*)(buffer + 69);
		uint32_t Bombsbomb = *(uint32_t*)(buffer + 73);
		uint32_t Reloadbomb = *(uint32_t*)(buffer + 77);
		outputMisPlayers << "  Number=" << Numberbomb << '\n' << "  Bombs=" << Bombsbomb << '\n' << "  Reload=" << Numberbomb << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " spy" << '\n';
		std::string_view namespy{ (const char*)buffer + 81 };
		outputMisPlayers << "  ID=" << namespy << '\n';
		uint32_t Numberspy = *(uint32_t*)(buffer + 113);
		uint32_t Bombsspy = *(uint32_t*)(buffer + 117);
		uint32_t Reloadspy = *(uint32_t*)(buffer + 121);
		outputMisPlayers << "  Number=" << Numberspy << '\n' << "  Bombs=" << Bombsspy << '\n' << "  Reload=" << Reloadspy << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " transport" << '\n';
		std::string_view nametransport{ (const char*)buffer + 125 };
		outputMisPlayers << "  ID=" << nametransport << '\n';
		uint32_t Numbertransport = *(uint32_t*)(buffer + 157);
		uint32_t Bombstransport = *(uint32_t*)(buffer + 161);
		uint32_t Reloadtransport = *(uint32_t*)(buffer + 165);
		outputMisPlayers << "  Number=" << Numbertransport << '\n' << "  Bombs=" << Bombstransport << '\n' << "  Reload=" << Reloadtransport << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " boxer" << '\n';
		std::string_view nameboxer{ (const char*)buffer + 169 };
		outputMisPlayers << "  ID=" << nameboxer << '\n';
		uint32_t Numberboxer = *(uint32_t*)(buffer + 201);
		uint32_t Bombsboxer = *(uint32_t*)(buffer + 205);
		uint32_t Reloadboxer = *(uint32_t*)(buffer + 209);
		outputMisPlayers << "  Number=" << Numberboxer << '\n' << "  Bombs=" << Bombsboxer << '\n' << "  Reload=" << Reloadboxer << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " descent 0" << '\n';
		uint8_t group1 = *(uint8_t*)(buffer + 213);
		uint8_t expa1 = *(uint8_t*)(buffer + 214);
		outputMisPlayers << "  group=" << (uint16_t)group1 << '\n' << "  expa=" << (uint16_t)expa1 << '\n';
		std::string_view ID11{ (const char*)buffer + 215 };
		std::string_view ID12{ (const char*)buffer + 231 };
		std::string_view ID13{ (const char*)buffer + 247 };
		std::string_view ID14{ (const char*)buffer + 263 };
		uint8_t number11 = *(uint8_t*)(buffer + 279);
		uint8_t number12 = *(uint8_t*)(buffer + 280);
		uint8_t number13 = *(uint8_t*)(buffer + 281);
		uint8_t number14 = *(uint8_t*)(buffer + 282);
		outputMisPlayers << "  ID 0=" << ID11 << '\n' << "  number 0=" << (uint16_t)number11 << '\n'
			<< "  ID 1=" << ID12 << '\n' << "  number 1=" << (uint16_t)number12 << '\n'
			<< "  ID 2=" << ID13 << '\n' << "  number 2=" << (uint16_t)number13 << '\n'
			<< "  ID 3=" << ID14 << '\n' << "  number 3=" << (uint16_t)number14 << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " descent 1" << '\n';
		uint8_t group2 = *(uint8_t*)(buffer + 283);
		uint8_t expa2 = *(uint8_t*)(buffer + 284);
		outputMisPlayers << "  group=" << (uint16_t)group2 << '\n' << "  expa=" << (uint16_t)expa2 << '\n';
		std::string_view ID21{ (const char*)buffer + 285 };
		std::string_view ID22{ (const char*)buffer + 301 };
		std::string_view ID23{ (const char*)buffer + 317 };
		std::string_view ID24{ (const char*)buffer + 333 };
		uint8_t number21 = *(uint8_t*)(buffer + 349);	 
		uint8_t number22 = *(uint8_t*)(buffer + 350);
		uint8_t number23 = *(uint8_t*)(buffer + 351);
		uint8_t number24 = *(uint8_t*)(buffer + 352);
		outputMisPlayers << "  ID 0=" << ID21 << '\n' << "  number 0=" << (uint16_t)number21 << '\n'
			<< "  ID 1=" << ID22 << '\n' << "  number 1=" << (uint16_t)number22 << '\n'
			<< "  ID 2=" << ID23 << '\n' << "  number 2=" << (uint16_t)number23 << '\n'
			<< "  ID 3=" << ID24 << '\n' << "  number 3=" << (uint16_t)number24 << '\n';
		//------------------------------------------------------------------------------
		outputMisPlayers << " planesdir=" << (uint16_t)planesdir << '\n';
	}
	outputMisPlayers.close();
}