/*
 * Class: RecipientStatistics
 * --------------------------
 * Store recipient primary key and his donation
 * statistics.
 */

#ifndef DONATION_ANALYTICS_MASTER_RECIPIENT_STATISTICS_H_
#define DONATION_ANALYTICS_MASTER_RECIPIENT_STATISTICS_H_

#include "statistics.h"
#include <unordered_map>

class RecipientStatistics {
 public:
  RecipientStatistics(const int percentile);
  ~RecipientStatistics();

  void AddData(const std::string &recipient, const std::string zip_code,
               int year, int amount);
  std::string GetStatistics() const;

 private:
  int percentile_;
  std::string result_;
  int total_donation_amount_ = 0;
  int total_donation_number_ = 0;
  std::unordered_map<std::string, Statistics> recipients_;
};

#endif //DONATION_ANALYTICS_MASTER_RECIPIENT_STATISTICS_H_

RecipientStatistics::RecipientStatistics(const int percentile) {
  percentile_ = percentile;
}

RecipientStatistics::~RecipientStatistics() {}

void RecipientStatistics::AddData(const std::string &recipient,
                                  const std::string zip_code, int year,
                                  int amount) {
  if (!recipient.empty() && !zip_code.empty() && year >= 2015 && amount >= 0) {
    std::string recipient_pk =
      recipient + "|" + zip_code + "|" + std::to_string(year);
    result_ = recipient_pk + "|";
    std::unordered_map<std::string, Statistics>::iterator it =
      recipients_.find(recipient_pk);
    if (it != recipients_.end()) {
      (it->second).AddValue(amount);
      result_ += (it->second).GetAllStatistics();
    } else {
      Statistics s;
      s.SetPercentile(percentile_);
      s.AddValue(amount);
      recipients_.insert({recipient_pk, s});
      result_ += s.GetAllStatistics();
    }
  }
}

std::string RecipientStatistics::GetStatistics() const { return result_; }