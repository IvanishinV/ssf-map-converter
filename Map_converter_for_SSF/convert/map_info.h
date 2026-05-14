#pragma once

#include <cstdint>
#include <filesystem>

namespace convert {

void map_info(const std::filesystem::path& map_folder,
              uint32_t map_identifier,
              uint32_t map_size_u,
              uint32_t map_size_v);

} // namespace convert
