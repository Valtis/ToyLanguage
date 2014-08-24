#pragma once

#include <string>

enum class TokenType : int {
  INVALID_TOKEN, FUNCTION, WHILE, IF, TEXT, IDENT, NUMBER, LPAREN, RPAREN, LBRACE, RBRACE, COMMA, COLON, SEMICOLON, VARIABLE, QUOTE, ASSIGNMENT,
  PLUS, MINUS, MULTIPLICATION, DIVISION
};

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

