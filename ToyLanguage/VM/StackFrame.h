#pragma once
#include <array>
//#include "Variable.h"
#include "Defines.h"

class StackFrame
{
public:
  StackFrame();
  ~StackFrame();

private:
  int m_stack_position;
  //std::array<Variable, OPERAND_STACK_SIZE> m_operand_stack;
};

