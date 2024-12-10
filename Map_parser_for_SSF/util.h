#pragma once

#include <cstdint>

const uint32_t FILE_TYPE_OFFSET = 0x0;
const uint32_t MapHeaderSMM = 548;
const uint32_t MapHeaderSSM = 52;
const uint32_t MapHeaderSSC_mission = 24;
const uint32_t MapHeaderSSC_map = 20;
//const uint32_t MapMiniSize = 32768;
const uint32_t LANDNAMESSIZE = 180;
//const uint32_t MISUNITNAMES = 16;
const uint32_t MISGROUPSSIZE = 2700;
const uint32_t MISPLAYERSSIZE = 353;
const uint32_t MISMUSICSIZE = 78;
const uint32_t MISOBJECTS = 400;

//Глобальная константа пустышки
const uint8_t GLOBALNULL = 0x00;

//константа размера файлов для мин и зон
const uint32_t LOCATIONSSIZE = 262144;

//Значение из скриптов при работе с памятью
const uint32_t NUMMEMORY = 3847412;

const uint32_t timeconvertnum = 25;

constexpr uint32_t HEADER_SINGLE = 0x4d535353;	// SSSM
constexpr uint32_t HEADER_MULTI = 0x4d4d5353;	// SSMM
constexpr uint32_t HEADER_CAMP_MAP = 0x504d4143;	// CAMP
constexpr uint32_t HEADER_CAMP_MIS = 0x534d4143;	// CAMS

enum class Action
{
	Parse,
	Convert
};

struct MapHeaderSSM
{
	// 4 буквы вначале, например SSSM
	char		fileType[4];			// offset 0x0
	uint32_t	unknown_1;
	uint32_t	unknown_2;
	uint32_t	unknown_3;
	uint32_t	unknown_4;
	uint32_t	unknown_5;
	uint32_t	unknown_6;
	uint32_t	unknown_7;
	uint32_t	unknown_8;
	uint32_t	unknown_9;
	uint32_t	mapIdentifier;
	uint32_t	mapWidth;
	uint32_t	mapHeight;
	// длина описания миссии
	//uint32_t	descriptionLength;		// offset 0x34
};
struct MapHeaderSMM
{

};