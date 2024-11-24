#pragma once


uint32_t parserMapFileSSM(const std::string_view& inputFile);
uint32_t parserMapFileSMM(const std::string_view& inputFile);
uint32_t parserMapFileSSC_map(const std::string_view& inputFile);
uint32_t parserMapFileSCC_mission(const std::string_view& inputFile);

//void displayinfo(uint32_t  mapSizeU, uint32_t mapSizeV, const uint32_t mapIdentifier, const uint32_t mapEndPosition);

uint32_t minimapsize(uint32_t mapSizeU, uint32_t mapSizeV);

uint32_t tileArray(const uint32_t mapSizeU, const uint32_t mapSizeV, const uint32_t number);

uint32_t misScripts(const std::string_view& inputFile, uint32_t scripts_number, const uint32_t scripts_position);
