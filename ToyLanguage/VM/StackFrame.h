#pragma once
#include <vector>
#include "VMObject.h"
#include "VMError.h"

class StackFrame
{
public:
  StackFrame(int functionID);
  ~StackFrame();

  int FunctionID() 
  {
    return m_functionID;
  }
  
  int NextInstruction()
  {
    return m_instruction_pointer++;
  }


  bool StackIsEmpty()
  {
    return m_operand_stack.empty();
  }

  VMObject Pop() 
  {
    auto top = m_operand_stack.back();
    m_operand_stack.pop_back();
    return top;
  }

  void Push(const VMObject &o)
  {
    m_operand_stack.push_back(o);
  }

  void AddVariable(const VMObject &o)
  {
    m_local_variables.push_back(o);
  }

  VMObject &GetVariable(const int id)
  {
    return m_local_variables[id];
  }

private:
  int m_functionID;
  int m_instruction_pointer;
  std::vector<VMObject> m_operand_stack;
  std::vector<VMObject> m_local_variables;
};

