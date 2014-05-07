#pragma once
#include "TokenType.h"
#include <string>

class Token
{
public:
  Token(TokenType type, std::string token_value);
  
  ~Token();

  TokenType Type() { return m_type; }
  std::string Value() { return m_token_value;  }

private:
  TokenType m_type;
  std::string m_token_value;

};

