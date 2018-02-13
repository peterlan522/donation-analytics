/*
 * Main function of donation analytics application.
 * -----------------------------------------------------------------
 * Presumption: data in itcont are streaming in, the data structure
 *              can hold all data which need to be processed.
 *
 * -----------------------------------------------------------------
 * Author: Peter Lan
 *
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <sys/stat.h>
#include <utility>
#include <vector>
#include <exception>

#include "donor.h"
#include "parser.h"
#include "recipient_statistics.h"
#include "record.h"
#include "validate_date.h"

inline bool isFileExisted(const std::string &name) {
  struct stat buffer;
  return (stat(name.c_str(), &buffer) == 0);
}

/*
 * Function: InputAndParse
 * -------------------------------
 * Input: A file name, a delimiter.
 * Output: Vectors of tokens of each line in the file.
 */
std::vector<std::vector<std::string>> InputAndParse(const std::string &input_filename,
const char &delim) {
  if (isFileExisted(input_filename)) {
    std::ifstream file;
    std::vector<std::vector<std::string>> texts;
    try {
      file.open(input_filename);

      std::string line;
      Parser p(delim);

      while (getline(file, line)) {
        std::vector<std::string> tokens = p.ParseLine(line);
        texts.push_back(tokens);
      }
    } catch (const std::exception & e) {
      std::cerr << "Unable to open the file: " << input_filename << std::endl;
    }
    file.close();
    return texts;
  } else {
    std::cout << "Cannot find the file." << std::endl;
  }
}

/*
 * Function: Distill
 * -------------------------------
 * Input: Vector of lines, vector of indices of need fields.
 * Output: Vectors of tokens of need fields.
 */
std::vector<std::string> Distill(std::vector<std::string> &line,
                                 std::vector<int> &fields_needed) {

  std::vector<std::string> fields;
  if (!line.empty()) {
    for (int n : fields_needed) {
      fields.push_back(line[n]);
    }
    return fields;
  }
}

/*
 * Function: isValidField
 * -------------------------------
 * Input: Vector of need fields.
 * Output: True or false based on rules on those fields.
 */
bool isValidField(std::vector<std::string> &fields) {
  if (!fields.empty()) {

    const int kRecipient = 0;
    const int kDonorName = 1;
    const int kDonorZipCode = 2;
    const int kTransactionDate = 3;
    const int kTransactionAmount = 4;
    const int kPersonDonor = 5;

    std::string recipient = fields[kRecipient];
    std::string donor_name = fields[kDonorName];
    std::string zip_code = fields[kDonorZipCode];
    std::string transaction_date = fields[kTransactionDate];
    std::string transaction_amount = fields[kTransactionAmount];
    std::string is_person_donor = fields[kPersonDonor];

    const int kValidDateLength = 8;
    const std::string recipient_rule = "[A-Z]\\d{8}";
    const std::string zip_code_rule = "\\d{5,9}";

    ValidateDate validate_date(kValidDateLength);

    return std::regex_match(recipient, std::regex(recipient_rule)) &&
           donor_name != "" &&
           std::regex_match(zip_code, std::regex(zip_code_rule)) &&
           validate_date.isValidDate(transaction_date) &&
           transaction_amount != "" && is_person_donor == "";
  }
}

/*
 * Function: Filter
 * -------------------------------
 * Input: Vector of texts.
 * Output: Vector of qulified fields.
 */
std::vector<Record> Filter(std::vector<std::vector<std::string>> &texts,
                           std::vector<int> &fields_needed) {

  std::vector<Record> records;
  if (!texts.empty()) {
    for (std::vector<std::string> line : texts) {
      std::vector<std::string> fields = Distill(line, fields_needed);
      if (isValidField(fields)) {
        Record r(fields);
        records.push_back(r);
      }
    }
    return records;
  }
}

/*
 * Function: Write
 * -------------------------------
 * Input: An output file name, a sequence qulified to be written to file.
 */
template <typename T> void Write(std::string output_filename, T &output) {
  std::ofstream out_put_file;
  out_put_file.open(output_filename);
  for (auto one_line : output) {
    out_put_file << one_line << std::endl;
  }
  out_put_file.close(); // end of writing
}

std::string CheckFileExist(std::string output_filename) {

  while (true) {
    if (!isFileExisted(output_filename)) {
      return output_filename;
    }
    std::string user_input;
    std::cout << "File exists, overwrite?(y/n): ";
    std::cin >> user_input;
    if (user_input == "y" || user_input == "Y") {
      return output_filename;
    } else if (user_input == "n" || user_input == "N") {
      std::cout << "Input a new file name: ";
      std::cin >> output_filename;
    } else {
      std::cout << "Invalid input." << std::endl;
    }
  }
}

template <typename T> void WriteToFile(std::string &output_filename, T &output) {
  Write(CheckFileExist(output_filename), output);
}

/*
 * Function: GenerateReport
 * -------------------------------
 * Input: Vector of records, a number of percentile.
 * Output: Vector of computed information of records.
 */
std::vector<std::string> GenerateReport(std::vector<Record> &records,
                                        const int percentile) {
  std::vector<std::string> output;
  if (!records.empty()) {
    Donor donors;
    RecipientStatistics rs(percentile);

    for (Record r : records) {
      std::string donor_name = r.GetDonorName();
      std::string zip_code = r.GetDonorZipCode();
      int year = r.GetTransactionYear();

      if (donors.isRepeatedDonor(donor_name, zip_code, year)) {
        rs.AddData(r.GetRecipientID(), zip_code, year,
                   r.GetTransactionAmount());
        std::string result = rs.GetStatistics();
        output.push_back(result);
      }
    }
  }
  return output;
}

/*
 * Function: Main
 * -------------------------------
 * Input: Command line input 3 file names.
 * Description: Read file in, filter need fields based on rules,
 *              compute statistics of records, and write to file.
 */
int main(int argc, char *argv[]) {
  const int kNumOfArguments = 4;
  if (argc != kNumOfArguments) {
    std::cout << "usage: " << argv[0] << " <itcont_file> <percentile_file> <output_file>\n";
  } else {

    std::string input_filename = argv[1];
    std::string percentile_filename = argv[2];
    std::string output_filename = argv[3];

    int percentile = 0;
    const char delim = '|';

    // fields needed
    const int kRecipient = 0;
    const int kDonorName = 7;
    const int kDonorZipCode = 10;
    const int kTransactionDate = 13;
    const int kTransactionAmount = 14;
    const int kPersonDonor = 15;
    std::vector<int> fields_needed = {kRecipient,         kDonorName,
                                      kDonorZipCode,      kTransactionDate,
                                      kTransactionAmount, kPersonDonor
                                     };

    std::vector<std::vector<std::string>> texts = InputAndParse(input_filename, delim);
    std::vector<std::vector<std::string>> percentiles = InputAndParse(percentile_filename, delim);
    std::vector<Record> records = Filter(texts, fields_needed);

    for (std::vector<std::string> line : percentiles) {
      for (std::string percentile : line) {
        std::vector<std::string> output = GenerateReport(records, std::stoi(percentile));
        std::cout << "running ..." << std::endl;
        WriteToFile(output_filename, output);
      }
    }
    std::cout << "done!" << std::endl;
  }
  return 0;
}
