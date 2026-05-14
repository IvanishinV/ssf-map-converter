#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace convert {

void mis_mult(const std::filesystem::path& mis_folder,
              std::string_view map_info,
              uint32_t maptypeheader);

} // namespace convert
