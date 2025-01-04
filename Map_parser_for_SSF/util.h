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

struct scripts1
{
	uint32_t size_of_script;
	uint8_t startscripts1;
	uint8_t startscripts2;
	uint8_t startscripts3;
	uint8_t startscripts4;
};
struct scripts2
{
	uint8_t num1;
	uint8_t num2;
	uint8_t num3;
	uint8_t num4;
};

enum scripts_num
{
	bufferNONE = 0x00,
	bufferAND,
	bufferOR,
	bufferUG,
	bufferUP,
	bufferUGL,
	bufferUPL,
	bufferEND,
	bufferSPPL,
	bufferPKP,
	bufferPKF,
	bufferETC,
	bufferSTRT,
	bufferSTPT,
	bufferMSTL,
	bufferSP,
	bufferSCD,
	bufferSNM,
	bufferTM,
	bufferSGB,
	bufferSGL1,
	bufferSGL2,
	bufferSGG1,
	bufferSGG2,
	bufferAPP,
	bufferAFP,
	bufferNF,
	bufferTE,
	bufferCD,
	bufferTMS,
	bufferUGLper,
	bufferUPLper,
	bufferUGper,
	bufferUPper,
	bufferAIGB,
	bufferAIZ1,
	bufferAIZ2,
	bufferAIG1,
	bufferAIG2,
	bufferRU,
	bufferSRFS,
	bufferOID,
	bufferSPTO,
	bufferSAT,
	bufferARPO,
	bufferARPO2,
	bufferSPPA,
	bufferLCCV,
	bufferMO,
	bufferGWATA,
	bufferVIC,
	bufferMIST,
	bufferSRES,
	bufferFRES,
	bufferPFF,
	bufferFPFF,
	bufferMFF,
	bufferSPPO,
	bufferDGP,
	bufferFVKGZ,
	bufferFVKGO = 0x3C,
	START		= 0x7F,
	END			= 0x87,
	bufferstart = 0x464F4143,
	bufferEND2	= 0x7fffffff,
};
