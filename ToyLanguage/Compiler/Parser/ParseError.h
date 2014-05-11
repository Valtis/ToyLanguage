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



class MissingParenthesisError : public ParseError
{
public:
  MissingParenthesisError(std::string what) : ParseError(what.c_str())
  {

  }

private:
  int m_line;

}; 

class MissingOperandError : public ParseError
{
public:
  MissingOperandError(std::string what) : ParseError(what.c_str())
  {

  }

private:
  int m_line;

};

class VariableShadowsFunctionError : public ParseError
{
public:
  VariableShadowsFunctionError(std::string what) : ParseError(what.c_str())
  {

  }

private:
  int m_line;

};