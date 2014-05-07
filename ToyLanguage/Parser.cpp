#include "Parser.h"
#include "ParseError.h"

Parser::Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens) : m_lines(tokens.first), m_tokens(tokens.second), m_current_token(m_tokens.begin())
{
  token_to_string[TokenType::FUNCTION] = "function declaration";
  token_to_string[TokenType::TEXT] = "Text";
  token_to_string[TokenType::NUMBER] = "Number";
  token_to_string[TokenType::LPAREN] = "(";
  token_to_string[TokenType::RPAREN] = ")";
  token_to_string[TokenType::LBRACE] = "{";
  token_to_string[TokenType::RBRACE] = "}";
  token_to_string[TokenType::COMMA] = ",";
  token_to_string[TokenType::COLON] = ":";
  token_to_string[TokenType::SEMICOLON] = ";";
  token_to_string[TokenType::VARIABLE] = "variable declaration";
  token_to_string[TokenType::ASSIGNMENT] = "=";
  token_to_string[TokenType::PLUS] = "+";
  token_to_string[TokenType::VOID_TOKEN] = "void";
  token_to_string[TokenType::INT_TOKEN] = "int";
  token_to_string[TokenType::QUOTE] = "\"";
  token_to_string[TokenType::IDENT] = "identifier";
  token_to_string[TokenType::MULTIPLICATION] = "*";

}


Parser::~Parser()
{

}

void Parser::Parse()
{
  while (HasTokens())
  {
    switch (m_current_token->Type())
    {
    case TokenType::FUNCTION:
      ParseFunction();
      break;
    default:
      InvalidTokenError("Expected function declaration");
    }
    NextToken();
  }
}

void Parser::ParseFunction()
{
  Expect(TokenType::IDENT);

  // start new function AST

  Expect(TokenType::LPAREN);
  
  if (PeekNextToken().Type() == TokenType::VARIABLE)
  {
    ParseVariableList();
  }
    
  Expect(TokenType::RPAREN);
  Expect(TokenType::COLON);

  ExpectType();


  ParseBlock();
}


void Parser::ParseVariableList()
{
  while (true)  
  {
    Expect(TokenType::VARIABLE);
    Expect(TokenType::IDENT);
    Expect(TokenType::COLON);
    ExpectNonVoidType();
    if (PeekNextToken().Type() != TokenType::COMMA)
      break;
    NextToken();
    
  }

}

void Parser::ParseBlock()
{

  Expect(TokenType::LBRACE);



  
  Expect(TokenType::RBRACE);

}




void Parser::NextToken()
{
  ++m_current_token;
  if (m_current_token == m_tokens.end())
  {
    throw ParseError("Unexpected end of file");
  }
}

Token Parser::PeekNextToken()
{
  if (m_current_token + 1 == m_tokens.end())
  {
    throw ParseError("Unexpected end of file");
  }
  return *(m_current_token + 1);
}


bool Parser::HasTokens()
{
  return m_current_token != m_tokens.end();
}

void Parser::Expect(TokenType type)
{
  NextToken();
  if (m_current_token->Type() != type)
  {
    InvalidTokenError("Expected '" + token_to_string[type] + "'\n");
  }
}

void Parser::ExpectType()
{
  NextToken();
  auto type = m_current_token->Type();
  if (type != TokenType::VOID_TOKEN && type != TokenType::INT_TOKEN)
  {

  }
}

void Parser::ExpectNonVoidType()
{
  NextToken();
  auto type = m_current_token->Type();
  if (type != TokenType::INT_TOKEN)
  {

  }
}


void Parser::InvalidTokenError(std::string expected)
{
  throw ParseError("Unexpected " + GetTokenErrorInfo() + expected + "\n");
}

std::string Parser::GetTokenErrorInfo()
{
  int line_number = m_current_token->LineNumber();
  return " token '" + m_current_token->Value() + "' at line " + std::to_string(line_number) + "\n\n  "
    + m_lines[line_number-1] + "\n\n";
}