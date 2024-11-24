
#include "stdafx.h"

#include "Map_mini.h"
#include "General.h"
#include "Displayinfo.h"

struct BITMAPINFOHEADER_
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

struct RGBQUAD_
{
	uint32_t rgbBlue;
	uint32_t rgbGreen;
	uint32_t rgbRed;
	//uint32_t rgbReserved;
};

void covertMapMini(const std::vector<uint8_t>& map_mini)
{
	std::ofstream outputFileMapMiniBMP("map_mini.bmp", std::ios::binary);
	if (!outputFileMapMiniBMP)
	{
		errorBuildFile();
		return;
	}

	const size_t size = map_mini.size();
	BITMAPFILEHEADER part1{};
	BITMAPINFOHEADER part2{};
	RGBQUAD_ part3{};
	//------------------------------------------------------------------------------
	part1.bfType = 19778;
	part1.bfSize = static_cast<DWORD>(size) + (sizeof(part1)) + (sizeof(part2)) + (sizeof(part3));
	part1.bfReserved1 = GLOBALNULL;
	part1.bfReserved2 = GLOBALNULL;
	part1.bfOffBits = (sizeof(part1)) + (sizeof(part2)) + (sizeof(part3));
	//------------------------------------------------------------------------------
	part2.biSize = sizeof(part2);
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
	part3.rgbBlue = 63488;
	part3.rgbGreen = 2016;
	part3.rgbRed = 31;
	//------------------------------------------------------------------------------
	outputFileMapMiniBMP.write((char*)&part1, 14);
	outputFileMapMiniBMP.write((char*)&part2, 40);
	outputFileMapMiniBMP.write((char*)&part3, 12);

	outputFileMapMiniBMP.write((char*)map_mini.data(), size);
	outputFileMapMiniBMP.close();

	return;
}