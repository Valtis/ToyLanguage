#include "Lexer.h"
#include "../Utility/StringUtility.h"
#include <sstream>
#include <algorithm>
#include <iostream>

Lexer::Lexer(std::istream &input) : m_input(input), m_current_line(1)
{
  m_string_tokens["fn"] = TokenType::FUNCTION;
  m_string_tokens["while"] = TokenType::WHILE;
  m_string_tokens["if"] = TokenType::IF;
  m_string_tokens["("] = TokenType::LPAREN;
  m_string_tokens[")"] = TokenType::RPAREN;
  m_string_tokens["{"] = TokenType::LBRACE;
  m_string_tokens["}"] = TokenType::RBRACE;
  m_string_tokens[","] = TokenType::COMMA;
  m_string_tokens[":"] = TokenType::COLON;
  m_string_tokens[";"] = TokenType::SEMICOLON;
  m_string_tokens["var"] = TokenType::VARIABLE;
  m_string_tokens["\""] = TokenType::QUOTE;
  m_string_tokens["="] = TokenType::ASSIGNMENT;
  m_string_tokens["+"] = TokenType::PLUS;
  m_string_tokens["-"] = TokenType::MINUS;
  m_string_tokens["*"] = TokenType::MULTIPLICATION;
  m_string_tokens["/"] = TokenType::DIVISION;
  m_string_tokens["void"] = TokenType::VOID_TOKEN;
  m_string_tokens["int"] = TokenType::INT_TOKEN;
}


Lexer::~Lexer()
{
}


std::pair<std::vector<std::string>, std::vector<Token>> Lexer::AnalyzeText()
{
  HandleLines();
  return { m_lines, m_syntax_tokens };
}

void Lexer::HandleLines()
{
  std::string line;
  while (getline(m_input, line))
  {
    HandleLine(line);
    ++m_current_line;
  }
}

void Lexer::HandleLine(std::string &line)
{
  RemoveComment(line);
  m_lines.push_back(line);
  auto str_tokens = Utility::Tokenize(line, "\"");
  int pos = 0;

  for (auto str_token : str_tokens)
  {
    ++pos;
    if (pos % 2 == 0)
    {
      m_syntax_tokens.push_back(Token{ TokenType::TEXT, str_token, m_current_line });
      continue;
    }

    HandleTokens(str_token);
  }
}


void Lexer::HandleTokens(std::string &str_token)
{
  str_token = Utility::Trim(str_token);
  
  auto space_tokens = Utility::Tokenize(str_token, " ");

  for (auto space_token : space_tokens)
  {
    if (m_string_tokens.count(space_token) != 0)
    {
      m_syntax_tokens.push_back(Token{ m_string_tokens[space_token], space_token, m_current_line });
    }
    else
    {
      HandleComplexToken(space_token);
    }
  }
}


void Lexer::HandleComplexToken(std::string token)
{
  std::string characters;


  for (char character : token)
  {
    // simply using std::string characters to match the string in map fails - TODO: Find out why (null terminator or similar issue maybe when adding characters one by one?)
    std::stringstream str;
    str << character;
    std::string chr_string = str.str();
    if (m_string_tokens.count(chr_string) == 0)
    {
      characters += character;
    }
    else
    {
      AddTokenFromCharacters(characters);
      characters = "";
      m_syntax_tokens.push_back(Token{ m_string_tokens[chr_string], chr_string, m_current_line });
    }

  }

  AddTokenFromCharacters(characters);

}

void Lexer::AddTokenFromCharacters(std::string characters)
{
  if (characters != "")
  {
    if (m_string_tokens.count(characters) == 0)
    {
      if (isdigit(characters[0]))
      {
        m_syntax_tokens.push_back(Token{ TokenType::NUMBER, characters, m_current_line });
      }
      else
      {
        m_syntax_tokens.push_back(Token{ TokenType::IDENT, characters, m_current_line });
      }  
    }
    else
    {
      m_syntax_tokens.push_back(Token{ m_string_tokens[characters], characters, m_current_line });

    }
  }
}

void Lexer::RemoveComment(std::string & line)
{
  auto comment_start = std::find(line.begin(), line.end(), '#');
  int dist = std::distance(line.begin(), comment_start);
  line = line.substr(0, dist);
}
