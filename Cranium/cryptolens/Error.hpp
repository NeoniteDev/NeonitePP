#pragma once

#include "basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

class Error : public basic_Error {
public:
  // Declarations below hide these methods inherited from the basic_Error class
  using basic_Error::get_subsystem;
  using basic_Error::get_reason;
  using basic_Error::get_extra;
  using basic_Error::reset;

  int get_subsystem() const noexcept { return get_subsystem(api::main()); }
  int get_reason() const noexcept { return get_reason(api::main()); }
  size_t get_extra() const noexcept { return get_extra(api::main()); }

  void reset() { reset(api::main()); }
};

} // namespace v20190401

namespace v20180502 {

using Error = ::cryptolens_io::v20190401::Error;

} // namespace v20180502

namespace latest {

using Error = ::cryptolens_io::v20190401::Error;

} // namespace latest

} // namespace cryptolens_io
