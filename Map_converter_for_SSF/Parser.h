#pragma once

class Parser
{
public:
	void parseMap(const std::filesystem::path& filepath);

private:
	std::filesystem::path m_mapFolder;
	std::filesystem::path m_misFolder;
	std::string m_stemFileName;
	uint32_t m_mapType;

	uint32_t parseMapFileSSM(const std::string_view& inputData) const;
	uint32_t parseMapFileSMM(const std::string_view& inputData) const;
	uint32_t parseMapFileSSC_map(const std::string_view& inputData) const;
	uint32_t parseMapFileSCC_mission(const std::string_view& inputData) const;
};
