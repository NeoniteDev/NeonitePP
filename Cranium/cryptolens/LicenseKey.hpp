#pragma once

#include <string>
#include <vector>

#include "imports/std/optional"

#include "basic_Error.hpp"
#include "ActivationData.hpp"
#include "Customer.hpp"
#include "DataObject.hpp"
#include "LicenseKeyInformation.hpp"
#include "RawLicenseKey.hpp"

namespace cryptolens_io {

namespace v20190401 {

class LicenseKeyChecker;

/**
 * This immutable class represents a license key.
 *
 * The class is constructed using the static factory make().
 *
 * The check() method can be used to create a LicenseKeyChecker,
 * which can be used to check properties of a license key in a
 * convenient fashion. I.e.
 *
 *     if (key.check().has_feature(1).has_not_feature(2)) {
 *       DO_SOMETHING();
 *     }
 */
class LicenseKey {
private:
  LicenseKeyInformation info_;
  RawLicenseKey raw_;
public:
  LicenseKey(LicenseKeyInformation && license_key_information, RawLicenseKey && raw_license_key);

  // TODO: Add factory taking r-value references?
  //       This would mostly be used internally in the library since in most cases
  //       the handle class will be responsible for constructing the object.
  //static optional<LicenseKey> make(basic_Error & e, RawLicenseKey const& raw_license_key);
  //static optional<LicenseKey> make(basic_Error & e, optional<RawLicenseKey> const& raw_license_key);
  //static optional<LicenseKey> make_unsafe(basic_Error & e, std::string const& license_key);

  std::string to_string() const;

  LicenseKeyInformation & get_license_key_information() { return info_; }
  LicenseKeyInformation const& get_license_key_information() const { return info_; }

  LicenseKeyChecker check() const;

  int           get_product_id() const;
  std::uint64_t get_created() const;
  std::uint64_t get_expires() const;
  int           get_period() const;
  bool          get_block() const;
  bool          get_trial_activation() const;
  std::uint64_t get_sign_date() const;
  bool          get_f1() const;
  bool          get_f2() const;
  bool          get_f3() const;
  bool          get_f4() const;
  bool          get_f5() const;
  bool          get_f6() const;
  bool          get_f7() const;
  bool          get_f8() const;

  optional<int>                         const& get_id() const;
  optional<std::string>                 const& get_key() const;
  optional<std::string>                 const& get_notes() const;
  optional<int>                         const& get_global_id() const;
  optional<Customer>                    const& get_customer() const;
  optional<std::vector<ActivationData>> const& get_activated_machines() const;
  optional<int>                         const& get_maxnoofmachines() const;
  optional<std::string>                 const& get_allowed_machines() const;
  optional<std::vector<DataObject>>     const& get_data_objects() const;
};

} // namespace v20190401

namespace v20180502 {

using LicenseKey = ::cryptolens_io::v20190401::LicenseKey;

} // namespace v20180502

namespace latest {

using LicenseKey = ::cryptolens_io::v20190401::LicenseKey;

} // namespace latest

} // namespace cryptolens_io
