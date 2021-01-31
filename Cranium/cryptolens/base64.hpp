#pragma once

#include <string>

#include "imports/std/optional"

namespace cryptolens_io {

namespace v20190401  {

namespace internal {

// Internal functions used by the library for dealing with messages
// encoded with base64.

int
b64_pton(char const *src, unsigned char *target, size_t targsize);

optional<std::string>
b64_decode(std::string const& b64);

} // namespace internal

} // namespace v20190401

} // namespace cryptolens_io
