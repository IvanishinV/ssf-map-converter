#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace convert {

void mis_zones(const std::filesystem::path& mis_folder,
               std::string_view data,
               uint32_t map_size_u);

} // namespace convert
