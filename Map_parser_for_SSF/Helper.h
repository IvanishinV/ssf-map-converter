#pragma once

#include <vector>
#include <string_view>

uint32_t position(const std::string_view& input, std::string_view& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::string_view& input, std::ofstream& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::vector<uint8_t>& input, std::ofstream& output, const uint32_t offset, const uint32_t size);

int8_t readFileInt8(const std::string_view& input, const uint32_t offset);
int8_t readFileInt8(const std::vector<uint8_t>& input, const uint32_t offset);
uint32_t readFileUint32(const std::vector<uint8_t>& input, const uint32_t offset);
uint32_t readFileUint32(const std::string_view& input, const uint32_t offset);

uint32_t minimapsize(uint32_t mapSizeU, uint32_t mapSizeV);
uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);
uint32_t misScripts(const std::string_view& input, uint32_t scripts_number, const uint32_t scripts_position);

void reverse_num(const uint32_t num, std::string& str);