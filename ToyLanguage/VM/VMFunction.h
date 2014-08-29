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
    return m_bytecode[position];
  }

  void AddByteCode(ByteCode code)
  {
    m_bytecode.push_back(code);
  }

  int ParameterCount() 
  {
    return m_parameter_count;
  }

  std::string Name() 
  {
    return m_name;
  }

private:
  std::string m_name;
  std::vector<ByteCode> m_bytecode;
  int m_parameter_count;
};

