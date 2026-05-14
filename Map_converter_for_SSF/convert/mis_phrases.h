#pragma once

#include <cstdint>
#include <filesystem>
#include <string_view>

namespace convert {

void mis_phrases(const std::filesystem::path& mis_folder,
                 std::string_view data,
                 uint32_t size_mis_phrases);

} // namespace convert
