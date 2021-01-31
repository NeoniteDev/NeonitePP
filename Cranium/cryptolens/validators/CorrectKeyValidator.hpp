#pragma once

#include "../api.hpp"
#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class CorrectKeyValidator_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  CorrectKeyValidator_(basic_Error & e) {}

  void
  validate(basic_Error & e, Env & env) {
    if (e) { return; }

    auto const& expected_key = env.get_key();
    auto const& key = env.get_license_key_information().get_key();

    if (key && key != expected_key) {
      e.set(api::main(), errors::Subsystem::Main, errors::Main::UNKNOWN_SERVER_REPLY);
    }
  }
};

} // namespace v20190401

namespace latest {

template<typename Env>
using CorrectKeyValidator_ = ::cryptolens_io::v20190401::CorrectKeyValidator_<Env>;

} // namespace latest

} // namespace cryptolens_io
