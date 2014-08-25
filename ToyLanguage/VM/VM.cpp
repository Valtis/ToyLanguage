#include "VM.h"
#include "Instructions/Instructions.h"



void VM::Initialize()
{
  m_running = true;
  m_frames.clear();
  m_frame_pointer = 0;

  m_frames.push_back(StackFrame(0));
}

void VM::Execute()
{
  while (m_running)
  {
    ByteCode code = m_functions[m_frames.back().FunctionID()].GetByteCode(m_frames.back().NextInstruction());
    switch (code.instruction)
    {
    case Instruction::NOP:
      break;
    case Instruction::STOP:
      m_running = false;
      break;
    case Instruction::ADD:
      Add(m_frames.back());
    case Instruction::PUSH:
      Push(m_frames.back(), *code.object);
      break;
    }
  }
}