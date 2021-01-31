#pragma once

#include "imports/std/optional"

#include <utility>

#include "basic_Error.hpp"
#include "LicenseKeyInformation.hpp"
#include "RawLicenseKey.hpp"

namespace cryptolens_io {

namespace v20190401 {

class ResponseParser_ArduinoJson5 {
/*
 * Note the API of this class is not considered stable. Please contact us if you would like to create
 * a custom ResponseParser.
 */
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  ResponseParser_ArduinoJson5(basic_Error & e) {}

  optional<LicenseKeyInformation> make_license_key_information(basic_Error & e, RawLicenseKey const& raw_license_key) const;
  optional<LicenseKeyInformation> make_license_key_information(basic_Error & e, optional<RawLicenseKey> const& raw_license_key) const;
  optional<LicenseKeyInformation> make_license_key_information_unsafe(basic_Error & e, std::string const& license_key) const;

  optional<std::pair<std::string, std::string>> parse_activate_response(basic_Error & e, std::string const& server_response) const;
  std::string parse_create_trial_key_response(basic_Error & e, std::string const& server_response) const;
  std::string parse_last_message_response(basic_Error & e, std::string const& server_response) const;
};

} // namespace latest

namespace latest {

using ResponseParser_ArduinoJson5 = ::cryptolens_io::v20190401::ResponseParser_ArduinoJson5;

} // namespace latest

} // namespace cryptolens_io
