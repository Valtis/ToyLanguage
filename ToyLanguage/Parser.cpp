#include "Parser.h"
#include "ParseError.h"

Parser::Parser(std::vector<Token> &tokens) : m_tokens(tokens)
{

}


Parser::~Parser()
{

}

void Parser::Parse()
{
  while (HasTokens())
  {
    NextToken();

    switch (m_current_token.Type())
    {

      default:
        throw ParseError("Unexpected token ");
    }
  }
}


void Parser::NextToken()
{


}


bool Parser::HasTokens()
{

  return false;
}