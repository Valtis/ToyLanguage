#pragma once
#include <vector>
#include "ByteCode.h"
class VMFunction
{
public:
  VMFunction();
  ~VMFunction();
  
  ByteCode GetByteCode(int position)
  {
    return m_bytecode[position];
  }

private:
  std::vector<ByteCode> m_bytecode;
};

