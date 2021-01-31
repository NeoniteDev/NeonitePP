#pragma once

namespace cryptolens_io {

namespace v20190401 {

// This immutable class represents a customer
class Customer {
private:
  int id_;
  std::string name_;
  std::string email_;
  std::string company_name_;
  std::uint64_t created_;
public:
  Customer
    ( int id
    , std::string name
    , std::string email
    , std::string company_name
    , std::uint64_t created
    )
  : id_(id)
  , name_(std::move(name))
  , email_(std::move(email))
  , company_name_(std::move(company_name))
  , created_(created)
  {
    // TODO: Check length requirements (does not matter when we are just reading things from the web api)
  }

  // Returns the customer id
  int                get_id() const { return id_; }

  // Returns the customer name
  std::string const& get_name() const { return name_; }

  // Returns the customer's email
  std::string const& get_email() const { return email_; }

  // Returns the customer's company name
  std::string const& get_company_name() const { return company_name_; }

  // Returns when the customer's account was created
  std::uint64_t get_created() const { return created_; }
};

} // namespace v20190401

namespace v20180502 {

using Customer = ::cryptolens_io::v20190401::Customer;

} // namespace v20180502

namespace latest {

using Customer = ::cryptolens_io::v20190401::Customer;

} // namespace latest

} // namespace cryptolens_io
