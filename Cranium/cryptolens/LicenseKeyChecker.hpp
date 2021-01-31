#pragma once

#include "LicenseKeyInformation.hpp"

namespace cryptolens_io {

namespace v20190401 {

class LicenseKeyInformation;

class LicenseKeyChecker {
  bool status_;
  LicenseKeyInformation const* key_;

public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  LicenseKeyChecker(LicenseKeyInformation const* license_key);

  explicit operator bool() const;

  LicenseKeyChecker& has_feature(int feature);
  LicenseKeyChecker& has_not_feature(int feature);
  LicenseKeyChecker& has_expired(std::uint64_t now);
  LicenseKeyChecker& has_not_expired(std::uint64_t now);
  LicenseKeyChecker& is_blocked();
  LicenseKeyChecker& is_not_blocked();
  LicenseKeyChecker& is_on_right_machine(std::string const& machine_code);
};

} // namespace v20190401

namespace v20180502 {

using LicenseKeyChecker = ::cryptolens_io::v20190401::LicenseKeyChecker;

} // namespace v20180502

namespace latest {

using LicenseKeyChecker = ::cryptolens_io::v20190401::LicenseKeyChecker;

} // namespace latest

} // namespace cryptolens_io
