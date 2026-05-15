#pragma once

#include <ostream>
#include <string_view>
#include <type_traits>
#include <vector>

#include "util.h"

// Slice `size` bytes from `input` starting at `offset` into `output`.
// `output` may be std::string_view (rebinds to a sub-view) or any std::ostream& (writes the bytes).
// Returns offset + size for chaining.
template <typename Output>
uint32_t position(std::string_view input, Output& output, uint32_t offset, uint32_t size)
{
	if constexpr (std::is_same_v<std::remove_cvref_t<Output>, std::string_view>)
		output = input.substr(offset, size);
	else
		output.write(input.data() + offset, size);
	return offset + size;
}

uint32_t minimapsize(const uint32_t mapSizeU, const uint32_t mapSizeV);
uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);
uint32_t misScripts(const std::string_view& input, uint32_t scripts_number, const uint32_t scripts_position);

std::string reverse_num(const uint32_t num);

void flip_v(char* pixels, const size_t height, const size_t width, const size_t pixelSize);