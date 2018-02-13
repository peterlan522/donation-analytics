#ifndef DONATION_ANALYTICS_MASTER_VALIDATE_DATE_H_
#define DONATION_ANALYTICS_MASTER_VALIDATE_DATE_H_

class ValidateDate {
 public:
  ValidateDate(const int valid_length);
  ~ValidateDate();

  bool isValidDate(std::string date);

 private:
  bool isLeap(int year);
  const int kMaxValidYear = 9999;
  const int kMinValidYear = 1800;
  int valid_length_;
};

#endif //DONATION_ANALYTICS_MASTER_VALIDATE_DATE_H_

ValidateDate::ValidateDate(const int valid_length) {
  valid_length_ = valid_length;
}

ValidateDate::~ValidateDate() {}

bool ValidateDate::isLeap(int year) {
  return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
}

bool ValidateDate::isValidDate(std::string date) {
  if (date.size() != valid_length_) {
    return false;
  }

  int month = stoi(date.substr(0, 2));
  int day = stoi(date.substr(2, 2));
  int year = stoi(date.substr(4, 4));

  if (year > kMaxValidYear || year < kMinValidYear)
    return false;
  if (month < 1 || month > 12)
    return false;
  if (day < 1 || day > 31)
    return false;

  // Handle February month with leap year
  if (month == 2) {
    if (isLeap(year))
      return (day <= 29);
    else
      return (day <= 28);
  }

  if (month == 4 || month == 6 || month == 9 || month == 11)
    return (day <= 30);

  return true;
}