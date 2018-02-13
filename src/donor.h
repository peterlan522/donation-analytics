/*
 * Class: Donor
 * ------------
 * Store donors' primary key and check
 * if this is a repeated donor.
 */

#ifndef DONATION_ANALYTICS_MASTER_DONOR_H_
#define DONATION_ANALYTICS_MASTER_DONOR_H_

#include <functional>
#include <string>
#include <unordered_map>
#include <utility>

class Donor {
 public:
  Donor();
  ~Donor();

  bool isRepeatedDonor(std::string donor_name, std::string zip_code, int year);

 private:
  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2> &p) const {
      auto h1 = std::hash<T1> {}(p.first);
      auto h2 = std::hash<T2> {}(p.second);
      return h1 ^ h2;
    }
  };
  std::unordered_map<std::pair<std::string, std::string>, int, pair_hash>
  doner_;
};

#endif //DONATION_ANALYTICS_MASTER_DONOR_H_

Donor::Donor() {}

Donor::~Donor() {}

bool Donor::isRepeatedDonor(std::string donor_name, std::string zip_code,
                            int year) {
  std::pair<std::string, std::string> donor;
  donor = std::make_pair(donor_name, zip_code);
  std::unordered_map<std::pair<std::string, std::string>, int,
      pair_hash>::iterator got;
  got = doner_.find(donor);
  if (got == doner_.end()) {
    doner_.insert({donor, year});
    return false;
  } else {
    if (got->second < year) {
      return true;
    } else if (got->second > year) {
      got->second = year;
      return false;
    }
  }
}
