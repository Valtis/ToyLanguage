#pragma once
#include <stdexcept>

class ParseError : std::runtime_error
{
public:
  ParseError(const char *what) : std::runtime_error(what)
  {

  }
  
};