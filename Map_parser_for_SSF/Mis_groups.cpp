#include <iostream>
#include <sstream>
#include <fstream> 
#include <array>

#include "Mis_groups.h"
#include "General.h"
#include "Displayinfo.h"

void aiFlagsConvert(std::ofstream& outputFileMisGroups, uint8_t aitype1, uint8_t aitype2, uint8_t aitype3, uint8_t aitype4)
{
	uint32_t AIflags = ((aitype4 & 0xFF) << 24) | ((aitype3 & 0xFF) << 16) | ((aitype2 & 0xFF) << 8) | (aitype1 & 0xFF);
	uint8_t AItype1;
	uint8_t AItype2;
	uint8_t AItype3;
	uint8_t AItype4;
	AItype1 = (uint8_t)(AIflags & 0xF0) >> 4;
	if (AItype1 >= 8)
	{
		outputFileMisGroups << " aif_useobjects";
		AItype1 -= 8;
	}
	if (AItype1 >= 4)
	{
		outputFileMisGroups << " aif_zonenoemeny";
		AItype1 -= 4;
	}
	if (AItype1 >= 2)
	{
		outputFileMisGroups << " aif_zonehaveown";
		AItype1 -= 2;
	}
	if (AItype1 == 1)
	{
		outputFileMisGroups << " aif_rndmove";
	}
	AItype2 = (uint8_t)(AIflags >> 8) & 0xFF;
	if (AItype2 >= 128)
	{
		outputFileMisGroups << " aif_atgnotow";
		AItype2 += 128;
	}
	if (AItype2 >= 64)
	{
		outputFileMisGroups << " aif_atgnomove";
		AItype2 -= 64;
	}
	if (AItype2 >= 32)
	{
		outputFileMisGroups << " aif_atgzonenoenemy";
		AItype2 -= 32;
	}
	if (AItype2 >= 16)
	{
		outputFileMisGroups << " aif_atgzonehaveown";
		AItype2 -= 16;
	}
	if (AItype2 >= 8)
	{
		outputFileMisGroups << " aif_atgnouse";
		AItype2 -= 8;
	}
	if (AItype2 >= 4)
	{
		outputFileMisGroups << " aif_zrestrict";
		AItype2 -= 4;
	}
	if (AItype2 >= 2)
	{
		outputFileMisGroups << " aif_grestrict";
		AItype2 -= 2;
	}
	if (AItype2)
	{
		outputFileMisGroups << " aif_rndtarget";
	}
	AItype3 = (uint8_t)(AIflags >> 16) & 0xFF;
	if (AItype3 >= 128)
	{
		outputFileMisGroups << " aif_domnouse";
		AItype3 += 128;
	}
	if (AItype3 >= 64)
	{
		outputFileMisGroups << " aif_gaubnodrop";
		AItype3 -= 64;
	}
	if (AItype3 >= 32)
	{
		outputFileMisGroups << " aif_gaubnotow";
		AItype3 -= 32;
	}
	if (AItype3 >= 16)
	{
		outputFileMisGroups << " aif_gaubnomove";
		AItype3 -= 16;
	}
	if (AItype3 >= 8)
	{
		outputFileMisGroups << " aif_gaubzonenoenemy";
		AItype3 -= 8;
	}
	if (AItype3 >= 4)
	{
		outputFileMisGroups << " aif_gaubzonehaveown";
		AItype3 -= 4;
	}
	if (AItype3 >= 2)
	{
		outputFileMisGroups << " aif_gaubnouse";
		AItype3 -= 2;
	}
	if (AItype3 == 1)
	{
		outputFileMisGroups << " aif_atgnodrop";
	}
	AItype4 = (uint8_t)(AIflags >> 24) & 0x0F;
	if (AItype4 >= 4)
	{
		outputFileMisGroups << " aif_holdfire";
		AItype4 -= 4;
	}
	if (AItype4 >= 2)
	{
		outputFileMisGroups << " aif_domhide";
		AItype4 -= 2;
	}
	if (AItype4 == 1)
	{
		outputFileMisGroups << " aif_domambush";
	}
	return;
}

void convertMisGroups(std::stringstream& mis_groups)
{
	std::ofstream outputFileMisGroups("map.000/mis.000/groups", std::ios::binary);
	if (!outputFileMisGroups)
	{
		erorbuildfile();
		return;
	}
	std::array <std::string, 11> AI_type_array =
	{
		"\"ai_none\"",
		"\"ai_recon\"",
		"\"ai_sold_guard\"",
		"\"ai_sold_follow\"",
		"\"ai_sold_art\"",
		"\"ai_tank_guard\"",
		"\"ai_tank_follow\"",
		"\"ai_furg_help\"",
		"\"ai_furg_move\"",
		"\"ai_gruz_reload\"",
		"\"ai_katya_move\""
	};
	//------------------------------------------------------------------------------
	uint32_t accumulator = 0;
	//------------------------------------------------------------------------------
	char buffer[27];
	while (mis_groups.read(buffer, sizeof(buffer)))
	{
		uint8_t aitype1 = *(uint8_t*)(buffer + 0);
		uint8_t aitype2 = *(uint8_t*)(buffer + 1);
		uint8_t aitype3 = *(uint8_t*)(buffer + 2);
		uint8_t aitype4 = *(uint8_t*)(buffer + 3);

		uint8_t zone1 = *(uint8_t*)(buffer + 4);
		uint8_t zone2 = *(uint8_t*)(buffer + 5);
		uint8_t group1 = *(uint8_t*)(buffer + 6);
		uint8_t group2 = *(uint8_t*)(buffer + 7);
		uint8_t flag = *(uint8_t*)(buffer + 8);
		uint8_t zone = *(uint8_t*)(buffer + 9);

		uint8_t atime1 = *(uint8_t*)(buffer + 10);
		uint8_t atime2 = *(uint8_t*)(buffer + 11);
		uint8_t atime3 = *(uint8_t*)(buffer + 12);
		uint8_t atime4 = *(uint8_t*)(buffer + 13);

		uint8_t delay1 = *(uint8_t*)(buffer + 14);
		uint8_t delay2 = *(uint8_t*)(buffer + 15);
		uint8_t delay3 = *(uint8_t*)(buffer + 16);
		uint8_t delay4 = *(uint8_t*)(buffer + 17);

		uint16_t min = *(uint16_t*)(buffer + 18);

		uint8_t force1 = *(uint8_t*)(buffer + 20);
		uint8_t force2 = *(uint8_t*)(buffer + 21);
		uint8_t force3 = *(uint8_t*)(buffer + 22);
		uint8_t force4 = *(uint8_t*)(buffer + 23);

		uint8_t hp = *(uint8_t*)(buffer + 24);
		uint8_t ammo = *(uint8_t*)(buffer + 25);
		uint8_t expa = *(uint8_t*)(buffer + 26);
		//------------------------------------------------------------------------------
		{
			outputFileMisGroups << "Group " << accumulator << '\n';
			uint8_t AI_type = (uint8_t)(aitype1 & 0x0F);
			outputFileMisGroups << " ai type=" << AI_type_array[AI_type];

			outputFileMisGroups << " group1=" << (uint16_t)group1 << " group2=" << (uint16_t)group2 << " zone1=" << (uint16_t)zone1 << " zone2=" << (uint16_t)zone2 << '\n';

			if (AI_type == 0 || AI_type == 1)
			{
				outputFileMisGroups << "  aiflags=" << '\n';
			}
			else
			{
				outputFileMisGroups << "  aiflags=";
				aiFlagsConvert(outputFileMisGroups, aitype1, aitype2, aitype3, aitype4);
				outputFileMisGroups << '\n';
			}

		}

		//------------------------------------------------------------------------------
		if (atime1 == 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
		{
			outputFileMisGroups << " reserv auto=" << 0 << " delay=" << 0 << " min=" << 0 << " force=" << 0 << " atime=" << 0;
		}
		else
		if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 1 << " delay=" << 0 << " min=" << min << " force=" << 0 << " atime=" << convertatime;
		}
		else
		if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && min == 0 && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertdelay = delay1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 3 << " delay=" << convertdelay << " min=" << min << " force=" << 0 << " atime=" << convertatime;
		}
		else
		if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && (min >= 1 && min <= 98) && force1 == 255 && force2 == 255 && force3 == 255 && force4 == 127)
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertdelay = delay1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 7 << " delay=" << convertdelay << " min=" << min << " force=" << 0 << " atime=" << convertatime;
		}
		else
		if (atime1 > 0 && (delay3 >= 1 && delay3 <= 13) && min == 0 && (force3 >= 1 && force3 <= 13))
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
			uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertdelay = delay1234 / timeconvertnum;
			uint32_t convertforce = force1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 11 << " delay=" << convertdelay << " min=" << min << " force=" << convertforce << " atime=" << convertatime;
		}
		else
		if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && min == 0 && (force3 >= 1 && force3 <= 13))
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertforce = force1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 9 << " delay=" << 0 << " min=" << 0 << " force=" << convertforce << " atime=" << convertatime;
		}
		else
		if (atime1 > 0 && delay1 == 255 && delay2 == 255 && delay3 == 255 && delay4 == 127 && (min >= 1 && min <= 98) && (force3 >= 1 && force3 <= 13))
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertforce = force1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 13 << " delay=" << 0 << " min=" << min << " force=" << convertforce << " atime=" << convertatime;
		}
		else
		{
			uint32_t atime1234 = ((atime4 & 0xFF) << 24) | ((atime3 & 0xFF) << 16) | ((atime2 & 0xFF) << 8) | (atime1 & 0xFF);
			uint32_t delay1234 = ((delay4 & 0xFF) << 24) | ((delay3 & 0xFF) << 16) | ((delay2 & 0xFF) << 8) | (delay1 & 0xFF);
			uint32_t force1234 = ((force4 & 0xFF) << 24) | ((force3 & 0xFF) << 16) | ((force2 & 0xFF) << 8) | (force1 & 0xFF);
			uint32_t convertatime = atime1234 / timeconvertnum;
			uint32_t convertdelay = delay1234 / timeconvertnum;
			uint32_t convertforce = force1234 / timeconvertnum;
			outputFileMisGroups << " reserv auto=" << 15 << " delay=" << convertdelay << " min=" << min << " force=" << convertforce << " atime=" << convertatime;
		}
		//------------------------------------------------------------------------------
		{
			outputFileMisGroups << " flag=" << (uint16_t)flag << " zone=" << (uint16_t)zone << " hp=" << (uint16_t)hp << " ammo=" << (uint16_t)ammo << " expa=" << (uint16_t)expa << '\n';
		}
		//------------------------------------------------------------------------------
		accumulator++;
	}
	outputFileMisGroups.close();
	return;
}




//int main()
//{
//	int mask;
//	std::cin >> mask;
//
//	std::string aif_array[] = { "aif_rndmove", "aif_zonehaveown", "aif_zonenoenemy", };
//	for (int i = 0; i < 3; ++i)
//	{
//		if (mask & (i << i))
//
//		{
//			std::cout << aif_array[i] << " ";
//		}
//
//
//	}
//}
