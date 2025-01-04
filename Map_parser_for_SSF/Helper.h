#pragma once

#include <vector>
#include <string_view>

#include "util.h"

uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t srcOffset, const size_t dstOffset, const uint32_t size);
uint32_t position(const std::string_view& input, std::string_view& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::string_view& input, std::ostream& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::vector<uint8_t>& input, std::ostream& output, const uint32_t offset, const uint32_t size);

int8_t readFileInt8(const std::string_view& input, const uint32_t offset);
int8_t readFileInt8(const std::vector<uint8_t>& input, const uint32_t offset);
uint32_t readFileUint32(const std::vector<uint8_t>& input, const uint32_t offset);
uint32_t readFileUint32(const std::string_view& input, const uint32_t offset);

uint32_t readUint32(scripts2& scripts);

uint32_t minimapsize(uint32_t mapSizeU, uint32_t mapSizeV);
uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);
uint32_t misScripts(const std::string_view& input, uint32_t scripts_number, const uint32_t scripts_position);

std::string reverse_num(const uint32_t num);