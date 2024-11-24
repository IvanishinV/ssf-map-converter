#include <iostream>
#include <sstream>
#include <math.h>
#include <fstream> 
#include <string>

#include "Map_mini.h"
#include "General.h"
#include "Displayinfo.h"

#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};
#pragma pack (pop)
struct BITMAPINFOHEADER
{
	uint32_t biSize;
	uint32_t biWidth;
	uint32_t biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	long	 biXPelsPerMeter;
	long	 biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};
struct RGBQUAD
{
	uint32_t rgbBlue;
	uint32_t rgbGreen;
	uint32_t rgbRed;
	//uint32_t rgbReserved;
};

void covertMapMini(std::stringstream& map_mini)
{
	std::ofstream outputFileMapMiniBMP("map_mini.bmp", std::ios::binary);
	if (!outputFileMapMiniBMP)
	{
		erorbuildfile();
		return;
	}
	map_mini.seekg(GLOBALNULL, std::ios::end);
	uint32_t size = map_mini.tellg();
	//------------------------------------------------------------------------------
	BITMAPFILEHEADER part1;
	part1.bfType = 19778;
	part1.bfSize = size + (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER)) + (sizeof(RGBQUAD));
	part1.bfReserved1 = GLOBALNULL;
	part1.bfReserved2 = GLOBALNULL;
	part1.bfOffBits = (sizeof(BITMAPFILEHEADER)) + (sizeof(BITMAPINFOHEADER)) + (sizeof(RGBQUAD));
	//------------------------------------------------------------------------------
	BITMAPINFOHEADER part2;
	part2.biSize = sizeof(BITMAPINFOHEADER);
	part2.biWidth = sqrt(size / 2);
	part2.biHeight = sqrt(size / 2);
	part2.biPlanes = 1;
	part2.biBitCount = 16;
	part2.biCompression = 3;
	part2.biSizeImage = GLOBALNULL;
	part2.biXPelsPerMeter = GLOBALNULL;
	part2.biYPelsPerMeter = GLOBALNULL;
	part2.biClrUsed = GLOBALNULL;
	part2.biClrImportant = GLOBALNULL;
	//------------------------------------------------------------------------------
	RGBQUAD part3;
	part3.rgbBlue = 63488;
	part3.rgbGreen = 2016;
	part3.rgbRed = 31;
	//------------------------------------------------------------------------------
	outputFileMapMiniBMP.write((char*)&part1, 14);
	outputFileMapMiniBMP.write((char*)&part2, 40);
	outputFileMapMiniBMP.write((char*)&part3, 12);
	map_mini.seekg(GLOBALNULL);
	char* buffer = new char[size];
	map_mini.read(buffer, size);
	outputFileMapMiniBMP.write(buffer, size);
	//------------------------------------------------------------------------------
	delete[] buffer;
	outputFileMapMiniBMP.close();
	return;
}