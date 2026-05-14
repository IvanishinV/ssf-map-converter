#pragma once

#include <cstdint>
#include <filesystem>

namespace convert {

void mis_info(const std::filesystem::path& mis_folder,
              uint32_t map_size_u,
              uint32_t map_size_v);

} // namespace convert
