#pragma once
#include <vector>
#include "ByteCode.h"

#define MAIN_FUNCTION_ID 0

class VMFunction
{
public:
  VMFunction();
  VMFunction(int parameter_count);
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

private:
  std::vector<ByteCode> m_bytecode;
  int m_parameter_count;
};

