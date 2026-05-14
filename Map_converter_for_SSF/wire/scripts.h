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

constexpr uint32_t script_reinf_player = 0x4;
constexpr uint32_t script_reinf_enemy = 0x8;
