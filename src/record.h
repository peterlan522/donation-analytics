#ifndef DONATION_ANALYTICS_MASTER_RECORD_H_
#define DONATION_ANALYTICS_MASTER_RECORD_H_

#include <ostream>
#include <string>

class Record {
 public:
  Record(std::vector<std::string> &record);
  ~Record();

  void SetRecipientID(const std::string &recipient_id);
  void SetDonorName(const std::string &donor_name);
  void SetDonorZipCode(const std::string &zip_code);
  void SetTransactionYear(const std::string &transaction_date);
  void SetTransactionAmount(const std::string &transaction_amount);

  std::string GetRecipientID() const;
  std::string GetDonorName() const;
  std::string GetDonorZipCode() const;
  int GetTransactionYear() const;
  int GetTransactionAmount() const;

 private:
  std::string recipient_id_;
  std::string donor_name_;
  std::string zip_code_;
  int transaction_year_;
  int transaction_amount_;
};

#endif //DONATION_ANALYTICS_MASTER_RECORD_H_

Record::Record(std::vector<std::string> &record) {
  SetRecipientID(record[0]);
  SetDonorName(record[1]);
  SetDonorZipCode(record[2]);
  SetTransactionYear(record[3]);
  SetTransactionAmount(record[4]);
}

Record::~Record() {}

void Record::SetRecipientID(const std::string &recipient_id) {
  recipient_id_ = recipient_id;
}

void Record::SetDonorName(const std::string &donor_name) {
  donor_name_ = donor_name;
}

void Record::SetDonorZipCode(const std::string &zip_code) {
  zip_code_ = zip_code.substr(0, 5);
}

void Record::SetTransactionYear(const std::string &transaction_date) {
  transaction_year_ = stoi(transaction_date.substr(4, 4));
}

void Record::SetTransactionAmount(const std::string &transaction_amount) {
  transaction_amount_ = stoi(transaction_amount);
}

std::string Record::GetRecipientID() const { return recipient_id_; }

std::string Record::GetDonorName() const { return donor_name_; }

std::string Record::GetDonorZipCode() const { return zip_code_; }

int Record::GetTransactionYear() const { return transaction_year_; }

int Record::GetTransactionAmount() const { return transaction_amount_; }

std::ostream &operator<<(std::ostream &strm, const Record &r) {
  return strm << "Record: " << r.GetRecipientID() << "|" << r.GetDonorName()
         << "|" << r.GetDonorZipCode() << "|" << r.GetTransactionYear()
         << "|" << r.GetTransactionAmount();
}