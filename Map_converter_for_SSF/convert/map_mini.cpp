#include "map_mini.h"
#include "../Displayinfo.h"
#include "../Helper.h"
#include "../Displayinfo.h"
#include "../wire/bmp.h"
#include "../wire/headers.h"

#include <fstream>

namespace convert {

void map_mini(const std::filesystem::path& map_folder,
              uint32_t map_type,
              uint32_t map_size_u,
              uint32_t map_size_v,
              std::string_view data)
{
	std::ofstream f(map_folder / "map_mini.bmp", std::ios::binary);
	if (!f)
	{
		errorWriteFile("map_mini.bmp");
		return;
	}

	// Mini map size logic: standard square maps (128x128, 256x256) use half the map
	// dimensions directly. Non-square or non-power-of-two maps need extra padding.
	bool isDoubled   = true;
	const bool isEven = (map_size_u / 2 & 1) == 0;
	const size_t wideWidth = (isEven ? map_size_u : map_size_u + 2) / 2;

	if ((map_size_u == 128 || map_size_u == 256) &&
	    (map_size_v == 128 || map_size_v == 256) &&
	    map_size_u == map_size_v)
		isDoubled = false;

	const uint32_t defaultSizeU = (map_type == HEADER_CAMP_MAP) ? map_size_u : 128;
	const uint32_t defaultSizeV = (map_type == HEADER_CAMP_MAP) ? map_size_v : 128;

	const size_t bmpWidth  = isDoubled ? map_size_u / 2       : defaultSizeU;
	const size_t bmpHeight = isDoubled ? map_size_v / 2       : defaultSizeV;
	const size_t pixelBytes = (isDoubled ? wideWidth : defaultSizeU) * bmpHeight * sizeof(uint16_t);

	BITMAPFILEHEADER part1{};
	BITMAPINFOHEADER part2{};
	BMP_RGB565_MASKS part3{};

	part1.bfType      = BMP_SIGNATURE;
	part1.bfSize      = static_cast<uint32_t>(pixelBytes + sizeof(part1) + sizeof(part2) + sizeof(part3));
	part1.bfReserved1 = 0;
	part1.bfReserved2 = 0;
	part1.bfOffBits   = sizeof(part1) + sizeof(part2) + sizeof(part3);

	part2.biSize          = sizeof(part2);
	part2.biWidth         = static_cast<int32_t>(bmpWidth);
	part2.biHeight        = static_cast<int32_t>(bmpHeight);
	part2.biPlanes        = 1;
	part2.biBitCount      = 16;
	part2.biCompression   = BI_BITFIELDS;
	part2.biSizeImage     = 0;
	part2.biXPelsPerMeter = 0;
	part2.biYPelsPerMeter = 0;
	part2.biClrUsed       = 0;
	part2.biClrImportant  = 0;

	part3.redMask   = RGB565_MASK_RED;
	part3.greenMask = RGB565_MASK_GREEN;
	part3.blueMask  = RGB565_MASK_BLUE;

	f.write(reinterpret_cast<const char*>(&part1), sizeof(part1));
	f.write(reinterpret_cast<const char*>(&part2), sizeof(part2));
	f.write(reinterpret_cast<const char*>(&part3), sizeof(part3));

	// FIXME: flip_v mutates the const string_view storage (technically UB) and the odd-size
	// else-branch overshoots each row by wideWidth-biWidth pixels, with the final overshoot
	// reading past data.end() into the parent rawData buffer. Output is stable because rawData
	// outlives this call. Fixing means changing the wire format of map_mini.bmp.
	flip_v(const_cast<char*>(data.data()), bmpHeight, bmpWidth, sizeof(uint16_t));
	if (isEven)
		f.write(data.data(), static_cast<std::streamsize>(data.size()));
	else
	{
		for (size_t i = 0; i < bmpHeight; ++i)
			f.write(data.data() + i * bmpWidth * sizeof(uint16_t),
			        static_cast<std::streamsize>(wideWidth * sizeof(uint16_t)));
	}
}

} // namespace convert
