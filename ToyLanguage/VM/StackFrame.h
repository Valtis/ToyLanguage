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



  void SetInstruction(int instruction)
  {
    m_instruction_pointer = instruction;
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

  void SetVariable(const int id, VMObject o)
  {
    if (m_local_variables.size() <= id)
    {
      m_local_variables.resize(id + 1);
    }
    m_local_variables[id] = o;
  }

private:
  int m_functionID;
  int m_instruction_pointer;
  std::vector<VMObject> m_operand_stack;
  std::vector<VMObject> m_local_variables;
};

