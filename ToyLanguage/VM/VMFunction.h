#pragma once
#include <vector>
#include "ByteCode.h"
#include <string>

#define MAIN_FUNCTION_ID 0

class VMFunction
{
public:
  VMFunction();
  VMFunction(std::string name, int parameter_count);
  ~VMFunction();
  
  ByteCode GetByteCode(int position)
  {
    return m_byte_code[position];
  }

  void AddByteCode(ByteCode code)
  {
    m_byte_code.push_back(code);
  }

  int ParameterCount() 
  {
    return m_parameter_count;
  }

  std::string Name() 
  {
    return m_name;
  }

  std::vector<ByteCode> GetByteCode()
  {
    return m_byte_code;
  }

  void SetByteCode(const std::vector<ByteCode> &byte_code)
  {
    m_byte_code = byte_code;
  }

private:
  std::string m_name;
  std::vector<ByteCode> m_byte_code;
  int m_parameter_count;
};

