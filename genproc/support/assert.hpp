#pragma once

#include <fmt/core.h>

#include <string>

namespace cmp_lattice::support {

struct AssertionException {};

void Assert(bool cond, const std::string& msg) {
    if (!cond) {
        fmt::println("Assertion failed:\n{}", msg);
        throw AssertionException{};
    }
}

} // namespace cmp::lattice::support