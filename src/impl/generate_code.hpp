#pragma once

#include <string>

namespace short_link::impl {

// @return 8 lenght (= default) std::string that implement code
std::string GenerateCode(std::size_t len = 8) noexcept;

}  // namespace short_link::impl
