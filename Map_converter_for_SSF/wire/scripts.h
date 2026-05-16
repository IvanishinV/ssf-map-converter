#pragma once

#include <cstdint>
#include <cstddef>

struct scripts1
{
	uint32_t size_of_script;
	int32_t startscripts;
};

struct scripts2
{
	int32_t num;
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
	bufferSRES,		// initial reserve
	bufferFRES,
	bufferPFF,		// planes for flags
	bufferMFF,		// message for flags
	bufferFPFF,		// send planes for flags
	bufferSPPO,
	bufferDGP,
	bufferFVKGZ,
	bufferFVKGO = 0x3C,
	bufferstart = 0x464F4143,
	bufferEND2	= 0x7fffffff,
};

static_assert(bufferSRES  == 52, "bufferSRES should be 52");
static_assert(bufferPFF   == 54, "bufferPFF should be 54");
static_assert(bufferMFF   == 55, "bufferMFF should be 55");
static_assert(bufferFPFF  == 56, "bufferFPFF should be 56");

constexpr size_t MAX_NUM_OF_ARGS_IN_INSTRUCTION = 7;

constexpr uint32_t script_reinf_player = 0x4;
constexpr uint32_t script_reinf_enemy = 0x8;
