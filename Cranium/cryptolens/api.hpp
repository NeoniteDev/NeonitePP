#pragma once

namespace cryptolens_io {

namespace v20190401 {

namespace api {

struct main { };

struct experimental_v1 { };

namespace internal {

struct main { };

}  // namespace internal

} // namespace api

} // namespace v20190401

namespace latest {

namespace api {

using ::cryptolens_io::v20190401::api::main;
using ::cryptolens_io::v20190401::api::experimental_v1;

} // namespace api

} // namespace latest

namespace v20180502 {

namespace api {

using ::cryptolens_io::v20190401::api::main;
using ::cryptolens_io::v20190401::api::experimental_v1;

} // namespace api

} // namespace v20180502

} // namespace cryptolens_io
