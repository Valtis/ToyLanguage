#pragma once
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error
{
public:

  ParseError(std::string what) : std::runtime_error(what.c_str())
  {

  }

  ParseError(const char *what) : std::runtime_error(what)
  {

  }
  
};