#pragma once

#include <bit>

// SSF wire format is little-endian; refuse to compile on big-endian hosts.
static_assert(std::endian::native == std::endian::little);
