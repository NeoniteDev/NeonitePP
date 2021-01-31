#pragma once

#include "../basic_Error.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename Env>
class OnValidMachineValidator_ {
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  OnValidMachineValidator_(basic_Error & e) {}

  void
  validate(basic_Error & e, Env & env) {
    if (e) { return; }

    auto const& license_key_information = env.get_license_key_information();
    auto const& expected_machine_code = env.get_machine_code();
    bool floating = env.get_floating();

    auto const& machines = license_key_information.get_activated_machines();
    auto const& maxnoofmachines = license_key_information.get_maxnoofmachines();
    bool valid = (maxnoofmachines && *maxnoofmachines == 0)
	      || !machines;
    if (machines && !valid) {
      std::string full_expected_machine_code;
      if (floating) { full_expected_machine_code += "floating:"; }
      full_expected_machine_code += expected_machine_code;

      for (auto const& m : *machines) {
        if (m.get_mid() == full_expected_machine_code) {
          valid = true;
          break;
        }
      }
    }

    if (!valid) {
      e.set(api::main(), errors::Subsystem::Main, errors::Main::UNKNOWN_SERVER_REPLY);
    }
  }
};

} // namespace v20190401

namespace latest {

template<typename Env>
using OnValidMachineValidator_ = ::cryptolens_io::v20190401::OnValidMachineValidator_<Env>;

} // namespace latest

} // namespace cryptolens_io
