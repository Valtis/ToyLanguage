#pragma once 
#include <stdexcept>
#include <string>

class SemanticError : public std::runtime_error
{
public:

  SemanticError(const std::string &what, int line_number) : std::runtime_error(what.c_str()), m_line_number(line_number)
  {

  }

  int LineNumber() const
  {
    return m_line_number;
  }

private:

  int m_line_number;
};


#define SEMANTIC_ERROR(NAME__) class NAME__ : public SemanticError  \
 { \
  public: \
   NAME__(const std::string &what, int line_number) : SemanticError(what.c_str(), line_number) {} \
};

SEMANTIC_ERROR(ParameterCountMismatchError)