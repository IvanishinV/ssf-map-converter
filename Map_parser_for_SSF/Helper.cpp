
#include "stdafx.h"

#include "Helper.h"


uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t srcOffset, const size_t dstOffset, const uint32_t size)
{
	std::copy(input.data() + srcOffset, input.data() + srcOffset + size, output.begin() + dstOffset);
	return srcOffset + size;
}

uint32_t position(const std::string_view& input, std::string_view& output, const uint32_t offset, const uint32_t size)
{
	output = input.substr(offset, size);
	return offset + size;
}

uint32_t position(const std::string_view& input, std::ostream& output, const uint32_t offset, const uint32_t size)
{
	output.write(input.data() + offset, size);

	return offset + size;
}

uint32_t position(const std::vector<uint8_t>& input, std::ostream& output, const uint32_t offset, const uint32_t size)
{
	output.write((const char*)input.data() + offset, size);

	return offset + size;
}


int8_t readFileInt8(const std::string_view& input, const uint32_t offset)
{
	return *reinterpret_cast<const int8_t*>(input.data() + offset);
}

int8_t readFileInt8(const std::vector<uint8_t>& input, const uint32_t offset)
{
	return *reinterpret_cast<const int8_t*>(input.data() + offset);
}

uint32_t readFileUint32(const std::vector<uint8_t>& input, const uint32_t offset)
{
	return *reinterpret_cast<const uint32_t*>(input.data() + offset);
}

uint32_t readFileUint32(const std::string_view& input, const uint32_t offset)
{
	return *reinterpret_cast<const uint32_t*>(input.data() + offset);
}


uint32_t minimapsize(const uint32_t mapSizeU, const uint32_t mapSizeV)
{
	// 0x100 0x100	32768
	// 0x96 0x96	11250
	// 0x80 0x80	32768
	// 0x80 0x100	16384
	static constexpr int MINI_MAP_DEFAULT_SIZE = 0x8000;

	if ((mapSizeU == 128 || mapSizeU == 256) && (mapSizeV == 128 || mapSizeV == 256) && mapSizeU == mapSizeV)
	{
		return MINI_MAP_DEFAULT_SIZE;
	}
	else
	{
		return mapSizeU * mapSizeV / 2;
	}
}

uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number)
{
	return mapSizeU * mapSizeV * number;
}

uint32_t misScripts(const std::string_view& inputFile, uint32_t scripts_number, const uint32_t scripts_position)
{
	if (scripts_number == 0)
	{
		return 0;
	}
	else
	{
		uint32_t accumulator = 0;
		while (scripts_number--)
		{
			uint32_t COAF;
			COAF = readFileUint32(inputFile, scripts_position + 4 + accumulator);
			accumulator += COAF + 4;
		}
		return accumulator;
	}
}


std::string reverse_num(const uint32_t num)
{
	std::string str = std::to_string(num);
	if (num <= 9)
		return str;
	std::reverse(str.begin(), str.end());
	return str;
}


void flip_v(char* pixels, const size_t height, const size_t width, const size_t pixelSize)
{
	std::vector<uint8_t> temp(width * pixelSize);

	for (size_t i = 0; i < height / 2; ++i)
	{
		const auto& src = &pixels[i * width * pixelSize];
		const auto& dst = &pixels[(height - i - 1) * width * pixelSize];
		std::copy(dst, dst + width * pixelSize, temp.data());
		std::copy(src, src + width * pixelSize, dst);
		std::copy(temp.data(), temp.data() + width * pixelSize, src);
	}
}