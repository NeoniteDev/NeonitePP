#pragma once

#include <string>

namespace cryptolens_io {

namespace v20190401 {

// An immutable class representing an Cryptolens Data Object
class DataObject {
private:
  int id_;
  std::string name_;
  std::string string_value_;
  int int_value_;
public:
  DataObject
    ( int id
    , std::string name
    , std::string string_value
    , int int_value
    )
  : id_(id)
  , name_(std::move(name))
  , string_value_(std::move(string_value))
  , int_value_(int_value)
  {
    // TODO: Check requirements on max length of name and string_value
    //       (Not really neccessary when we are just parsing requests from
    //        the server, though)
  }

  // Returns the Id of the data object
  int                get_id() const;

  // Returns the name of the data object
  std::string const& get_name() const;

  // Returns the string value of the data object
  std::string const& get_string_value() const;

  // Returns the integer value of the data object
  int                get_int_value() const;
};

} // namespace v20190401

namespace v20180502 {

using DataObject = ::cryptolens_io::v20190401::DataObject;

} // namespace v20180502

namespace latest {

using DataObject = ::cryptolens_io::v20190401::DataObject;

} // namespace latest

} // namespace cryptolens_io
