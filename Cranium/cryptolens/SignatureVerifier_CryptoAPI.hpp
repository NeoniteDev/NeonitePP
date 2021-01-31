#pragma once

#include <string>

#include "imports/windows/Windows.h"
#include "imports/windows/wincrypt.h"

#include "basic_Error.hpp"
#include "Error.hpp"
#include "SignatureVerifier_v20190401_to_v20180502.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier depends on the Microsoft CryptoAPI.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
class SignatureVerifier_CryptoAPI
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  SignatureVerifier_CryptoAPI(basic_Error & e);
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  SignatureVerifier_CryptoAPI(SignatureVerifier_CryptoAPI const&) = delete;
  SignatureVerifier_CryptoAPI(SignatureVerifier_CryptoAPI &&) = delete;
  void operator=(SignatureVerifier_CryptoAPI const&) = delete;
  void operator=(SignatureVerifier_CryptoAPI &&) = delete;
#endif
  ~SignatureVerifier_CryptoAPI();

  void set_modulus_base64(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(basic_Error & e, std::string const& exponent_base64) {}
  bool verify_message(basic_Error & e, std::string const& message, std::string const& signature_base64) const;
private:
  HCRYPTPROV hProv_;
  HCRYPTKEY hPubKey_;

  void init(basic_Error & e);
  void set_modulus_base64_(basic_Error & e, std::string const& modulus_base64);
};

} // namespace v20190401

namespace latest {

using SignatureVerifier_CryptoAPI = ::cryptolens_io::v20190401::SignatureVerifier_CryptoAPI;

} // namespace latest

namespace v20180502 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier depends on the Microsoft CryptoAPI.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
using SignatureVerifier_CryptoAPI = ::cryptolens_io::internal::SignatureVerifier_v20190401_to_v20180502<::cryptolens_io::v20190401::SignatureVerifier_CryptoAPI>;

} // namespace v20180502

} // namespace cryptolens_io
