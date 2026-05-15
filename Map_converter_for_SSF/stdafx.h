#pragma once

#ifdef _WIN32
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
  #endif

  #ifndef NOMINMAX
  #define NOMINMAX
  #endif

  #include <Windows.h>
#else
  #include <cstdint>

  using DWORD = uint32_t;
  using LONG  = int32_t;
  using INT   = int;
  using WORD  = uint16_t;
  using BYTE  = uint8_t;

  #define LOWORD(l)        ((WORD)((uint32_t)(l) & 0xFFFF))
  #define HIWORD(l)        ((WORD)(((uint32_t)(l) >> 16) & 0xFFFF))
  #define LOBYTE(w)        ((BYTE)((uint16_t)(w) & 0xFF))
  #define HIBYTE(w)        ((BYTE)(((uint16_t)(w) >> 8) & 0xFF))
  #define MAKEWORD(a, b)   ((WORD)(((BYTE)((uint32_t)(a) & 0xFF)) | (((WORD)((BYTE)((uint32_t)(b) & 0xFF))) << 8)))
  #define MAKELONG(a, b)   ((uint32_t)(((WORD)((uint32_t)(a) & 0xFFFF)) | (((uint32_t)((WORD)((uint32_t)(b) & 0xFFFF))) << 16)))
#endif

