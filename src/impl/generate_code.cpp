#include "generate_code.hpp"

#include <random>
#include <string>
#include <userver/engine/mutex.hpp>

namespace {
userver::engine::Mutex rnd_mutex;
constexpr const char* kBase62 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
std::mt19937 rand_generator(std::random_device{}());
std::uniform_int_distribution<std::size_t> dist(0, 61);

char GetRandomBase62Char() noexcept { return kBase62[dist(rand_generator)]; }

}  // namespace

namespace short_link::impl {

std::string GenerateCode(std::size_t len) noexcept {
    std::string code(len, ' ');
    auto lock = std::lock_guard{rnd_mutex};
    for (std::size_t i = 0; i < len; ++i) {
        code[i] = GetRandomBase62Char();
    }
    return code;
}

}  // namespace short_link::impl