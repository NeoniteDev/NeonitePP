#pragma once

#include <string>

namespace cryptolens_io {

namespace v20190401 {

// An immutable class representing an activated machine
// for a given serial key
class ActivationData {
private:
  std::string mid_;
  std::string ip_;
  std::uint64_t time_;
public:
  ActivationData
    ( std::string mid
    , std::string ip
    , std::uint64_t time
    )
  : mid_(std::move(mid))
  , ip_(std::move(ip))
  , time_(time)
  { }

  // Returns the machine id
  std::string const& get_mid() const { return mid_; }

  // Returns the IP when the machine was activated the first time
  std::string const& get_ip() const { return ip_; }

  // Returns the time the machine was activated the first time
  std::uint64_t get_time() const { return time_; }
};

} // namespace v20190401

namespace v20180502 {

using ::cryptolens_io::v20190401::ActivationData;

} // namespace v20180502

namespace latest {

using ::cryptolens_io::v20190401::ActivationData;

} // namespace latest

} // namespace cryptolens_io
