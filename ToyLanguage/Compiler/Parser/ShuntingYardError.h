#pragma once
#include "ParseError.h"
#include "../DataStructures/Token.h"



class ShuntingYardError : public std::runtime_error
{
public:

  ShuntingYardError(Token token) : m_token(token), std::runtime_error("")
  {

  }
  
  Token GetToken() const
  {
    return m_token;
  }

private:
  Token m_token;

};

class ShuntingYardUndeclaredVariableError : public ShuntingYardError
{
public:
  ShuntingYardUndeclaredVariableError(Token token) : ShuntingYardError(token)
  {

  }
};

class ShuntingYardMissingParenthesisError : public ShuntingYardError
{
public:
  ShuntingYardMissingParenthesisError(Token token) : ShuntingYardError(token)
  {

  }
};

class ShuntingYardMissingOperandError : public ShuntingYardError
{
public:
  ShuntingYardMissingOperandError(Token token) : ShuntingYardError(token)
  {

  }
};

