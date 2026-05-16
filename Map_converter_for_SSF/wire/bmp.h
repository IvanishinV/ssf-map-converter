#pragma once

#include "util/endian.h"

#include <cstdint>

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef NOMINMAX
  #define NOMINMAX
  #endif
  #include <Windows.h>
#else
#pragma pack(push, 1)
struct BITMAPFILEHEADER
{
	uint16_t bfType;
	uint32_t bfSize;
	uint16_t bfReserved1;
	uint16_t bfReserved2;
	uint32_t bfOffBits;
};
#pragma pack(pop)

struct BITMAPINFOHEADER
{
	uint32_t biSize;
	int32_t  biWidth;
	int32_t  biHeight;
	uint16_t biPlanes;
	uint16_t biBitCount;
	uint32_t biCompression;
	uint32_t biSizeImage;
	int32_t  biXPelsPerMeter;
	int32_t  biYPelsPerMeter;
	uint32_t biClrUsed;
	uint32_t biClrImportant;
};

inline constexpr uint32_t BI_BITFIELDS = 3;
#endif

static_assert(sizeof(BITMAPFILEHEADER) == 14);
static_assert(sizeof(BITMAPINFOHEADER) == 40);

// Three DWORD channel masks appended after BITMAPINFOHEADER when biCompression == BI_BITFIELDS.
struct BMP_RGB565_MASKS
{
	uint32_t redMask;
	uint32_t greenMask;
	uint32_t blueMask;
};
static_assert(sizeof(BMP_RGB565_MASKS) == 12);

constexpr uint16_t BMP_SIGNATURE     = 0x4d42;   // 'BM'
constexpr uint32_t RGB565_MASK_RED   = 0xF800;   // bits 11..15
constexpr uint32_t RGB565_MASK_GREEN = 0x07E0;   // bits 5..10
constexpr uint32_t RGB565_MASK_BLUE  = 0x001F;   // bits 0..4
