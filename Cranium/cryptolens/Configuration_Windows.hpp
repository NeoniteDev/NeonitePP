#pragma once

#include "ResponseParser_ArduinoJson5.hpp"
#include "RequestHandler_WinHTTP.hpp"
#include "SignatureVerifier_CryptoAPI.hpp"

#include "validators/AndValidator.hpp"
#include "validators/CorrectKeyValidator.hpp"
#include "validators/CorrectProductValidator.hpp"
#include "validators/NotExpiredValidator_kernel32.hpp"
#include "validators/OnValidMachineValidator.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename MachineCodeComputer_>
struct Configuration_Windows {
  using ResponseParser = ResponseParser_ArduinoJson5;
  using RequestHandler = RequestHandler_WinHTTP;
  using SignatureVerifier = SignatureVerifier_CryptoAPI;
  using MachineCodeComputer = MachineCodeComputer_;

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
	  , AndValidator_<Env, CorrectProductValidator_<Env>
	  , AndValidator_<Env, OnValidMachineValidator_<Env>
	  , NotExpiredValidator_kernel32_<Env>
	  >>>;
};

template<typename MachineCodeComputer_>
struct Configuration_Windows_IgnoreExpires {
  using ResponseParser = ResponseParser_ArduinoJson5;
  using RequestHandler = RequestHandler_WinHTTP;
  using SignatureVerifier = SignatureVerifier_CryptoAPI;
  using MachineCodeComputer = MachineCodeComputer_;

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
	  , AndValidator_<Env, CorrectProductValidator_<Env>
	  , OnValidMachineValidator_<Env>
	  >>;
};

} // namespace v20190401

namespace latest {

template<typename MachineCodeComputer_>
using Configuration_Windows = ::cryptolens_io::v20190401::Configuration_Windows<MachineCodeComputer_>;

template<typename MachineCodeComputer_>
using Configuration_Windows_IgnoreExpires = ::cryptolens_io::v20190401::Configuration_Windows_IgnoreExpires<MachineCodeComputer_>;

} // namespace latest

} // namespace cryptolens_io
