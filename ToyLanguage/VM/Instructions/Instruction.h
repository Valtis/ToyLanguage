#pragma once

class VM;
class Instruction
{
public:
  Instruction(VM *vm);
  virtual void Execute() = 0;
};