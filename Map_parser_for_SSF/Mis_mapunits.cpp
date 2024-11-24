#include <iostream>
#include <sstream>
#include <fstream> 
#include <map>

#include <windows.h>
#include "Mis_mapunits.h"
#include "Displayinfo.h"

void covertMisUnits(std::stringstream& mis_unitnames, std::stringstream& mis_mapunits, std::stringstream& mis_support)
{
	//------------------------------------------------------------------------------
	std::ofstream outputFileMapUnits("map.000/mis.000/mapunits", std::ios::binary);
	std::ofstream outputFileSupport("map.000/mis.000/support", std::ios::binary);
	if (!outputFileMapUnits || !outputFileSupport)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	std::map <int, std::string> nameunit;
	int i = 0;
	char buffername[16];
	while (mis_unitnames.read(buffername, sizeof(buffername)))
	{
		nameunit[i] = std::string(buffername);
		i++;
	}
	//mis_unitnames.close();
	for (int y = 0; y < i; y++)
	{
		if (nameunit.find(y) == nameunit.end())
		{
			std::cout << "not found" << std::endl;
		}
		//else
		//{
		//	std::cout << "found" << endl;
		//}
		/*cout << nameunit[y] << endl;*/
	}
	//------------------------------------------------------------------------------
	uint32_t numberofunit;
	mis_mapunits.read(reinterpret_cast<char*>(&numberofunit), sizeof(numberofunit));
	//------------------------------------------------------------------------------
	char buffer[13];
	while (mis_mapunits.read(buffer, sizeof(buffer)))
	{
		uint8_t GRP = *(uint8_t*)(buffer + 0);
		uint8_t HP = *(uint8_t*)(buffer + 1);
		uint8_t Ammo = *(uint8_t*)(buffer + 2);
		uint8_t Expa = *(uint8_t*)(buffer + 3);
		uint8_t Lives = *(uint8_t*)(buffer + 4);
		uint8_t ID = *(uint8_t*)(buffer + 5);
		uint8_t In = *(uint8_t*)(buffer + 6);
		uint16_t U = *(uint16_t*)(buffer + 7);
		uint16_t V = *(uint16_t*)(buffer + 9);
		uint8_t Dir = *(uint8_t*)(buffer + 11);
		uint8_t Owner = *(uint8_t*)(buffer + 12);
		{
			outputFileMapUnits << "ID=" << nameunit[(uint16_t)ID] << " GRP=" << (uint16_t)GRP << " HP=" << (uint16_t)HP << " Ammo=" << (uint16_t)Ammo 
				<< " Expa=" << (uint16_t)Expa << " Lives=" << (uint16_t)Lives << " U=" << U << " V=" << V << " Dir=" << (uint16_t)Dir 
				<< " In=" << (uint16_t)In << " Owner=" << (uint16_t)Owner << '\n';
		}
		if (In > 0)
		{
			while (In--)
			{
				char buffer[6];
				mis_mapunits.read(buffer, sizeof(buffer));
				uint8_t GRPpassenger = *(uint8_t*)(buffer + 0);
				uint8_t HPpassenger = *(uint8_t*)(buffer + 1);
				uint8_t Ammopassenger = *(uint8_t*)(buffer + 2);
				uint8_t Expapassenger = *(uint8_t*)(buffer + 3);
				uint8_t Livespassenger = *(uint8_t*)(buffer + 4);
				uint8_t IDpassenger = *(uint8_t*)(buffer + 5);
				{
					outputFileMapUnits << " ID=" << nameunit[(uint16_t)IDpassenger] << " GRP=" << (uint16_t)GRPpassenger 
						<< " HP=" << (uint16_t)HPpassenger << " Ammo=" << (uint16_t)Ammopassenger 
						<< " Expa=" << (uint16_t)Expapassenger << " Lives=" << (uint16_t)Livespassenger << '\n';
				}
			}
		}
	}
	//------------------------------------------------------------------------------
	uint32_t SupportSize;
	mis_support.seekg(0, std::ios::beg);
	mis_support.read(reinterpret_cast<char*>(&SupportSize), sizeof(SupportSize));
	uint32_t flag_num = 16;
	uint32_t script_num = 64;
	while (flag_num--)
	{
		char buffer[20];
		mis_support.read(buffer, sizeof(buffer));
		INT num1 = *(uint32_t*)(buffer + 0);
		INT num2 = *(uint32_t*)(buffer + 4);
		INT num3 = *(uint32_t*)(buffer + 8);
		INT num4 = *(uint32_t*)(buffer + 12);
		INT num5 = *(uint32_t*)(buffer + 16);
		//cout << "flag " << num1 << " " << num2 << " " << num5 << " " << num3 << " " << num4 << '\n';
		outputFileSupport << "flag " << num1 << " " << num2 << " " << num5 << " " << num3 << " " << num4 << '\n';
	}
	//cout << '\n';
	outputFileSupport << '\n';
	//------------------------------------------------------------------------------
	uint32_t accumulator = 0;
	while (script_num--)
	{
		uint8_t NameSize;
		mis_support.seekg(324 + accumulator, std::ios::beg);
		mis_support.read(reinterpret_cast<char*>(&NameSize), sizeof(NameSize));
		if (NameSize > 0)
		{
			mis_support.seekg(324 + 1 + accumulator);
			char* buffername = new char[NameSize + 1];
			buffername[NameSize] = '\0';
			mis_support.read(buffername, NameSize);
			//cout << "support " << "\"" << buffername << "\"" << '\n';
			outputFileSupport << "support " << "\"" << buffername << "\"" << '\n';
			delete[] buffername;
		}
		else
		{
			//cout << "support " << "\"" << "\"" << '\n';
			outputFileSupport << "support " << "\"" << "\"" << '\n';
		}
		//------------------------------------------------------------------------------
		uint32_t UnitScriptSize;
		mis_support.seekg(324 + 1 + accumulator + NameSize, std::ios::beg);
		mis_support.read(reinterpret_cast<char*>(&UnitScriptSize), sizeof(UnitScriptSize));
		accumulator += 5 + NameSize;
		if (UnitScriptSize > 0)
		{
			while (UnitScriptSize--)
			{
				char bufferunit[7];
				mis_support.read(bufferunit, sizeof(bufferunit));
				uint8_t GRP = *(uint8_t*)(bufferunit + 0);
				uint8_t HP = *(uint8_t*)(bufferunit + 1);
				uint8_t Ammo = *(uint8_t*)(bufferunit + 2);
				uint8_t Expa = *(uint8_t*)(bufferunit + 3);
				uint8_t Lives = *(uint8_t*)(bufferunit + 4);
				uint8_t Id = *(uint8_t*)(bufferunit + 5);
				uint8_t In = *(uint8_t*)(bufferunit + 6);
				accumulator += 7;
				{
					//cout << "ID=" << Id << " GRP=" << GRP << " HP=" << HP << " Ammo=" << Ammo << " Expa=" << Expa << " Lives=" << Lives << " In=" << In << '\n';
					outputFileSupport << "ID=" << nameunit[(uint16_t)Id] << " GRP=" << (uint16_t)GRP << " HP=" << (uint16_t)HP 
						<< " Ammo=" << (uint16_t)Ammo << " Expa=" << (uint16_t)Expa 
						<< " Lives=" << (uint16_t)Lives << " In=" << (uint16_t)In << '\n';
				}
				if (In > 0)
				{
					while (In--)
					{
						char bufferunit[6];
						mis_support.read(bufferunit, sizeof(bufferunit));
						uint8_t GRPpasseenger = *(uint8_t*)(bufferunit + 0);
						uint8_t HPpasseenger = *(uint8_t*)(bufferunit + 1);
						uint8_t Ammopasseenger = *(uint8_t*)(bufferunit + 2);
						uint8_t Expapasseenger = *(uint8_t*)(bufferunit + 3);
						uint8_t Livespasseenger = *(uint8_t*)(bufferunit + 4);
						uint8_t Idpasseenger = *(uint8_t*)(bufferunit + 5);
						accumulator += 6;
						{
							//cout << "ID=" << Idpasseenger << " GRP=" << GRPpasseenger << " HP=" << HPpasseenger << " Ammo=" << Ammopasseenger << " Expa=" << Expapasseenger << " Lives=" << Livespasseenger << '\n';
							outputFileSupport << "ID=" << nameunit[(uint16_t)Idpasseenger] << " GRP=" << (uint16_t)GRPpasseenger
								<< " HP=" << (uint16_t)HPpasseenger << " Ammo=" << (uint16_t)Ammopasseenger
								<< " Expa=" << (uint16_t)Expapasseenger << " Lives=" << (uint16_t)Livespasseenger << '\n';
						}
					}
				}
			}
		}
		//cout << "end" << '\n';
		outputFileSupport << "end" << '\n';
	}
	//------------------------------------------------------------------------------
	outputFileMapUnits.close();
	outputFileSupport.close();
	//------------------------------------------------------------------------------
	return;
}
