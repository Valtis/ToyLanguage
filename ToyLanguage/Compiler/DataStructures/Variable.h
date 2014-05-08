#pragma once
#include <string>

enum class VariableType { NONE, INTEGER, LONG, STRING, FLOAT, DOUBLE, BOOLEAN };

struct VariableValue
{
  VariableType type;
  union 
  {
    int integer_value;
    long long_value;

    struct
    {
      char *str;
      int length;
    } string_value;

    float float_value;
    double double_value;
    bool boolean_value;
  } value;
};


class Variable
{
public:
  Variable() : m_declaration_line(-1), m_name("")
  {

  }

  Variable(std::string name, int line) : m_declaration_line(line), m_name(name)
  {

  }

  int DeclarationLine()
  {
    return m_declaration_line;
  }

  std::string Name()
  {
    return m_name;
  }
  
  template <typename T>
  void SetValue(VariableType type, T value)
  {
    m_value.type = type;
    *((T *)(&m_value.value)) = value;
  }

  template <>
  void SetValue(VariableType type, std::string value)
  {
    m_value.type = type;
    m_value.value.string_value.length =  value.length();
    m_value.value.string_value.str = new char[value.length() + 1];
    memset(m_value.value.string_value.str, 0, value.length() + 1);
#pragma warning(disable:4996)
    strncpy(m_value.value.string_value.str, value.c_str(), value.length() + 1);
  }

  VariableValue Value()
  {
    return m_value;
  }

private:
  int m_declaration_line;
  VariableValue m_value;
  std::string m_name;
};