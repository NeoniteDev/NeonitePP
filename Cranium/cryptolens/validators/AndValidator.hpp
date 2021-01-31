#pragma once

#include "../basic_Error.hpp"
#include "TautologyValidator.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env, typename H, typename T = TautologyValidator_<Env>>
class AndValidator_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  AndValidator_(basic_Error & e) : head(e), tail(e) {}

  void
  validate(basic_Error & e, Env & env) {
    // Should we add checks if e is in an error state?
    // This is not neccessary given that H and T correctly
    // checks e. But maybe just in case?

    head.validate(e, env);
    tail.validate(e, env);
  }

private:
  H head;
  T tail;
};

} // namespace v20190401

namespace latest {

template<typename Env, typename H, typename T = TautologyValidator_<Env>>
using AndValidator_ = ::cryptolens_io::v20190401::AndValidator_<Env, H, T>;

} // namespace latest

} // namespace cryptolens_io
