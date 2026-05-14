#pragma once

#include <filesystem>
#include <string_view>

namespace convert {

void map_objects(const std::filesystem::path& map_folder,
                 std::string_view data);

} // namespace convert
