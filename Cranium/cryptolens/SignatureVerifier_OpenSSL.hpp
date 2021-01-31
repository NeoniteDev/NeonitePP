#pragma once

#include <string>

#include "imports/openssl/rsa.h"

#include "basic_Error.hpp"
#include "Error.hpp"
#include "SignatureVerifier_v20190401_to_v20180502.hpp"

namespace cryptolens_io {

namespace v20190401 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier can be used with the OpenSSL or LibreSSL libraries.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
class SignatureVerifier_OpenSSL
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  SignatureVerifier_OpenSSL(basic_Error & e);
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  SignatureVerifier_OpenSSL(SignatureVerifier_OpenSSL const&) = delete;
  SignatureVerifier_OpenSSL(SignatureVerifier_OpenSSL &&) = delete;
  void operator=(SignatureVerifier_OpenSSL const&) = delete;
  void operator=(SignatureVerifier_OpenSSL &&) = delete;
#endif
  ~SignatureVerifier_OpenSSL();

  void set_modulus_base64(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(basic_Error & e, std::string const& exponent_base64);

  bool verify_message(basic_Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  RSA * rsa;

  void set_modulus_base64_(basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64_(basic_Error & e, std::string const& exponent_base64);
};

} // namespace v20190401

namespace latest {

using SignatureVerifier_OpenSSL = ::cryptolens_io::v20190401::SignatureVerifier_OpenSSL;

} // namespace latest

namespace v20180502 {

/**
 * A signature verifier used by the library for checking the cryptographic
 * signatures return by the Cryptolens Web API and thus makes sure
 * the responses from the web API has not been tampered with. This signature
 * verifier can be used with the OpenSSL or LibreSSL libraries.
 *
 * In order for this signature verifier to work the modulus and exponent
 * must be set using the set_modulus_base64() and set_exponent_base64()
 * methods.
 */
using SignatureVerifier_OpenSSL = ::cryptolens_io::internal::SignatureVerifier_v20190401_to_v20180502<::cryptolens_io::v20190401::SignatureVerifier_OpenSSL>;

} // namespace v20180502

} // namespace cryptolens_io
