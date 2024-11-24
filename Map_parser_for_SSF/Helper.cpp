
#include "stdafx.h"

uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t startPosition, const uint32_t size)
{
	output.insert(output.end(), input.cbegin() + startPosition, input.cbegin() + startPosition + size);
	//std::copy(input.cbegin() + startPosition, input.cbegin() + startPosition + size, 
	//output.write(input.data() + startPosition, size);

	return startPosition + size;
}

uint32_t position(const std::string_view& inputFile, std::ofstream& outputFile, const uint32_t startPosition, const uint32_t size)
{
	outputFile.write(inputFile.data() + startPosition, size);
	outputFile.close();

	return startPosition + size;
}

uint32_t position(const std::vector<uint8_t>& inputFile, std::ofstream& outputFile, const uint32_t startPosition, const uint32_t size)
{
	outputFile.write((const char*)inputFile.data() + startPosition, size);

	return startPosition + size;
}

uint32_t readFileUint32(const std::vector<uint8_t>& input, const uint32_t fileOffset)
{
	return *reinterpret_cast<const uint32_t*>(input.data() + fileOffset);
}

uint32_t readFileUint32(const std::string_view& input, const uint32_t fileOffset)
{
	return *reinterpret_cast<const uint32_t*>(input.data() + fileOffset);
}