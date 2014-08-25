#include "VM.h"
#include "Instructions/Instructions.h"



void VM::Initialize(std::unordered_map<int, VMFunction> code)
{
  m_functions = code;
  m_running = true;
  m_frames.clear();
  m_frame_pointer = 0;

  m_frames.push_back(StackFrame(MAIN_FUNCTION_ID));


}

void VM::Execute()
{
  while (m_running)
  {
    ByteCode code = m_functions[m_frames.back().FunctionID()].GetByteCode(m_frames.back().NextInstruction());
    switch (code.GetInstruction())
    {
    case Instruction::NOP:
      break;
    case Instruction::STOP:
      m_running = false;
      break;
    case Instruction::ADD:
      Add(m_frames.back());
      break;
    case Instruction::SUB:
      Sub(m_frames.back());
      break;
    case Instruction::MUL:
      Mul(m_frames.back());
      break;
    case Instruction::DIV:
      Div(m_frames.back());
      break;
    case Instruction::PRINT:
      Print(m_frames.back());
      break;

    case Instruction::PUSH:
      Push(m_frames.back(), *code.GetObject());
      break;
    }
  }
}