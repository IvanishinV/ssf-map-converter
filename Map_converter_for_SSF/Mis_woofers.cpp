
#include "stdafx.h"

#include "Helper.h"
#include "Mis_woofers.h"
#include "Mis_scripts.h"
#include "Displayinfo.h"

void covertMisWoofers(const std::vector<uint8_t>& mis_woofers)
{
	std::ofstream outputFileMisWoofers("map.000/mis.000/sounds", std::ios::binary);
	if (!outputFileMisWoofers)
	{
		errorWriteFile();
		return;
	}
	//------------------------------------------------------------------------------
	const uint32_t numOfSounds = readFileUint32(mis_woofers, 0);
	for (uint32_t i = 0, curOffset = sizeof(numOfSounds); i < numOfSounds; ++i)
	{
		const uint8_t* buffername = mis_woofers.data() + curOffset;
		curOffset += 64;
		const uint8_t* buffer = mis_woofers.data() + curOffset;
		curOffset += 14;
		//------------------------------------------------------------------------------
		uint16_t U = *(uint16_t*)(buffer + 0);
		uint16_t V = *(uint16_t*)(buffer + 2);
		uint16_t Radius = *(uint16_t*)(buffer + 4);
		float Worse = *(float*)(buffer + 6);
		uint16_t MinWait = *(uint16_t*)(buffer + 10);
		uint16_t MaxWait = *(uint16_t*)(buffer + 12);
		//------------------------------------------------------------------------------
		outputFileMisWoofers << "Name=\"" << (const char*)buffername;
		//outputFileMisWoofers.write((const char*)buffername, 64);
		outputFileMisWoofers << "\"\nU=" << U << "\nV=" << V
			<< "\nRadius=" << Radius << "\nWorse=" << Worse << "\nMinWait=" << MinWait << "\nMaxWait=" << MaxWait << "\n\n";
	}
	outputFileMisWoofers.close();
}