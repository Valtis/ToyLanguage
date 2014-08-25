#pragma once
#include <vector>
#include "ByteCode.h"

#define MAIN_FUNCTION_ID 0

class VMFunction
{
public:
  VMFunction();
  ~VMFunction();
  
  ByteCode GetByteCode(int position)
  {
    return m_bytecode[position];
  }

  void AddByteCode(ByteCode code)
  {
    m_bytecode.push_back(code);
  }

private:
  std::vector<ByteCode> m_bytecode;
};

