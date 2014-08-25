#pragma once
#include <cstdint>
#include "VMObject.h"

enum class Instruction : std::uint8_t { NOP, ADD, STOP, PUSH };




struct ByteCode
{

  ByteCode() : instruction(Instruction::NOP), object(nullptr)
  {

  }

  ~ByteCode()
  {
    delete object;
  }

  Instruction instruction;
  VMObject *object; // optional, used by some instructions
  
};