#pragma once

#include <filesystem>
#include <string_view>

namespace convert {

void mis_scripts(const std::filesystem::path& mis_folder,
                 std::string_view stem_name,
                 std::string_view data);

} // namespace convert
