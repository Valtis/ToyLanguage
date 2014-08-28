#pragma once
#include <stdexcept>

class InternalVMError : public std::runtime_error
{
public:

  InternalVMError(const std::string &what) : std::runtime_error(what.c_str())
  {

  }
private:

};


#define VM_ERROR(NAME__) class NAME__ : public InternalVMError { \
public: \
NAME__(const std::string &what) : InternalVMError(what) {} \
}; 



VM_ERROR(StackUnderFlowError)
VM_ERROR(StackOverFlowError)
VM_ERROR(UndefinedInstructionError)
VM_ERROR(InvalidOperandError)