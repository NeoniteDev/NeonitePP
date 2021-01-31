#pragma once

#include "ResponseParser_ArduinoJson5.hpp"
#include "RequestHandler_curl.hpp"
#include "SignatureVerifier_OpenSSL.hpp"

#include "validators/AndValidator.hpp"
#include "validators/CorrectKeyValidator.hpp"
#include "validators/CorrectProductValidator.hpp"
#include "validators/NotExpiredValidator_ctime.hpp"
#include "validators/OnValidMachineValidator.hpp"

namespace cryptolens_io {

namespace v20190401 {

template<typename MachineCodeComputer_>
struct Configuration_Unix {
  using ResponseParser = ResponseParser_ArduinoJson5;
  using RequestHandler = RequestHandler_curl;
  using SignatureVerifier = SignatureVerifier_OpenSSL;
  using MachineCodeComputer = MachineCodeComputer_;

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                          , AndValidator_<Env, CorrectProductValidator_<Env>
                          , AndValidator_<Env, OnValidMachineValidator_<Env>
                          ,                    NotExpiredValidator_ctime_<Env>
                          >>>;
};

template<typename MachineCodeComputer_>
struct Configuration_Unix_IgnoreExpires {
  using ResponseParser = ResponseParser_ArduinoJson5;
  using RequestHandler = RequestHandler_curl;
  using SignatureVerifier = SignatureVerifier_OpenSSL;
  using MachineCodeComputer = MachineCodeComputer_;

  template<typename Env>
  using ActivateValidator = AndValidator_<Env, CorrectKeyValidator_<Env>
                          , AndValidator_<Env, CorrectProductValidator_<Env>
                          ,                    OnValidMachineValidator_<Env>
                          >>;
};

} // namespace v20190401

namespace latest {

template<typename MachineCodeComputer_>
using Configuration_Unix = ::cryptolens_io::v20190401::Configuration_Unix<MachineCodeComputer_>;

template<typename MachineCodeComputer_>
using Configuration_Unix_IgnoreExpires = ::cryptolens_io::v20190401::Configuration_Unix_IgnoreExpires<MachineCodeComputer_>;

} // namespace latest

} // namespace cryptolens_io
