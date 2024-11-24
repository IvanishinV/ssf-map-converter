#pragma once


uint32_t parserMapFileSSM(std::stringstream& inputFile);
uint32_t parserMapFileSMM(std::stringstream& inputFile);
uint32_t parserMapFileSSC_map(std::stringstream& inputFile);
uint32_t parserMapFileSCC_mission(std::stringstream& inputFile);

//void displayinfo(uint32_t  mapSizeU, uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition);

uint32_t minimapsize(uint32_t  mapSizeU, uint32_t mapSizeV);

uint32_t readFileUint32(std::stringstream& inputFile, const uint32_t fileOffset);

uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);

uint32_t misScripts(std::stringstream& inputFile, uint32_t scripts_number, const uint32_t scripts_position);
