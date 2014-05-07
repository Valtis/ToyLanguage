#include "Lexer.h"
#include "StringUtility.h"
#include <sstream>
#include <iostream>

Lexer::Lexer(std::istream &input) : m_input(input)
{
  m_string_tokens["fn"] = TokenType::FUNCTION;
  m_string_tokens["("] = TokenType::LPAREN;
  m_string_tokens[")"] = TokenType::RPAREN;
  m_string_tokens["{"] = TokenType::LBRACE;
  m_string_tokens["}"] = TokenType::RBRACE;
  m_string_tokens[":"] = TokenType::COLON;
  m_string_tokens[";"] = TokenType::SEMICOLON;
  m_string_tokens["var"] = TokenType::VARIABLE;
  m_string_tokens["\""] = TokenType::QUOTE;
  m_string_tokens["="] = TokenType::ASSIGNMENT;
  m_string_tokens["+"] = TokenType::PLUS;
  m_string_tokens["+="] = TokenType::PLUSEQUAL;
  m_string_tokens["void"] = TokenType::VOID_TOKEN;
  m_string_tokens["int"] = TokenType::INT_TOKEN;


}


Lexer::~Lexer()
{
}


std::vector<Token> Lexer::AnalyzeText()
{
  std::vector<Token> syntax_tokens;
  HandleLines(syntax_tokens);
  return syntax_tokens;
}

void Lexer::HandleLines(std::vector<Token> &syntax_tokens)
{
  std::string line;
  while (getline(m_input, line))
  {
    HandleLine(line, syntax_tokens);
  }
}

void Lexer::HandleLine(std::string &line, std::vector<Token> &syntax_tokens)
{
  auto str_tokens = Utility::Tokenize(line, "\"");
  int pos = 0;

  for (auto token : str_tokens)
  {
    ++pos;
    if (pos % 2 == 0)
    {
      syntax_tokens.push_back(Token{ TokenType::TEXT, token });
      continue;
    }

    auto space_tokens = Utility::Tokenize(token, " ");

    for (auto space_token : space_tokens)   
    {
      if (m_string_tokens.count(space_token) != 0)
      {
        syntax_tokens.push_back(Token{ m_string_tokens[space_token], space_token });
      }
      else
      {
        HandleComplexToken(space_token, syntax_tokens);
      }
    }
  }
}

void Lexer::HandleComplexToken(std::string token, std::vector<Token> &syntax_tokens)
{
  std::string characters;


  for (char character : token)
  {
    std::stringstream str;
    str << character;
    std::string chr_string = str.str();

    if (m_string_tokens.count(chr_string) == 0)
    {
      characters += character;
    }
    else
    {
      if (characters != "")
      {
        if (m_string_tokens.count(characters) == 0)
        {
          syntax_tokens.push_back(Token{ TokenType::IDENT, characters });
        }
        else
        {
          syntax_tokens.push_back(Token{ m_string_tokens[characters], characters });

        }
        characters = "";
      }
      syntax_tokens.push_back(Token{ m_string_tokens[chr_string], chr_string });
    }

  }

  if (characters != "")
  {
    if (m_string_tokens.count(characters) == 0)
    {
      syntax_tokens.push_back(Token{ TokenType::IDENT, characters });
    }
    else
    {
      syntax_tokens.push_back(Token{ m_string_tokens[characters], characters });

    }
  }
}
