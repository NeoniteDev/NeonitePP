#pragma once

#include "../imports/windows/Windows.h"

#include "../api.hpp"
#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class NotExpiredValidator_kernel32_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  NotExpiredValidator_kernel32_(basic_Error & e) {}

  void validate(basic_Error & e, Env & env) {
    std::uint64_t expires = env.get_license_key_information().get_expires();

    if (expires < get_current_()) {
      using namespace errors;
      e.set(api::main(), Subsystem::Main, Main::KEY_EXPIRED);
    }
  }

private:
  std::uint64_t get_current_() {
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    constexpr std::uint64_t EPOCH = 0x019DB1DED53E8000;
    constexpr std::uint64_t TICKS_PER_SECOND = 10000000;

    std::uint64_t tlow = ft.dwLowDateTime;
    std::uint64_t thigh = ft.dwHighDateTime;
    std::uint64_t t = thigh << 32 | tlow;
    return (t - EPOCH) / TICKS_PER_SECOND;
  }
};

} // namespace v20190401

namespace latest {

template<typename Env>
using NotExpiredValidator_kernel32_ = ::cryptolens_io::v20190401::NotExpiredValidator_kernel32_<Env>;

} // namespace latest

} // namespace cryptolens_io
