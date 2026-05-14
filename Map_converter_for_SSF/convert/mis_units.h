#pragma once

#include <filesystem>
#include <string_view>

namespace convert {

void mis_units(const std::filesystem::path& mis_folder,
               std::string_view mis_unitnames,
               std::string_view mis_mapunits,
               std::string_view mis_support);

} // namespace convert
