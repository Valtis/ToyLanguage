#pragma once
#include "TokenType.h"
#include <string>

class Token
{
public:
  Token();
  Token(TokenType type, std::string token_value, int line_number);
  
  ~Token();

  TokenType Type() { return m_type; }
  std::string Value() { return m_token_value;  }
  int LineNumber() { return m_line_number;  }

private:
  TokenType m_type;
  std::string m_token_value;
  int m_line_number;

};

