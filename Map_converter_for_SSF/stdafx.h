#pragma once

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
  #endif

  #ifndef NOMINMAX
  #define NOMINMAX
  #endif

  #include <Windows.h>
  #include <conio.h>
#else
  #include <cstdint>

  using DWORD = uint32_t;
  using LONG  = int32_t;
  using INT   = int;
  using WORD  = uint16_t;
  using BYTE  = uint8_t;

  inline constexpr uint32_t BI_RGB       = 0;
  inline constexpr uint32_t BI_BITFIELDS = 3;

  #define LOWORD(l)        ((WORD)((uint32_t)(l) & 0xFFFF))
  #define HIWORD(l)        ((WORD)(((uint32_t)(l) >> 16) & 0xFFFF))
  #define LOBYTE(w)        ((BYTE)((uint16_t)(w) & 0xFF))
  #define HIBYTE(w)        ((BYTE)(((uint16_t)(w) >> 8) & 0xFF))
  #define MAKEWORD(a, b)   ((WORD)(((BYTE)((uint32_t)(a) & 0xFF)) | (((WORD)((BYTE)((uint32_t)(b) & 0xFF))) << 8)))
  #define MAKELONG(a, b)   ((uint32_t)(((WORD)((uint32_t)(a) & 0xFFFF)) | (((uint32_t)((WORD)((uint32_t)(b) & 0xFFFF))) << 16)))

  #pragma pack(push, 1)
  struct BITMAPFILEHEADER {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
  };
  #pragma pack(pop)

  struct BITMAPINFOHEADER {
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

  struct RGBQUAD {
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
  };
#endif

#include <iostream>
#include <fstream>
#include <print>

#include <array>
#include <map>
#include <vector>
#include <stack>
#include <string>

#include <math.h>

#include <filesystem>
#include <random>
#include <chrono>
#include <future>
#include <stdexcept>

#include "Lang.h"