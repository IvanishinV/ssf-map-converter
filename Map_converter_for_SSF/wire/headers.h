#pragma once

#include "util/endian.h"

#include <cstdint>

constexpr uint32_t HEADER_SINGLE   = 0x4d535353;	// SSSM
constexpr uint32_t HEADER_MULTI    = 0x4d4d5353;	// SSMM
constexpr uint32_t HEADER_CAMP_MAP = 0x504d4143;	// CAMP
constexpr uint32_t HEADER_CAMP_MIS = 0x534d4143;	// CAMS

constexpr uint32_t FILE_TYPE_OFFSET     = 0x0;
constexpr uint32_t MapHeaderSMM         = 148;
constexpr uint32_t MapHeaderSSM         = 52;
constexpr uint32_t MapHeaderSSC_mission = 24;
constexpr uint32_t MapHeaderSSC_map     = 20;

constexpr uint32_t LANDNAMESSIZE  = 180;
constexpr uint32_t MISGROUPSSIZE  = 2700;
constexpr uint32_t MISPLAYERSSIZE = 353;
constexpr uint32_t MISMUSICSIZE   = 78;
constexpr uint32_t MISOBJECTS     = 400;
