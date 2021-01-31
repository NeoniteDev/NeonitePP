#pragma once

#include <cstring>
#include <string>
#include <sstream>
#include <unordered_map>

#include "imports/std/optional"

#include "imports/ArduinoJson5/ArduinoJson.hpp"

#include "ActivateError.hpp"
#include "basic_Error.hpp"
#include "RawLicenseKey.hpp"
#include "LicenseKey.hpp"
#include "LicenseKeyInformation.hpp"
#include "LicenseKeyChecker.hpp"
#include "api.hpp"

namespace cryptolens_io {

namespace v20180502 {

namespace internal {

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  );

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

  auto x = internal::handle_activate(e, signature_verifier, response);
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

  optional<RawLicenseKey> x = internal::handle_activate(e, signature_verifier, response);
  optional<LicenseKeyInformation> y = LicenseKeyInformation::make(e, x);
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
template<typename RequestHandler, typename SignatureVerifier>
class basic_SKM
{
public:
  basic_SKM() { }

  optional<LicenseKey>
  activate
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<RawLicenseKey>
  activate_raw
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<LicenseKey>
  activate_floating
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , long floating_time_interval
    , int fields_to_return = 0
    );

  RawLicenseKey
  activate_raw_exn
    ( api::experimental_v1 experimental
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<LicenseKey>
  make_license_key(basic_Error & e, std::string const& s);

  SignatureVerifier signature_verifier;
  RequestHandler request_handler;

private:
  optional<RawLicenseKey>
  activate_
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , int fields_to_return = 0
    );

  optional<RawLicenseKey>
  activate_floating_
    ( basic_Error & e
    , std::string token
    , std::string product_id
    , std::string key
    , std::string machine_code
    , long floating_time_interval
    , int fields_to_return = 0
    );
};

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
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename RequestHandler, typename SignatureVerifier>
optional<LicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> x = this->activate_
      ( e
      , std::move(token)
      , std::move(product_id)
      , std::move(key)
      , std::move(machine_code)
      , fields_to_return
      );
  optional<LicenseKeyInformation> y = LicenseKeyInformation::make(e, x);
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
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename RequestHandler, typename SignatureVerifier>
optional<RawLicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_raw
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

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
 * Make a floating Activate request to the SKM Web API
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
 *   machine_code - the machine code, i.e. a string that identifies a device
 *                  for activation.
 *   floating_time_interval - we count machine codes that were created after
 *                            current_time - floating_time_interval, where
 *                            floating_time_interval is given in seconds.
 *
 * Returns:
 *   An optional with a RawLicenseKey representing if the request was
 *   successful or not.
 */
template<typename RequestHandler, typename SignatureVerifier>
optional<LicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_floating
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , long floating_time_interval
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> x = this->activate_floating_
      ( e
      , std::move(token)
      , std::move(product_id)
      , std::move(key)
      , std::move(machine_code)
      , floating_time_interval
      , fields_to_return
      );
  optional<LicenseKeyInformation> y = LicenseKeyInformation::make(e, x);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_ACTIVATE_FLOATING); return nullopt; }
  return LicenseKey(std::move(*y), std::move(*x));
}

template<typename RequestHandler, typename SignatureVerifier>
optional<RawLicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::unordered_map<std::string,std::string> args;
  args["token"] = token;
  args["ProductId"] = product_id;
  args["Key"] = key;
  args["Sign"] = "true";
  args["MachineCode"] = machine_code;
  // Fix since to_string is not available everywhere
  //args["FieldsToReturn"] = std::to_string(fields_to_return);
  std::ostringstream stm; stm << fields_to_return;
  args["FieldsToReturn"] = stm.str();
  args["SignMethod"] = "1";
  args["v"] = "1";

  std::string response = request_handler.make_request(e, "Activate", args);

  return ::cryptolens_io::v20180502::handle_activate_raw(e, this->signature_verifier, response);
}

template<typename RequestHandler, typename SignatureVerifier>
optional<RawLicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::activate_floating_
  ( basic_Error & e
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , long floating_time_interval
  , int fields_to_return
  )
{
  if (e) { return nullopt; }

  std::unordered_map<std::string,std::string> args;
  args["token"] = token;
  args["ProductId"] = product_id;
  args["Key"] = key;
  args["Sign"] = "true";
  args["MachineCode"] = machine_code;
  // Fix since to_string is not available everywhere
  //args["FieldsToReturn"] = std::to_string(fields_to_return);
  std::ostringstream stm; stm << fields_to_return;
  args["FieldsToReturn"] = stm.str();
  args["SignMethod"] = "1";
  args["v"] = "1";
  stm.str(""); stm.clear(); stm << floating_time_interval;
  args["FloatingTimeInterval"] = stm.str();

  std::string response = request_handler.make_request(e, "Activate", args);

  return ::cryptolens_io::v20180502::handle_activate_raw(e, this->signature_verifier, response);
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
template<typename RequestHandler, typename SignatureVerifier>
RawLicenseKey
basic_SKM<RequestHandler, SignatureVerifier>::activate_raw_exn
  ( api::experimental_v1 experimental
  , std::string token
  , std::string product_id
  , std::string key
  , std::string machine_code
  , int fields_to_return
  )
{
  basic_Error e;
  optional<RawLicenseKey> raw_license_key =
    activate_raw( e, std::move(token), std::move(product_id), std::move(key)
            , std::move(machine_code), fields_to_return);

  if (!e) { return *raw_license_key; }
  throw ActivateError::from_server_response(NULL);
}

template<typename RequestHandler, typename SignatureVerifier>
optional<LicenseKey>
basic_SKM<RequestHandler, SignatureVerifier>::make_license_key(basic_Error & e, std::string const& s)
{
  if (e) { return nullopt; }

  optional<RawLicenseKey> raw_license_key;

  raw_license_key =
    ::cryptolens_io::v20180502::internal::handle_activate(e, this->signature_verifier, s);

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

  optional<LicenseKeyInformation> license_key_information = LicenseKeyInformation::make(e, raw_license_key);
  if (e) { e.set_call(api::main(), errors::Call::BASIC_SKM_MAKE_LICENSE_KEY); return nullopt; }
  return LicenseKey(std::move(*license_key_information), std::move(*raw_license_key));
}


namespace internal {

int
activate_parse_server_error_message(char const* server_response);

template<typename SignatureVerifier>
optional<RawLicenseKey>
handle_activate
  ( basic_Error & e
  , SignatureVerifier const& signature_verifier
  , std::string const& response
  )
{
  if (e) { return nullopt; }

  using namespace errors;
  api::main api;

  using namespace ArduinoJson;
  DynamicJsonBuffer jsonBuffer;
  JsonObject & j = jsonBuffer.parseObject(response);

  if (!j.success()) { e.set(api, Subsystem::Json); return nullopt; }

  if (!j["result"].is<int>() || j["result"].as<int>() != 0) {
    if (!j["message"].is<const char*>() || j["message"].as<char const*>() == NULL) {
      e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
      return nullopt;
    }

    int reason = activate_parse_server_error_message(j["message"].as<char const*>());
    e.set(api, Subsystem::Main, reason);
    return nullopt;
  }

  if (!j["licenseKey"].is<char const*>() || j["licenseKey"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  if (!j["signature"].is<char const*>() || j["signature"].as<char const*>() == NULL) {
    e.set(api, Subsystem::Main, Main::UNKNOWN_SERVER_REPLY);
    return nullopt;
  }

  return RawLicenseKey::make
           ( e
           , signature_verifier
           , j["licenseKey"].as<char const*>()
           , j["signature"].as<char const*>()
	   );
}

} // namespace internal

} // namespace v20180502

} // namespace cryptolens_io
