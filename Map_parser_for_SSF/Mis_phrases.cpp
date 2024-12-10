
#include "stdafx.h"

//Project header
#include "Helper.h"
#include "Mis_phrases.h"
#include "util.h"
#include "Displayinfo.h"

//ДОПИЛИТЬ убрать лишние функции
//___________________________________________________________________________________________________
void covertMisPhrases(const std::vector<uint8_t>& mis_phrases, const uint32_t sizeMisPhrases)
{
	std::ofstream outputFileMisPharses("map.000/mis.000/phrases", std::ios::binary);
	if (!outputFileMisPharses)
	{
		errorWriteFile();
		return;
	}
	//------------------------------------------------------------------------------
	uint32_t accumulator = 0;
	//------------------------------------------------------------------------------
	while (sizeMisPhrases > accumulator)
	{
		uint32_t numtext = readFileInt8(mis_phrases, 0 + accumulator);

		uint32_t sizeline1 = readFileInt8(mis_phrases, 1 + accumulator);
		if (sizeline1 > 0)
		{
			position(mis_phrases, outputFileMisPharses, 2 + accumulator, sizeline1);
			accumulator += sizeline1;
			for (uint32_t i = 0; i < 64 - sizeline1; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		uint32_t sizeline2 = readFileInt8(mis_phrases, 2 + accumulator);
		if (sizeline2 > 0)
		{
			position(mis_phrases, outputFileMisPharses, 3 + accumulator, sizeline2);
			accumulator += sizeline2;
			for (uint32_t i = 0; i < 64 - sizeline2; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		uint32_t sizeline3 = readFileInt8(mis_phrases, 3 + accumulator);
		if (sizeline3 > 0)
		{
			position(mis_phrases, outputFileMisPharses, 4 + accumulator, sizeline3);
			accumulator += sizeline3;
			for (uint32_t i = 0; i < 64 - sizeline3; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		uint32_t sizeline4 = readFileInt8(mis_phrases, 4 + accumulator);
		if (sizeline4 > 0)
		{
			position(mis_phrases, outputFileMisPharses, 5 + accumulator, sizeline4);
			accumulator += sizeline4;
			for (uint32_t i = 0; i < 64 - sizeline4; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		uint32_t sizeline5 = readFileInt8(mis_phrases, 5 + accumulator);
		if (sizeline5 > 0)
		{
			position(mis_phrases, outputFileMisPharses, 6 + accumulator, sizeline5);
			accumulator += sizeline5;
			for (uint32_t i = 0; i < 64 - sizeline5; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		else
		{
			for (int i = 0; i < 64; i++)
			{
				outputFileMisPharses << GLOBALNULL;
			}
		}
		accumulator += 6;
	}

	outputFileMisPharses.close();
}


//struct MisPhrases
//{
//	uint8_t numberText;
//	uint8_t sizeLineText1;
//	uint8_t sizeLineText2;
//	uint8_t sizeLineText3;
//	uint8_t sizeLineText4;
//	uint8_t sizeLineText5;
//};