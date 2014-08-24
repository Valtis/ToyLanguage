#pragma once
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error
{
public:

  ParseError(const std::string &what, int line_number) : std::runtime_error(what.c_str()), m_line_number(line_number)
  {

  }

  int LineNumber() const 
  {
    return m_line_number;
  }

private:

  int m_line_number;
};


#define PARSE_ERROR(NAME__) class NAME__ : public ParseError  \
 { \
  public: \
   NAME__(const std::string &what, int line_number) : ParseError(what.c_str(), line_number) {} \
};



PARSE_ERROR(UnexpectedTokenError)

PARSE_ERROR(UnexpectedEOFError)
PARSE_ERROR(UndefinedMainError)
PARSE_ERROR(UndefinedFunctionCall)
PARSE_ERROR(FunctionRedeclarationError)
PARSE_ERROR(InvalidTokenError)


