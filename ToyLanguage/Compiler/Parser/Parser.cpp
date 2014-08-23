#include "Parser.h"
#include "ParseError.h"
#include <list>
#include <algorithm>


Parser::Parser(std::pair<std::vector<std::string>, std::vector<Token>> &tokens) : m_lines(tokens.first), m_tokens(tokens.second), m_current_token(m_tokens.begin())
{
  token_to_string[TokenType::FUNCTION] = "function declaration";
  token_to_string[TokenType::WHILE] = "while";
  token_to_string[TokenType::IF] = "if";
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
  token_to_string[TokenType::QUOTE] = "\"";
  token_to_string[TokenType::IDENT] = "identifier";
  token_to_string[TokenType::MULTIPLICATION] = "*";
}


Parser::~Parser()
{

}

std::unordered_map<std::string, Function> Parser::Parse()
{
  while (HasTokens())
  {
    ParseFunctionDeclaration();
  }


  if (m_functions.count("main") == 0)
  {
    throw UndefinedMainError("main-function not defined", m_tokens.back().LineNumber()+1);
  }

  return m_functions;
}

void Parser::ParseFunctionDeclaration()
{
  Function f;
  Expect(TokenType::LPAREN);
  Expect(TokenType::FUNCTION);

  auto name_token = Expect(TokenType::IDENT);
  if (m_functions.count(name_token.Value()) != 0) {
    throw FunctionRedeclarationError("Redeclaration of function '" + name_token.Value() + "'", name_token.LineNumber());
  }

  ParseFunctionArguments(f);
  ParseFunctionBody(f);

  Expect(TokenType::RPAREN);

  m_functions[name_token.Value()] = f;
}

void Parser::ParseFunctionArguments(Function &f)
{
  Expect(TokenType::LPAREN);
  Expect(TokenType::RPAREN);
}


void Parser::ParseFunctionBody(Function &f)
{
  Expect(TokenType::LPAREN);
  Expect(TokenType::RPAREN);
}




Token Parser::CurrentToken()
{
  if (m_current_token == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file", m_tokens.back().LineNumber() + 1);
  }
  return *m_current_token;
}

Token Parser::NextToken()
{
  ++m_current_token;
  return CurrentToken();
}

Token Parser::PeekToken()
{
  if (m_current_token + 1 == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file", m_tokens.back().LineNumber() + 1);
  }
  return *(m_current_token + 1);
}


bool Parser::HasTokens()
{
  return m_current_token != m_tokens.end();
}

Token Parser::Expect(TokenType type)
{

  if (m_current_token == m_tokens.end())
  {
    throw UnexpectedEOFError("Unexpected end of file", m_tokens.back().LineNumber() + 1);
  }

  if (m_current_token->Type() != type)
  {
    throw UnexpectedTokenError("Expected '" + token_to_string[type] + "', actual '" + token_to_string[m_current_token->Type()] + "'", m_current_token->LineNumber());
  }
  auto token = CurrentToken();
  ++m_current_token;
  //NextToken();

  return token;
}
