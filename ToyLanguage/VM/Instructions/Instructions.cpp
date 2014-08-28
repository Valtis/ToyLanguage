#include "Instructions.h"
#include "ConversionFunctions.h"
#include "..\StackFrame.h"
#include "..\VMObject.h"
#include "..\VM.h"

#include <functional>
#include <cstdio>
void TwoOperandOperationWithResult(StackFrame &frame, VMObject(*operation)(const VMObject &, const VMObject &))
{ 

  VMObject second = Pop(frame);
  VMObject first = Pop(frame);
  
  Push(frame, operation(first, second));
}

void Add(StackFrame &frame)
{
  TwoOperandOperationWithResult(frame, [](const VMObject &first, const VMObject &second) { 
    VMObject o = first;
    o.value.number += second.value.number;
    return o;
  });
}

void Sub(StackFrame &frame)
{
  TwoOperandOperationWithResult(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number -= second.value.number;
    return o;
  });
}

void Mul(StackFrame &frame)
{
  TwoOperandOperationWithResult(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number *= second.value.number;
    return o;
  });
}

void Div(StackFrame &frame)
{
  TwoOperandOperationWithResult(frame, [](const VMObject &first, const VMObject &second) {
    VMObject o = first;
    o.value.number /= second.value.number;
    return o;
  });
}

void CallFunction(VM *vm, StackFrame &frame)
{
  VMObject new_function_id = Pop(frame);
  StackFrame new_frame(new_function_id.value.integer);
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

void Push(StackFrame &frame, const VMObject &o)
{
  frame.Push(o);
}

VMObject Pop(StackFrame &frame)
{
  return frame.Pop();
}
