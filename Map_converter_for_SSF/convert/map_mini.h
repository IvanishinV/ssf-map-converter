#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace convert {

void map_mini(const std::filesystem::path& map_folder,
              uint32_t map_type,
              uint32_t map_size_u,
              uint32_t map_size_v,
              std::string_view data);

} // namespace convert
