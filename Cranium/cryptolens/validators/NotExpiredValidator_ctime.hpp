#pragma once

#include <ctime>

#include "../api.hpp"
#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class NotExpiredValidator_ctime_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  NotExpiredValidator_ctime_(basic_Error & e) {}

  void validate(basic_Error & e, Env & env) {
    std::uint64_t expires = env.get_license_key_information().get_expires();

    auto current = std::time(NULL);
    if (current < 0 || expires < (std::uint64_t)current) {
      using namespace errors;
      e.set(api::main(), Subsystem::Main, Main::KEY_EXPIRED);
    }
  }

private:
};

} // namespace v20190401

namespace latest {

template<typename Env>
using NotExpiredValidator_ctime_ = ::cryptolens_io::v20190401::NotExpiredValidator_ctime_<Env>;

} // namespace latest

} // namespace cryptolens_io
