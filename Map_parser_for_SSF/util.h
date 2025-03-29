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
	int32_t startscripts;
};

struct scripts2
{
	int32_t num;
};

// If you need any documentation - there is a separate xlsx file with all the info
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
	bufferSPPL,		// send planes in zone
	bufferPKP,
	bufferPKF,
	bufferETC,
	bufferSTRT,		// set alarm clock
	bufferSTPT,
	bufferMSTL,
	bufferSP,
	bufferSCD,
	bufferSNM,
	bufferTM,		// end mission
	bufferSGB,
	bufferSGL1,
	bufferSGL2,
	bufferSGG1,
	bufferSGG2,
	bufferAPP,
	bufferAFP,
	bufferNF,
	bufferTE,		// alarm clock ringing
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
	bufferSRFS,		// send reinf
	bufferOID,
	bufferSPTO,		// send planes
	bufferSAT,
	bufferARPO,		// add route point
	bufferASPO,		// add drop point
	bufferSPPA,		// send plane along an air route
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
	bufferstart = 0x464F4143,
	bufferEND2	= 0x7fffffff,
};

constexpr size_t MAX_NUM_OF_ARGS_IN_INSTRUCTION = 7;

const uint32_t script_reinf_player = 0x4;
const uint32_t script_reinf_enemy = 0x8;