#pragma once

class Parser
{
public:
	void parseMap(const std::string_view& inputData, const std::string_view& filename);

private:
	std::string m_mapFileName;
	std::string_view m_rawMapFileName;
	std::filesystem::path m_mapFolder;
	std::filesystem::path m_misFolder;

	uint32_t parseMapFileSSM(const std::string_view& inputData) const;
	uint32_t parseMapFileSMM(const std::string_view& inputData) const;
	uint32_t parseMapFileSSC_map(const std::string_view& inputData) const;
	uint32_t parseMapFileSCC_mission(const std::string_view& inputData) const;
};
