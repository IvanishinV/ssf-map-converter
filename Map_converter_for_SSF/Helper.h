#pragma once

#include <ostream>
#include <string_view>
#include <vector>

#include "util.h"

uint32_t position(const std::string_view& input, std::vector<uint8_t>& output, const uint32_t srcOffset, const size_t dstOffset, const uint32_t size);
uint32_t position(const std::string_view& input, std::string_view& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::string_view& input, std::ostream& output, const uint32_t offset, const uint32_t size);
uint32_t position(const std::vector<uint8_t>& input, std::ostream& output, const uint32_t offset, const uint32_t size);

uint32_t minimapsize(const uint32_t mapSizeU, const uint32_t mapSizeV);
uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);
uint32_t misScripts(const std::string_view& input, uint32_t scripts_number, const uint32_t scripts_position);

std::string reverse_num(const uint32_t num);

void flip_v(char* pixels, const size_t height, const size_t width, const size_t pixelSize);