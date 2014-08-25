#include "Instructions.h"
#include "ConversionFunctions.h"
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


void Print(StackFrame &frame)
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
