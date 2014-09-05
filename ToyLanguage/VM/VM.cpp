#include "VM.h"
#include "Instructions/Instructions.h"
#include "VMError.h"
#include <string>

// 10 megabyte heap for now
VM::VM() : m_memory_manager(1024 * 1024 * 10)
{

}

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
    case Instruction::PRINTLINE:
      PrintLine(m_frames.back());
      break;
    case Instruction::COMPARE:
      Equals(m_frames.back());
      break;

    case Instruction::JUMP_IF_TRUE:
      JumpIfTrue(m_frames.back(), code.GetObject());
      break;

    case Instruction::JUMP_IF_FALSE:
      JumpIfFalse(m_frames.back(), code.GetObject());
      break;
    case Instruction::JUMP:
      Jump(m_frames.back(), code.GetObject()); 
        break;

    case Instruction::CALLFUNCTION:
      CallFunction(this, m_frames.back());
      break;
    case  Instruction::RETURN:
      Return(this, m_frames.back());
      break;

    case Instruction::PUSH:
      Push(m_frames.back(), code.GetObject());
      break;
    case Instruction::PUSH_VARIABLE:
      PushVariable(m_frames.back(), code.GetObject());
      break;

    case Instruction::ALLOCATE_PTR:
      AllocatePtr(m_frames.back(), m_memory_manager);
      break;

    case Instruction::WRITE_PTR:
      WritePtr(m_frames.back(), m_memory_manager);
      break;

    case Instruction::READ_PTR:
      ReadPtr(m_frames.back(), m_memory_manager);
      break;

    default:
      throw UndefinedInstructionError(std::string("Instruction ") + std::to_string(static_cast<int>(code.GetInstruction())) + " was not recognized by the virtual machine");
    }
  }
}