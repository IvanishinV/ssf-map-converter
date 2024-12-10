
#include "stdafx.h"

#include "Helper.h"


uint32_t position(const std::string_view& input, std::string_view& output, const uint32_t offset, const uint32_t size)
{
	output = input.substr(offset, size);
	return offset + size;
}

uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t offset, const uint32_t size)
{
	output.insert(output.end(), input.cbegin() + offset, input.cbegin() + offset + size);

	return offset + size;
}

uint32_t position(const std::string_view& input, std::ofstream& output, const uint32_t offset, const uint32_t size)
{
	output.write(input.data() + offset, size);
	output.close();

	return offset + size;
}

uint32_t position(const std::vector<uint8_t>& input, std::ofstream& output, const uint32_t offset, const uint32_t size)
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


uint32_t minimapsize(uint32_t mapSizeU, uint32_t mapSizeV)
{
	if (mapSizeU <= 128 && mapSizeV <= 128 && mapSizeU == mapSizeV)
	{
		uint32_t MiniSize = mapSizeU * mapSizeV * 2;
		return MiniSize;
	}
	else
	{
		uint32_t MiniSize = mapSizeU * mapSizeV / 2;
		return MiniSize;
	}
}

uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number)
{
	uint32_t result;
	result = mapSizeU * mapSizeV * number;
	return result;
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


void reverse_num(const uint32_t num1, std::string& str)
{
	str = std::to_string(num1);
	reverse(str.begin(), str.end());
}