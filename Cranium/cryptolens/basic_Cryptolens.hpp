#pragma once

#include <cstring>
#include <string>
#include <sstream>

#include "imports/std/optional"

#include "ActivateError.hpp"
#include "api.hpp"
#include "basic_Error.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyChecker.hpp"
#include "LicenseKeyInformation.hpp"
#include "RawLicenseKey.hpp"
#include "ResponseParser_ArduinoJson5.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace internal {

template<typename ResponseParser, typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , ResponseParser const& response_parser
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  );

int
activate_parse_server_error_message(char const* server_response);

class ActivateEnvironment {
public:
  ActivateEnvironment
    ( LicenseKeyInformation const& license_key_information
    , int product_id
    , std::string const& key
    , std::string const& machine_code
    , int fields_to_return
    , bool floating
    )
  : license_key_information_(&license_key_information)
  , product_id_(product_id), key_(&key), machine_code_(&machine_code)
  , fields_to_return_(fields_to_return), floating_(floating)
  {}

  LicenseKeyInformation const&
  get_license_key_information() const {
    return *license_key_information_;
  }

  int
  get_product_id() const
  {
    return product_id_;
  }

  std::string const&
  get_key() const
  {
    return *key_;
  }

  std::string const&
  get_machine_code() const
  {
    return *machine_code_;
  }

  int
  get_fields_to_return() const
  {
    return fields_to_return_;
  }

  bool
  get_floating() const
  {
    return floating_;
  }

private:
  LicenseKeyInformation const* license_key_information_;
  int product_id_;
  std::string const* key_;
  std::string const* machine_code_;
  int fields_to_return_;
  bool floating_;
};

} // namespace internal

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate_raw
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  ResponseParser_ArduinoJson5 response_parser(e);
  if (e) { return nullopt; }

  auto x = internal::handle_activate(e, response_parser, signature_verifier, response);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_HANDLE_ACTIVATE_RAW); }
  return x;
}

template<typename SignatureVerifier>
RawLicenseKey
handle_activate_raw_exn
  ( api::experimental_v1 experimental
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  basic_Error e;
  optional<RawLicenseKey> raw_license_key =
    handle_activate(e, signature_verifier, response);

  if (raw_license_key && !e) { return *raw_license_key; }

  throw ActivateError::from_server_response(NULL);
}

template<typename SignatureVerifier>
optional<LicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  ResponseParser_ArduinoJson5 response_parser(e);
  if (e) { return nullopt; }

  optional<RawLicenseKey> x = internal::handle_activate(e, response_parser, signature_verifier, response);
  optional<LicenseKeyInformation> y = response_parser.make_license_key_information(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_HANDLE_ACTIVATE); }

  return LicenseKey(std::move(*y), std::move(*x));
}

/**
 * This class makes it possible to interact with the Cryptolens Web API. Among the
 * various methods available in the Web API the only ones currently supported
 * in the C++ API are Activate and Deactivate.
 *
 * This class uses two policy classes, SignatureVerifier and RequestHandler,
 * which are responsible for handling verification of signatures and making
 * requests to the Web API, respectivly. Consult the documentation for the
 * chosen policy classes since in some cases special initialization may be
 * neccessary.
 */
template<typename Configuration>
class basic_Cryptolens
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  basic_Cryptolens(basic_Error & e)
  : response_parser(e), request_handler(e), signature_verifier(e), machine_code_computer(e)
  , activate_validator(e)
  { }

  optional<LicenseKey>
  activate
    ( basic_Error & e
    , std::string token
    , int product_id
    , std::string key
    , int fields_to_return = 0
    );

  optional<RawLicenseKey>
  activate_raw
    ( basic_Error & e
    , std::string token
    , int product_id
    , std::string key
    , int fields_to_return = 0
    );

  optional<LicenseKey>
  activate_floating
    ( basic_Error & e
    , std::string token
    , int product_id
    , std::string key
    , long floating_time_interval
    , int fields_to_return = 0
    );

  RawLicenseKey
  activate_raw_exn
    ( api::experimental_v1 experimental
    , std::string token
    , int product_id
    , std::string key
    , int fields_to_return = 0
    );

  std::string
  create_trial_key
    ( basic_Error & e
    , std::string token
    , int product_id
    );

  std::string
  last_message
    ( basic_Error & e
    , std::string token
    , std::string channel
    , int since_unix_timestamp
    );

  optional<LicenseKey>
  make_license_key(basic_Error & e, std::string const& s);

  typename Configuration::ResponseParser response_parser;
  typename Configuration::RequestHandler request_handler;
  typename Configuration::SignatureVerifier signature_verifier;
  typename Configuration::MachineCodeComputer machine_code_computer;
  typename Configuration::template ActivateValidator<internal::ActivateEnvironment> activate_validator;

private:
  optional<RawLicenseKey>
  activate_
    ( basic_Error & e
    , std::string token
    , int product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<RawLicenseKey>
  activate_floating_
    ( basic_Error & e
    , std::string token
    , int product_id
    , std::string key
    , std::string machine_code
    , long floating_time_interval
    , int fields_to_return = 0
    );

  std::string
  create_trial_key_
    ( basic_Error & e
    , std::string token
    , int product_id
    );

  std::string
  last_message_
    ( basic_Error & e
    , std::string token
    , std::string channel
    , int since_unix_timestamp
    );
};

/**
 * Make an Activate request to the Cryptolens Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename Configuration>
optional<LicenseKey>
basic_Cryptolens<Configuration>::activate
  ( basic_Error & e
  , std::string token
  , int product_id
  , std::string key
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::string machine_code = machine_code_computer.get_machine_code(e);

  optional<RawLicenseKey> x = this->activate_
      ( e
      , std::move(token)
      , product_id
      , key // NOTE: Copy is performed here
      , machine_code // NOTE: Copy is performed here
      , fields_to_return
      );
  optional<LicenseKeyInformation> y = response_parser.make_license_key_information(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE); return nullopt; }

  typename internal::ActivateEnvironment env(*y, product_id, key, machine_code, fields_to_return, false);
  activate_validator.validate(e, env);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE); return nullopt; }

  return LicenseKey(std::move(*y), std::move(*x));
}

/**
 * Make an Activate request to the Cryptolens Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename Configuration>
optional<RawLicenseKey>
basic_Cryptolens<Configuration>::activate_raw
  ( basic_Error & e
  , std::string token
  , int product_id
  , std::string key
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::string machine_code = machine_code_computer.get_machine_code(e);

  auto x = this->activate_( e
                          , std::move(token)
                          , std::move(product_id)
                          , std::move(key)
                          , std::move(machine_code)
                          , fields_to_return
                          );
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE_RAW); }
  return x;
}

/**
 * Make a floating Activate request to the Cryptolens Web API
 *
 * A standard and a floating activate request differs in how the
 * number of machine codes for the license is computed. The standard request
 * computes the number of machine codes in the entire history. The floating
 * request instead just computes the machine codes that have been activated
 * recently, where the length of the interval is given by the
 * floating_time_interval.
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *   floating_time_interval - we count machine codes that were created after
 *                            current_time - floating_time_interval, where
 *                            floating_time_interval is given in seconds.
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename Configuration>
optional<LicenseKey>
basic_Cryptolens<Configuration>::activate_floating
  ( basic_Error & e
  , std::string token
  , int product_id
  , std::string key
  , long floating_time_interval
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::string machine_code = machine_code_computer.get_machine_code(e);

  optional<RawLicenseKey> x = this->activate_floating_
      ( e
      , std::move(token)
      , std::move(product_id)
      , key // NOTE: Copy is performed here
      , machine_code // NOTE: Copy is performed here
      , floating_time_interval
      , fields_to_return
      );
  optional<LicenseKeyInformation> y = response_parser.make_license_key_information(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE_FLOATING); return nullopt; }

  typename internal::ActivateEnvironment env(*y, product_id, key, machine_code, fields_to_return, true);
  activate_validator.validate(e, env);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE_FLOATING); return nullopt; }

  return LicenseKey(std::move(*y), std::move(*x));
}

template<typename Configuration>
std::string
basic_Cryptolens<Configuration>::create_trial_key
  ( basic_Error & e
  , std::string token
  , int product_id
  )
{
  if (e) { return ""; }

  std::string key = create_trial_key_(e, token, product_id);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_CREATE_TRIAL_KEY); return ""; }
  return key;
}

template<typename Configuration>
optional<RawLicenseKey>
basic_Cryptolens<Configuration>::activate_
  ( basic_Error & e
  , std::string token
  , int product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  auto request = request_handler.post_request(e, "app.cryptolens.io", "/api/key/Activate");

  std::ostringstream product_id_; product_id_ << product_id;
  std::ostringstream fields_to_return_; fields_to_return_ << fields_to_return;

  std::string response =
    request.add_argument(e, "token"         , token.c_str())
           .add_argument(e, "ProductId"     , product_id_.str().c_str())
           .add_argument(e, "Key"           , key.c_str())
           .add_argument(e, "Sign"          , "true")
           .add_argument(e, "MachineCode"   , machine_code.c_str())
           .add_argument(e, "FieldsToReturn", fields_to_return_.str().c_str())
           .add_argument(e, "SignMethod"    , "1")
           .add_argument(e, "v"             , "1")
           .make(e);

  return handle_activate_raw(e, this->signature_verifier, response);
}

template<typename Configuration>
optional<RawLicenseKey>
basic_Cryptolens<Configuration>::activate_floating_
  ( basic_Error & e
  , std::string token
  , int product_id
  , std::string key
  , std::string machine_code
  , long floating_time_interval
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  auto request = request_handler.post_request(e, "app.cryptolens.io", "/api/key/Activate");

  std::ostringstream product_id_; product_id_ << product_id;
  std::ostringstream fields_to_return_; fields_to_return_ << fields_to_return;
  std::ostringstream floating_time_interval_; floating_time_interval_ << floating_time_interval;

  std::string response =
    request.add_argument(e, "token"               , token.c_str())
           .add_argument(e, "ProductId"           , product_id_.str().c_str())
           .add_argument(e, "Key"                 , key.c_str())
           .add_argument(e, "Sign"                , "true")
           .add_argument(e, "MachineCode"         , machine_code.c_str())
           .add_argument(e, "FieldsToReturn"      , fields_to_return_.str().c_str())
           .add_argument(e, "SignMethod"          , "1")
           .add_argument(e, "v"                   , "1")
           .add_argument(e, "FloatingTimeInterval", floating_time_interval_.str().c_str())
           .make(e);

  return handle_activate_raw(e, this->signature_verifier, response);
}

/**
 * Make an Activate request to the Cryptolens Web API
 *
 * Arguments:
 *   token - acces token to use
 *   product_id - the product id
 *   key - the serial key string, e.g. ABCDE-EFGHI-JKLMO-PQRST
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *
 * Returns:
 *   A RawLicenseKey. If the request is unsuecessful an ActivateError is thrown.
 */
template<typename Configuration>
RawLicenseKey
basic_Cryptolens<Configuration>::activate_raw_exn
  ( api::experimental_v1 experimental
  , std::string token
  , int product_id
  , std::string key
  , int fields_to_return
  )
{
  basic_Error e;

  std::string machine_code = machine_code_computer.get_machine_code(e);

  optional<RawLicenseKey> raw_license_key =
    activate_raw( e, std::move(token), std::move(product_id), std::move(key)
            , std::move(machine_code), fields_to_return);

  if (!e) { return *raw_license_key; }
  throw ActivateError::from_server_response(NULL);
}

template<typename Configuration>
std::string
basic_Cryptolens<Configuration>::last_message
  ( basic_Error & e
  , std::string token
  , std::string channel
  , int since_unix_timestamp
  )
{
  if (e) { return ""; }

  std::string message = last_message_(e, token, channel, since_unix_timestamp);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_LAST_MESSAGE); return ""; }
  return message;
}

template<typename Configuration>
std::string
basic_Cryptolens<Configuration>::create_trial_key_
  ( basic_Error & e
  , std::string token
  , int product_id
  )
{
  if (e) { return ""; }

  std::string machine_code = machine_code_computer.get_machine_code(e);

  auto request = request_handler.post_request(e, "app.cryptolens.io", "/api/Key/CreateTrialKey");

  std::ostringstream product_id_; product_id_ << product_id;

  std::string response =
    request.add_argument(e, "token"      , token.c_str())
           .add_argument(e, "ProductId"  , product_id_.str().c_str())
           .add_argument(e, "MachineCode", machine_code.c_str())
           .make(e);

  if (e) { return ""; }

  return response_parser.parse_create_trial_key_response(e, response);
}

template<typename Configuration>
std::string
basic_Cryptolens<Configuration>::last_message_
  ( basic_Error & e
  , std::string token
  , std::string channel
  , int since_unix_timestamp
  )
{
  if (e) { return ""; }

  auto request = request_handler.post_request(e, "app.cryptolens.io", "/api/message/GetMessages");

  std::ostringstream stm; stm << since_unix_timestamp;

  std::string response =
    request.add_argument(e, "token"  , token.c_str())
           .add_argument(e, "Channel", channel.c_str())
           .add_argument(e, "Time"   , stm.str().c_str())
           .make(e);

  if (e) { return ""; }

  return response_parser.parse_last_message_response(e, response);
}

template<typename Configuration>
optional<LicenseKey>
basic_Cryptolens<Configuration>::make_license_key(basic_Error & e, std::string const& s)
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> raw_license_key;

  raw_license_key =
    ::cryptolens_io::v20190401::internal::handle_activate(e, this->response_parser, this->signature_verifier, s);

  if (e) {
    e.reset(api::main());

    size_t k = s.find('-');
    if (k == std::string::npos) { e.set(api::main(), errors::Subsystem::Main, errors::Main::UNKNOWN_SERVER_REPLY); return nullopt; }

    std::string version = s.substr(0, k);
    std::string rem = s.substr(k+1, std::string::npos);
    // NOTE: s.substr(s.size(), _) returns empty string, thus the previous line does never throw

    k = rem.find('-');
    if (k == std::string::npos) { e.set(api::main(), errors::Subsystem::Main, errors::Main::UNKNOWN_SERVER_REPLY); return nullopt; }

    std::string license = rem.substr(0, k);
    std::string signature = rem.substr(k+1, std::string::npos); // k+1 is fine, see comment above

    raw_license_key =
        RawLicenseKey::make
             ( e
             , signature_verifier
             , license
             , signature
             );
  }

  optional<LicenseKeyInformation> license_key_information = response_parser.make_license_key_information(e, raw_license_key);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_MAKE_LICENSE_KEY); return nullopt; }
  return LicenseKey(std::move(*license_key_information), std::move(*raw_license_key));
}

namespace internal {

template<typename ResponseParser, typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , ResponseParser const& response_parser
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  optional<std::pair<std::string, std::string>> x = response_parser.parse_activate_response(e, response);
  if (e) { return nullopt; }

  return RawLicenseKey::make
           ( e
           , signature_verifier
           , x->first
           , x->second
           );
}

} // namespace internal

} // namespace v20190401

namespace latest {

template<typename Configuration>
using basic_Cryptolens = ::cryptolens_io::v20190401::basic_Cryptolens<Configuration>;

} // namespace latest

} // namespace cryptolens_io
