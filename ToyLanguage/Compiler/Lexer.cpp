#include "Lexer.h"
#include "../Utility/StringUtility.h"
#include <sstream>
#include <algorithm>
#include "Parser/ParseError.h"

Lexer::Lexer(std::istream &input) : m_input(input), m_current_line(1)
{

  m_reserved_words = { "fn", "while", "if", "var" };

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
}


Lexer::~Lexer()
{
}


std::pair<std::vector<std::string>, std::vector<Token>> Lexer::AnalyzeText()
{
  HandleLines();
  return{ m_lines, m_syntax_tokens };
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


void Lexer::HandleTokens(const std::string &str_token)
{

  auto space_tokens = Utility::Tokenize(Utility::Trim(str_token), " ");

  for (auto space_token : space_tokens)
  {
      HandleComplexToken(space_token);
  }
}


void Lexer::HandleComplexToken(const std::string &token)
{
  if (token.empty())
  {
    return;
  }

  if (!MatchReservedWord(token) && !MatchNumber(token) && !MatchIdentifier(token) && !MatchSingleCharacterToken(token))
  {
    throw InvalidTokenError("Invalid token " + token, m_current_line);
  }
}


bool Lexer::MatchReservedWord(const std::string &token)
{
  auto match = MathReservedWordFromString(token);
  if (!match.match_string.empty())
  {
    return MatchTypeOf(m_string_tokens[match.match_string], match);
  }
  return false;
}


bool Lexer::MatchNumber(const std::string &token)
{
  return MatchTypeOf(TokenType::NUMBER, MatchNumberFromString(token));
}

bool Lexer::MatchIdentifier(const std::string & token)
{
  return MatchTypeOf(TokenType::IDENT, MatchIdentifierFromString(token));
}

bool Lexer::MatchTypeOf(TokenType type, Match match)
{
  if (!match.match_string.empty())
  {
    m_syntax_tokens.push_back(Token{ type, match.match_string, m_current_line });
    
     if (match.remnant_string.length() > 0 && !(match.remnant_string[0] == ')' || match.remnant_string[0] == '('))
     {
       throw InvalidTokenError("Invalid token", m_current_line);
     }

     HandleComplexToken(match.remnant_string);

    return true;
  }
  return false;
}



bool Lexer::MatchSingleCharacterToken(const std::string &token)
{
  std::string chr(1, token[0]);
  if (m_string_tokens.count(chr) != 0)
  {
    m_syntax_tokens.push_back(Token{ m_string_tokens[chr], chr, m_current_line });
    HandleComplexToken(token.substr(1));
    return true;
  }
  return false;
}


Lexer::Match Lexer::MathReservedWordFromString(const std::string &characters) {
  for (auto word : m_reserved_words)
  {
    std::regex ex("^" + word);
    auto ret = MatchRegexFromString(characters, ex);
    if (!ret.match_string.empty())
    {
      return ret;
    }
  }
  return{ "", characters };
}

// returns pair where first member contains the parsed number, second contains remnant of the string
Lexer::Match Lexer::MatchNumberFromString(const std::string &characters)
{
  std::regex ex("^(\\+|-)?(\\d)+(\\.\\d+)?");
  return MatchRegexFromString(characters, ex);
}


// returns pair where first member contains the parsed identifier, second contains remnant of the string
Lexer::Match Lexer::MatchIdentifierFromString(const std::string &characters)
{
  std::regex ex("^\\w+(\\d|\\w|-|\\?|\\+|!|)*");
  return MatchRegexFromString(characters, ex);
}


Lexer::Match Lexer::MatchRegexFromString(const std::string &characters, const std::regex &ex)
{
  Match match_strings;

  std::smatch match;

  std::regex_search(characters, match, ex);

  if (match.begin() != match.end()) {
    match_strings.match_string = *match.begin();
  }

  match_strings.remnant_string = match.suffix().str();
  return match_strings;
}

void Lexer::RemoveComment(std::string & line)
{
  auto comment_start = std::find(line.begin(), line.end(), '#');
  int dist = std::distance(line.begin(), comment_start);
  line = line.substr(0, dist);
}

