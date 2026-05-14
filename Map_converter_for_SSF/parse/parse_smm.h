#pragma once

#include <filesystem>
#include <string_view>

namespace parse {

void parse_smm(const std::filesystem::path& map_folder, std::string_view data);

} // namespace parse
