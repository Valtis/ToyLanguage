#pragma once
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error
{
public:

  ParseError(std::string what) : std::runtime_error(what.c_str())
  {

  }

  ParseError(const char *what) : std::runtime_error(what)
  {

  }
  
};

class UndeclaredVariableError : public ParseError
{
public:
  UndeclaredVariableError(std::string what) : ParseError(what.c_str())
  {

  }

  UndeclaredVariableError(Token token) : ParseError("Undeclared variable " + token.Value()), m_token(token)
  {

  }
  Token GetToken() const { return m_token;  }

private:
  Token m_token;



};

class VariableRedeclarationError : public ParseError
{
public:
  VariableRedeclarationError(std::string what) : ParseError(what.c_str())
  {

  }
};