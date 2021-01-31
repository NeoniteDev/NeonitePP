#pragma once

#include "api.hpp"
#include "basic_Error.hpp"
#include "Error.hpp"

namespace cryptolens_io {

namespace internal {

template<typename RequestHandler>
class RequestHandler_v20190401_to_v20180502
{
public:
#ifndef CRYPTOLENS_20190701_ALLOW_IMPLICIT_CONSTRUCTORS
  explicit
#endif
  RequestHandler_v20190401_to_v20180502();

  template<typename Map>
  std::string
  make_request(::cryptolens_io::v20180502::basic_Error & e, char const* method, Map const& map);

private:
  ::cryptolens_io::v20190401::Error e_;
  RequestHandler inner_;
};

template<typename RequestHandler>
RequestHandler_v20190401_to_v20180502<RequestHandler>::RequestHandler_v20190401_to_v20180502()
: e_(), inner_(e_)
{}

template<typename RequestHandler>
template<typename Map>
std::string
RequestHandler_v20190401_to_v20180502<RequestHandler>::make_request(::cryptolens_io::v20180502::basic_Error & e, char const* method, Map const& map)
{
  namespace api = ::cryptolens_io::v20180502::api;
  namespace errors = ::cryptolens_io::v20180502::errors;

  if (e) { return ""; }
  if (e_) {
    e.set(api::main(), errors::Subsystem::RequestHandler);  // TODO: Add more detailed error code
    return "";
  }

  std::string endpoint("/api/key/");
  endpoint += method;

  auto request = inner_.post_request(e, "app.cryptolens.io", endpoint.c_str());

  for (auto x : map) { request.add_argument(e, x.first.c_str(), x.second.c_str()); }

  return request.make(e);
}

} // namespace internal

} // namespace cryptolens_io
