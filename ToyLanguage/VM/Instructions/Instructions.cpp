#include "Instructions.h"
#include "ConversionFunctions.h"
#include "..\StackFrame.h"
#include "..\VMObject.h"
#include "..\VM.h"
#include "..\VMError.h"

#include <functional>
#include <cstdio>
void ArithmeticHelper(StackFrame &frame, VMObject(*operation)(const VMObject &, const VMObject &))
{ 

  VMObject second = Pop(frame);
  VMObject first = Pop(frame);

  if (second.type != VMObjectType::NUMBER || first.type != VMObjectType::NUMBER)
  {
    throw InvalidOperandError("Invalid operand for arithmetic operation: number expected");
  }
  
  Push(frame, operation(first, second));
}

void Add(StackFrame &frame)
{
  ArithmeticHelper(frame, [](const VMObject &first, const VMObject &second) { 
    VMObject o = first;
    o.value.number += second.value.number;
    return o;
  });
}

void Sub(StackFrame &frame)
{
  ArithmeticHelper(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number -= second.value.number;
    return o;
  });
}

void Mul(StackFrame &frame)
{
  ArithmeticHelper(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number *= second.value.number;
    return o;
  });
}

void Div(StackFrame &frame)
{
  ArithmeticHelper(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number /= second.value.number;
    return o;
  });
}

void CallFunction(VM *vm, StackFrame &frame)
{
  int new_function_id = Pop(frame).value.integer;
  
  StackFrame new_frame(new_function_id);

  VMFunction f = vm->Function(new_function_id);
  std::vector<VMObject> parameters;
  for (int i = 0; i < f.ParameterCount(); ++i)
  {
    parameters.push_back(Pop(vm->CurrentFrame()));
  }

  for (auto it = parameters.rbegin(); it < parameters.rend(); ++it)
  {
    new_frame.AddVariable(*it);
  }



  vm->PushFrame(new_frame);
}

void Return(VM *vm, StackFrame &frame)
{
  std::vector<VMObject> objects;
  while (!frame.StackIsEmpty())
  {
    objects.push_back(Pop(frame));
  }

  vm->PopFrame();
  
  for (auto it = objects.rbegin(); it != objects.rend(); ++it) 
  { 
    Push(vm->CurrentFrame(), *it);
  }
  
}


void Print(StackFrame &frame)
{
  VMObject o = Pop(frame);
  fputs(as_string(o).c_str(), stdout);
}

void PrintLine(StackFrame &frame)
{
  VMObject o = Pop(frame);
  puts(as_string(o).c_str());
}

void PushVariable(StackFrame &frame, const VMObject &o)
{
  int variable_id = o.value.integer;
  Push(frame, frame.GetVariable(variable_id));
}

void Push(StackFrame &frame, const VMObject &o)
{
  frame.Push(o);
}

VMObject Pop(StackFrame &frame)
{
  return frame.Pop();
}
