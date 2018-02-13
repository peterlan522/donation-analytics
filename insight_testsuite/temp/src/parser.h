#ifndef DONATION_ANALYTICS_MASTER_PARSER_H_
#define DONATION_ANALYTICS_MASTER_PARSER_H_

#include <cstring>
#include <sstream>
#include <vector>

class Parser {
 public:
  Parser(char delimiter);
  ~Parser();

  std::vector<std::string> ParseLine(const std::string &line);

 private:
  char delimiter_;
};

#endif //DONATION_ANALYTICS_MASTER_PARSER_H_

Parser::Parser(char delimiter) { delimiter_ = delimiter; }

Parser::~Parser() {}

std::vector<std::string> Parser::ParseLine(const std::string &line) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(line);
  while (std::getline(tokenStream, token, delimiter_)) {
    tokens.push_back(token);
  }
  return tokens;
}