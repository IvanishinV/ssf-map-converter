#include <iostream>
#include <sstream>
#include <fstream> 

#include "Mis_woofers.h"
#include "Mis_scripts.h"
#include "Displayinfo.h"

void covertMisWoofers(std::stringstream& mis_woofers)
{
	std::ofstream outputFileMisWoofers("map.000/mis.000/sounds", std::ios::binary);
	if (!outputFileMisWoofers)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	uint32_t startposition = readFileUint32(mis_woofers, 0);
	while (startposition--)
	{
		char buffername[64];
		mis_woofers.read(reinterpret_cast<char*>(&buffername), sizeof(buffername));
		buffername[63] = '\0';
		//------------------------------------------------------------------------------
		char buffer[14];
		(mis_woofers.read(buffer, sizeof(buffer)));
		//------------------------------------------------------------------------------
		uint16_t U = *(uint16_t*)(buffer + 0);
		uint16_t V = *(uint16_t*)(buffer + 2);
		uint16_t Radius = *(uint16_t*)(buffer + 4);
		float Worse = *(float*)(buffer + 6);
		uint16_t MinWait = *(uint16_t*)(buffer + 10);
		uint16_t MaxWait = *(uint16_t*)(buffer + 12);
		//------------------------------------------------------------------------------
		outputFileMisWoofers << "Name=" << "\"" << buffername << "\"" << "\n" << "U=" << U << "\n" << "V=" << V
			<< "\n" << "Radius=" << Radius << "\n" << "Worse=" << Worse << "\n" << "MinWait=" << MinWait << "\n" << "MaxWait=" << MaxWait << "\n\n";
	}
	outputFileMisWoofers.close();
}