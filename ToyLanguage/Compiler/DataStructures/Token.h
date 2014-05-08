#pragma once
#include "TokenType.h"
#include <string>

class Token
{
public:
  Token();
  Token(TokenType type, std::string token_value, int line_number);
  ~Token();

  TokenType Type() const { return m_type; }
  std::string Value() const { return m_token_value;  }
  int LineNumber() const { return m_line_number;  }

private:
  TokenType m_type;
  std::string m_token_value;
  int m_line_number;

};

