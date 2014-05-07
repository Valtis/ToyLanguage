#pragma once
#include <vector>
#include <string>

namespace Utility
{
  std::vector<std::string> Tokenize(const std::string &text, const std::string &delimiters);
  std::string Trim(const std::string &line);
  void ToLower(std::string &line);
}