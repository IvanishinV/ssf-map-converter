#pragma once

#include <filesystem>
#include <string>

class Parser
{
public:
	void parseMap(const std::filesystem::path& filepath);

private:
	std::filesystem::path m_mapFolder;
	std::filesystem::path m_misFolder;
	std::string m_stemFileName;
	uint32_t m_mapType;
};
