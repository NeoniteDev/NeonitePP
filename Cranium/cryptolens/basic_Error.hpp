#pragma once

#include <stdlib.h>

#include "api.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

/*
 * Ok means no error has occured
 * All other values indicate error
 */
namespace Subsystem {

int constexpr Ok = 0;
int constexpr Main = 1;
int constexpr Json = 2;
int constexpr Base64 = 3;
int constexpr RequestHandler = 4;
int constexpr SignatureVerifier = 5;

} // namespace Subsystem

/*
 * Which method caused the error
 */
namespace Call {

int constexpr BASIC_SKM_ACTIVATE_RAW = 1;
int constexpr BASIC_CRYPTOLENS_ACTIVATE_RAW = BASIC_SKM_ACTIVATE_RAW;

int constexpr BASIC_SKM_HANDLE_ACTIVATE_RAW = 2;
int constexpr BASIC_CRYPTOLENS_HANDLE_ACTIVATE_RAW = BASIC_SKM_HANDLE_ACTIVATE_RAW;

int constexpr SIGNATURE_VERIFIER_SET_EXPONENT_BASE64 = 3;
int constexpr SIGNATURE_VERIFIER_SET_MODULUS_BASE64 = 4;

int constexpr BASIC_SKM_HANDLE_ACTIVATE = 5;
int constexpr BASIC_CRYPTOLENS_HANDLE_ACTIVATE = BASIC_SKM_HANDLE_ACTIVATE;

int constexpr BASIC_SKM_ACTIVATE = 5;
int constexpr BASIC_CRYPTOLENS_ACTIVATE = BASIC_SKM_ACTIVATE;

int constexpr BASIC_SKM_ACTIVATE_FLOATING = 6;
int constexpr BASIC_CRYPTOLENS_ACTIVATE_FLOATING = BASIC_SKM_ACTIVATE_FLOATING;

int constexpr BASIC_SKM_MAKE_LICENSE_KEY = 7;
int constexpr BASIC_CRYPTOLENS_MAKE_LICENSE_KEY = BASIC_SKM_MAKE_LICENSE_KEY;

int constexpr BASIC_SKM_LAST_MESSAGE = 8;
int constexpr BASIC_CRYPTOLENS_LAST_MESSAGE = BASIC_SKM_LAST_MESSAGE;

int constexpr BASIC_SKM_CREATE_TRIAL_KEY = 9;
int constexpr BASIC_CRYPTOLENS_CREATE_TRIAL_KEY = BASIC_SKM_CREATE_TRIAL_KEY;

} // namespace Call

// Errors for the Main subsystem
namespace Main {

constexpr int UNKNOWN_SERVER_REPLY      = 1;
constexpr int INVALID_ACCESS_TOKEN      = 2;
constexpr int ACCESS_DENIED             = 3;
constexpr int INCORRECT_INPUT_PARAMETER = 4;
constexpr int PRODUCT_NOT_FOUND         = 5;
constexpr int KEY_NOT_FOUND             = 6;
constexpr int KEY_BLOCKED               = 7;
constexpr int DEVICE_LIMIT_REACHED      = 8;
constexpr int KEY_EXPIRED               = 9;

} // namespace Main

} // namespace errors

/**
 * This error is used by all methods in this library that can fail.
 *
 * Several things are provided:
 *   - Chaining multiple calls without having to check for error
 *     after every call.
 *   - Still keep track of which call caused the error, if neccesary.
 *     Maybe add a user version of this tracking too.
 *   - Summary information about the error
 *   - Detailed information for debugging
 */
class basic_Error {
private:
  int    call_;
  int    subsystem_;
  int    reason_;
  size_t extra_;
public:
  basic_Error(): call_(0), subsystem_(errors::Subsystem::Ok), reason_(0), extra_(0) { }
  basic_Error(basic_Error const& e) = delete;
  basic_Error & operator=(basic_Error const& e) = delete;

  /**
   * Explicit conversion to bool allowing the object to be used in e.g.
   * control flow statements.
   *
   * Returns true if an error has occured, false otherwise.
   */
  explicit operator bool() const { return subsystem_ != errors::Subsystem::Ok; }

  virtual int get_subsystem(api::main api) const noexcept { return subsystem_; }
  virtual int get_reason(api::main api) const noexcept { return reason_; }
  virtual size_t get_extra(api::main api) const noexcept { return extra_; }

  /**
   * Can be used to reset the Error object to the initial state.
   *
   * Can be used for reusing the same Error object after an error
   * has occured.
   */
  virtual void reset(api::main api) { subsystem_ = errors::Subsystem::Ok; reason_ = 0; extra_ = 0; }

  virtual void set(api::main api, int subsystem) { subsystem_ = subsystem; }
  virtual void set(api::main api, int subsystem, int reason) { subsystem_ = subsystem; reason_ = reason; }
  virtual void set(api::main api, int subsystem, int reason, size_t extra) { subsystem_ = subsystem; reason_ = reason; extra_ = extra; }
  virtual void set_call(api::main api, int call) { call_ = call; }
};

} // namespace v20190401

namespace v20180502 {

namespace errors {

namespace Subsystem = ::cryptolens_io::v20190401::errors::Subsystem;
namespace Call = ::cryptolens_io::v20190401::errors::Call;
namespace Main = ::cryptolens_io::v20190401::errors::Main;

} // namespace errors

using basic_Error = ::cryptolens_io::v20190401::basic_Error;

} // namespace v20180502

namespace latest {

namespace errors {

namespace Subsystem = ::cryptolens_io::v20190401::errors::Subsystem;
namespace Call = ::cryptolens_io::v20190401::errors::Call;
namespace Main = ::cryptolens_io::v20190401::errors::Main;

} // namespace errors

using basic_Error = ::cryptolens_io::v20190401::basic_Error;

} // namespace latest

} // namespace cryptolens_io
