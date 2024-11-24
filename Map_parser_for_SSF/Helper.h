#pragma once

#include <vector>
#include <string_view>

uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t startPosition, const uint32_t size);
uint32_t position(const std::string_view& inputFile, std::ofstream& outputFile, const uint32_t startPosition, const uint32_t size);
uint32_t position(const std::vector<uint8_t>& inputFile, std::ofstream& outputFile, const uint32_t startPosition, const uint32_t size);

uint32_t readFileUint32(const std::vector<uint8_t>& input, const uint32_t fileOffset);
uint32_t readFileUint32(const std::string_view& input, const uint32_t fileOffset);