/*
 * Class: Statistics
 * -----------------
 * Store a sequence of numbers, then compute and
 * return total amount, total number and running
 * percentile of the data.
 */

#ifndef DONATION_ANALYTICS_MASTER_STATISTICS_H_
#define DONATION_ANALYTICS_MASTER_STATISTICS_H_

#include <algorithm>
#include <cmath>
#include <set>
#include <string>

class Statistics {
 public:
  Statistics();
  ~Statistics();

  void AddValue(const int value);
  void SetPercentile(const int percentile);

  int GetTotalAmount() const;
  int GetTotalNumber() const;
  int GetPercentile() const;
  std::string GetAllStatistics() const;

 private:
  int ComputePercentile(const int value);
  double percentile_;
  int percentile_value_ = 0;
  int total_amount_ = 0;
  int total_number_ = 0;
  std::multiset<int> values_;
};

#endif //DONATION_ANALYTICS_MASTER_STATISTICS_H_

Statistics::Statistics() {}

Statistics::~Statistics() {}

void Statistics::SetPercentile(const int percentile) {
  percentile_ = (percentile / 100.0);
}

void Statistics::AddValue(const int value) {
  total_amount_ += value;
  total_number_++;
  values_.insert(value);
  percentile_value_ = ComputePercentile(value);
}

int Statistics::GetTotalAmount() const { return total_amount_; }

int Statistics::GetTotalNumber() const { return total_number_; }

int Statistics::GetPercentile() const { return percentile_value_; }

std::string Statistics::GetAllStatistics() const {
  return std::to_string(percentile_value_) + "|" +
         std::to_string(total_amount_) + "|" + std::to_string(total_number_);
}

int Statistics::ComputePercentile(const int value) {
  int set_size = values_.size();
  int pos = std::ceil(percentile_ * set_size) - 1;
  if (set_size > pos) {
    std::set<int>::iterator it = std::next(values_.begin(), pos);
    return std::round(*it);
  }
}