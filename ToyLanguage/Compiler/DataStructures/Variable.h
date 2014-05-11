#pragma once
#include <string>

enum class VariableType { NONE, INTEGER, LONG, STRING, FLOAT, DOUBLE, BOOLEAN };
// todo: Move

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
    memset(&m_value, 0, sizeof(VariableValue));
  }

  Variable(std::string name, int line) : m_declaration_line(line), m_name(name)
  {
    DeallocateString();
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
    DeallocateString();
    m_value.type = type;
    *((T *)(&m_value.value)) = value;
  }

  template <>
  void SetValue(VariableType type, std::string value)
  {
    DeallocateString();
    m_value.type = type;
    m_value.value.string_value.length =  value.length();
    m_value.value.string_value.str = new char[value.length() + 1];
    memset(m_value.value.string_value.str, 0, value.length() + 1);
#pragma warning(disable:4996)
    strncpy(m_value.value.string_value.str, value.c_str(), value.length());
  }

  template <>
  void SetValue(VariableType type, char *value)
  {
    DeallocateString();
    m_value.type = type;
    m_value.value.string_value.length = strlen(value);
    m_value.value.string_value.str = new char[strlen(value) + 1];
    memset(m_value.value.string_value.str, 0, strlen(value) + 1);
#pragma warning(disable:4996)
    strncpy(m_value.value.string_value.str, value, strlen(value));
  }

  VariableValue VariableData()
  {
    return m_value;
  }

private:
  void DeallocateString()
  {
    if (m_value.type == VariableType::STRING && m_value.value.string_value.length > 0)
    {
      delete [] m_value.value.string_value.str;
      m_value.value.string_value.str = nullptr;
      m_value.value.string_value.length = 0;
    }
  }
  int m_declaration_line;
  VariableValue m_value;
  std::string m_name;
};