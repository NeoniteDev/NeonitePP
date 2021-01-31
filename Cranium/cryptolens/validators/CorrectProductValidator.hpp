#pragma once

#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class CorrectProductValidator_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  CorrectProductValidator_(basic_Error & e) {}

  void
  validate(basic_Error & e, Env & env) {
    if (e) { return; }

    int expected_product_id = env.get_product_id();
    int product_id = env.get_license_key_information().get_product_id();

    if (product_id != expected_product_id) {
      e.set(api::main(), errors::Subsystem::Main, errors::Main::UNKNOWN_SERVER_REPLY);
    }
  }
};

} // namespace v20190401

namespace latest {

template<typename Env>
using CorrectProductValidator_ = ::cryptolens_io::v20190401::CorrectProductValidator_<Env>;

} // namespace latest

} // namespace cryptolens_io
