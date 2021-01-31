#pragma once

#include <string>

#include "imports/curl/curl.h"

#include "basic_Error.hpp"
#include "RequestHandler_v20190401_to_v20180502.hpp"

namespace cryptolens_io {

namespace v20190401 {

namespace errors {

namespace RequestHandler_curl {

int constexpr CURL_NULL = 1;
int constexpr ESCAPE = 2;
int constexpr SETOPT_URL = 3;
int constexpr SETOPT_WRITEFUNCTION = 4;
int constexpr SETOPT_WRITEDATA = 5;
int constexpr SETOPT_VERIFYPEER = 6;
int constexpr SETOPT_VERIFYHOST = 7;
int constexpr PERFORM = 8;
int constexpr SETOPT_POSTFIELDS = 9;

} // namespace RequestHandler_curl

} // namespace errors

class RequestHandler_curl_PostBuilder {
public:
  RequestHandler_curl_PostBuilder(CURL * curl, char const* host, char const* endpoint);

  RequestHandler_curl_PostBuilder &
  add_argument(basic_Error & e, char const* key, char const* value);

  std::string
  make(basic_Error & e);

private:
  CURL *curl_;
  char separator_;
  std::string postfields_;
  std::string url_;
};

/**
 * A request handler that is responsible for making the HTTPS requests
 * to the Cryptolens Web API. This request handler is build
 * around the Curl library, which is responsible for making the
 * actual HTTPS request.
 *
 * No particular initialization is needed in order to use this
 * RequestHandler.
 */
class RequestHandler_curl
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  RequestHandler_curl(basic_Error & e);
#ifndef CRYPTOLENS_ENABLE_DANGEROUS_COPY_MOVE_CONSTRUCTOR
  RequestHandler_curl(RequestHandler_curl const&) = delete;
  RequestHandler_curl(RequestHandler_curl &&) = delete;
  void operator=(RequestHandler_curl const&) = delete;
  void operator=(RequestHandler_curl &&) = delete;
#endif
  ~RequestHandler_curl();

  using PostBuilder = RequestHandler_curl_PostBuilder;

  PostBuilder
  post_request(basic_Error & e, char const* host, char const* endpoint);
private:
  CURL *curl;
};

} // namespace v20190401

namespace latest {

namespace errors {

namespace RequestHandler_curl = ::cryptolens_io::v20190401::errors::RequestHandler_curl;

} // namespace errors

using RequestHandler_curl = ::cryptolens_io::v20190401::RequestHandler_curl;

} // namespace latest

namespace v20180502 {

namespace errors {

namespace RequestHandler_curl {

int constexpr CURL_NULL = 1;
int constexpr ESCAPE = 2;
int constexpr SETOPT_URL = 3;
int constexpr SETOPT_WRITEFUNCTION = 4;
int constexpr SETOPT_WRITEDATA = 5;
int constexpr SETOPT_VERIFYPEER = 6;
int constexpr SETOPT_VERIFYHOST = 7;
int constexpr PERFORM = 8;
int constexpr SETOPT_POSTFIELDS = 9;

} // namespace RequestHandler_curl

} // namespace errors

using RequestHandler_curl = ::cryptolens_io::internal::RequestHandler_v20190401_to_v20180502<::cryptolens_io::v20190401::RequestHandler_curl>;

} // namespace v20180502

} // namespace cryptolens_io
