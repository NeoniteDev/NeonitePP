#pragma once

#include <string>

#include "api.hpp"
#include "basic_Error.hpp"
#include "Error.hpp"

namespace cryptolens_io {

namespace internal {

namespace {

constexpr int INNER_NOT_INITIALIZED = 7623;

} // namespace

template<typename SignatureVerifier>
class SignatureVerifier_v20190401_to_v20180502
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  SignatureVerifier_v20190401_to_v20180502();

  void set_modulus_base64(::cryptolens_io::v20180502::basic_Error & e, std::string const& modulus_base64);
  void set_exponent_base64(::cryptolens_io::v20180502::basic_Error & e, std::string const& exponent_base64);
  bool verify_message(::cryptolens_io::v20180502::basic_Error & e, std::string const& message, std::string const& signature_base64) const;

private:
  ::cryptolens_io::v20190401::Error e_;
  SignatureVerifier verifier_;
};

template<typename SignatureVerifier>
SignatureVerifier_v20190401_to_v20180502<SignatureVerifier>::SignatureVerifier_v20190401_to_v20180502()
: e_(), verifier_(e_)
{}

/**
 * Sets the modulus of the public key used by the cryptolens.io Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on cryptolens.io at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "AbC=" should be passed to this method.
 */
template<typename SignatureVerifier>
void
SignatureVerifier_v20190401_to_v20180502<SignatureVerifier>::set_modulus_base64(::cryptolens_io::v20180502::basic_Error & e, std::string const& modulus_base64)
{
  namespace api = ::cryptolens_io::v20180502::api;
  namespace errors = ::cryptolens_io::v20180502::errors;

  if (e) { return; }
  if (e_) {
    e.set(api::main(), errors::Subsystem::SignatureVerifier, INNER_NOT_INITIALIZED);
  } else {
    verifier_.set_modulus_base64(e, modulus_base64);
  }
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_MODULUS_BASE64); }
}

/**
 * Sets the exponent of the public key used by the cryptolens.io Web API for signing
 * the responses.
 *
 * This value is unique for each account and can be found on cryptolens.io at the
 * "Account Settings" found in the personal menu ("Hello, <account name>!" in the upper
 * right corner). The public key is listed in XML format as something similar to
 *
 *     <RSAKeyValue><Modulus>AbC=</Modulus><Exponent>deFG</Exponent></RSAKeyValue>
 *
 * and in this case the string "deFG" should be passed to this method.
 */
template<typename SignatureVerifier>
void
SignatureVerifier_v20190401_to_v20180502<SignatureVerifier>::set_exponent_base64(::cryptolens_io::v20180502::basic_Error & e, std::string const& exponent_base64)
{
  namespace api = ::cryptolens_io::v20180502::api;
  namespace errors = ::cryptolens_io::v20180502::errors;

  if (e) { return; }
  if (e_) {
    e.set(api::main(), errors::Subsystem::SignatureVerifier, INNER_NOT_INITIALIZED);
  } else {
    verifier_.set_exponent_base64(e, exponent_base64);
  }
  if (e) { e.set_call(api::main(), errors::Call::SIGNATURE_VERIFIER_SET_EXPONENT_BASE64); }
}

/**
 * This function is used internally by the library and need not be called.
 */
template<typename SignatureVerifier>
bool
SignatureVerifier_v20190401_to_v20180502<SignatureVerifier>::verify_message
  ( ::cryptolens_io::v20180502::basic_Error & e
  , std::string const& message
  , std::string const& signature_base64
  )
const
{
  namespace api = ::cryptolens_io::v20180502::api;
  namespace errors = ::cryptolens_io::v20180502::errors;

  if (e) { return false; }
  if (e_) {
    e.set(api::main(), errors::Subsystem::SignatureVerifier, INNER_NOT_INITIALIZED);
    return false;
  }

  return verifier_.verify_message(e, message, signature_base64);
}

} // namespace internal

} // namespace cryptolens_io
