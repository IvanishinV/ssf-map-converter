#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace convert {

void map_desc(const std::filesystem::path& map_folder,
              uint32_t map_type,
              std::string_view stem);

} // namespace convert
