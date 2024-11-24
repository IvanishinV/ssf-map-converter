//VS header
#include <iostream>
#include <sstream>
#include <fstream> 

//Project header
#include "Mis_phrases.h"
#include "General.h"
#include "Displayinfo.h"

//ДОПИЛИТЬ убрать лишние функции
//___________________________________________________________________________________________________
uint32_t positionPhrases(std::stringstream& mis_phrases, std::ofstream& outputFile, const uint32_t startPosition, const uint32_t size)
{
	mis_phrases.seekg(startPosition);
	char* buffer = new char[size];
	mis_phrases.read(buffer, size);
	outputFile.write(buffer, size);
	delete[] buffer;
	uint32_t position = startPosition + size;
	return position;
}
//___________________________________________________________________________________________________
char readFileUint8(std::stringstream& mis_phrases, const uint32_t fileOffset)
{
	char result;
	mis_phrases.seekg(fileOffset, std::ios::beg);
	mis_phrases.read(reinterpret_cast<char*>(&result), sizeof(result));
	return result;
}
//___________________________________________________________________________________________________
void covertMisPhrases(std::stringstream& mis_phrases, uint32_t sizeMisPhrases)
{
	std::ofstream outputFileMisPharses("map.000/mis.000/phrases", std::ios::binary);
	if (!outputFileMisPharses)
	{
		erorbuildfile();
		return;
	}
	//------------------------------------------------------------------------------
	uint32_t accumulator = 0;
	//------------------------------------------------------------------------------
	while (sizeMisPhrases > accumulator)
	{
		uint32_t numtext = readFileUint8(mis_phrases, 0 + accumulator);

		uint32_t sizeline1 = readFileUint8(mis_phrases, 1 + accumulator);
		if (sizeline1 > 0)
		{
			positionPhrases(mis_phrases, outputFileMisPharses, 2 + accumulator, sizeline1);
			accumulator += sizeline1;
			for (int i = 0; i < 64 - sizeline1; i++)
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
		uint32_t sizeline2 = readFileUint8(mis_phrases, 2 + accumulator);
		if (sizeline2 > 0)
		{
			positionPhrases(mis_phrases, outputFileMisPharses, 3 + accumulator, sizeline2);
			accumulator += sizeline2;
			for (int i = 0; i < 64 - sizeline2; i++)
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
		uint32_t sizeline3 = readFileUint8(mis_phrases, 3 + accumulator);
		if (sizeline3 > 0)
		{
			positionPhrases(mis_phrases, outputFileMisPharses, 4 + accumulator, sizeline3);
			accumulator += sizeline3;
			for (int i = 0; i < 64 - sizeline3; i++)
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
		uint32_t sizeline4 = readFileUint8(mis_phrases, 4 + accumulator);
		if (sizeline4 > 0)
		{
			positionPhrases(mis_phrases, outputFileMisPharses, 5 + accumulator, sizeline4);
			accumulator += sizeline4;
			for (int i = 0; i < 64 - sizeline4; i++)
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
		uint32_t sizeline5 = readFileUint8(mis_phrases, 5 + accumulator);
		if (sizeline5 > 0)
		{
			positionPhrases(mis_phrases, outputFileMisPharses, 6 + accumulator, sizeline5);
			accumulator += sizeline5;
			for (int i = 0; i < 64 - sizeline5; i++)
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