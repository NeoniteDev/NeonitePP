#pragma once

#include <string>

#include "imports/std/optional"

#include "basic_Error.hpp"
#include "base64.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * This class represents a raw reply from the Cryptolens Web API with
 * a license key.
 *
 * This object cannot be queried directly, instead a LicenseKey
 * object must be created, e.g:
 *
 *     LicenseKey key = LicenseKey::make(raw_license_key)
 *
 */
class RawLicenseKey {
  RawLicenseKey
    ( std::string base64_license
    , std::string signature
    , std::string decoded_license
    )
  : base64_license_(std::move(base64_license))
  , signature_(std::move(signature))
  , license_(std::move(decoded_license))
  { }

  std::string base64_license_;
  std::string signature_;
  std::string license_;
public:
  std::string const& get_base64_license() const;

  std::string const& get_signature() const;

  std::string const& get_license() const;

  template<typename SignatureVerifier>
  static
  optional<RawLicenseKey>
  make
    ( basic_Error & e
    , SignatureVerifier const& verifier
    , std::string base64_license
    , std::string signature
    )
  {
    if (e) { return nullopt; }

    optional<std::string> decoded = ::cryptolens_io::v20190401::internal::b64_decode(base64_license);

    if (!decoded) {
      e.set(api::main(), errors::Subsystem::Base64);
      return nullopt;
    }

    if (verifier.verify_message(e, *decoded, signature)) {
      return make_optional(
        RawLicenseKey
          ( std::move(base64_license)
          , std::move(signature)
          , std::move(*decoded)
          )
        );
    } else {
      return nullopt;
    }
  }
};

} // namespace v20190401

namespace v20180502 {

using RawLicenseKey = ::cryptolens_io::v20190401::RawLicenseKey;

} // namespace v20180502

namespace latest {

using RawLicenseKey = ::cryptolens_io::v20190401::RawLicenseKey;

} // namespace latest

} // namespace cryptolens_io
