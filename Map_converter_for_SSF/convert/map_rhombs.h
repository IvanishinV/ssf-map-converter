#pragma once

#include <cstdint>

#include <filesystem>
#include <string_view>

namespace convert {

void map_rhombs(const std::filesystem::path& map_folder, std::string_view data, uint32_t map_identifier);

} // namespace convert
