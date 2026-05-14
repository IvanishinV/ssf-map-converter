#pragma once

#include "wire/headers.h"
#include "wire/scripts.h"
#include "wire/types.h"

#include <cstdint>

enum class RGB565_SHIFT : uint8_t { R = 3, G = 2, B = 3 };

constexpr uint8_t  GLOBALNULL      = 0x00;
constexpr uint32_t LOCATIONSSIZE   = 262144;
constexpr uint32_t NUMMEMORY       = 3847412;
constexpr uint32_t timeconvertnum  = 25;

enum class Action
{
	Parse,
	Convert
};
