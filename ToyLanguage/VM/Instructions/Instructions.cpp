#include "Instructions.h"
#include "ConversionFunctions.h"
#include "../StackFrame.h"
#include "../VMObject.h"
#include "../VM.h"
#include "../VMError.h"
#include "../GC\MemoryManager.h"

#include <functional>
#include <cstdio>
void ArithmeticHelper(StackFrame &frame, VMObject(*operation)(const VMObject &, const VMObject &))
{

  VMObject second = Pop(frame);
  VMObject first = Pop(frame);

  if (second.type != VMObjectType::NUMBER || first.type != VMObjectType::NUMBER)
  {
    throw TypeError("Invalid operand for arithmetic operation: number expected");
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



void JumpIfTrue(StackFrame &frame, const VMObject &location)
{
  VMObject truth_value = Pop(frame);

  if (as_boolean(truth_value))
  {
    frame.SetInstruction(location.value.integer);
  }
}

void JumpIfFalse(StackFrame &frame, const VMObject &location)
{
  VMObject truth_value = Pop(frame);

  if (!as_boolean(truth_value))
  {
    frame.SetInstruction(location.value.integer);
  }
}



void Jump(StackFrame &frame, const VMObject &location)
{
  frame.SetInstruction(location.value.integer);
}

void IsNullPointer(StackFrame &frame)
{
  VMObject ptr = Pop(frame);
  VMObject o;
  o.type = VMObjectType::BOOLEAN;
  o.value.boolean = ptr.value.pointer == nullptr;
  Push(frame, o);
}

void Compare(StackFrame &frame)
{
  VMObject second = Pop(frame);
  VMObject first = Pop(frame);

  VMObject result;
  result.type = VMObjectType::BOOLEAN;
  if (first.type != second.type)
  {
    result.value.boolean = false;
  }
  else
  {
    switch (first.type)
    {
    case VMObjectType::NUMBER:
      // probably should do something like abs(a - b) < epsilon 
      result.value.boolean = first.value.number == second.value.number;
      break;
    case VMObjectType::INTEGER:
      result.value.boolean = first.value.integer == second.value.integer;
      break;
    case VMObjectType::BOOLEAN:
      result.value.boolean = first.value.boolean == second.value.boolean;
      break;
    }

  }

  frame.Push(result);
}

// does not consume the pointer the value is being compared to
void CompareNull(StackFrame &frame)
{
  VMObject first = Pop(frame);
  VMObject answer;
  answer.type = VMObjectType::BOOLEAN;
  answer.value.boolean = first.value.pointer == nullptr;
  
  Push(frame, first);
  Push(frame, answer);
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

void AllocatePtr(StackFrame &frame, MemoryManager &manager)
{

  VMObject size = Pop(frame);
  Push(frame, manager.Allocate(sizeof(VMObject)*size.value.integer));  
}

void WritePtr(StackFrame &frame, MemoryManager &manager)
{
  VMObject offset = Pop(frame);
  VMObject pointer = Pop(frame);
  VMObject value = Pop(frame);
  
  manager.Write(pointer, offset.value.integer*sizeof(VMObject), value);

  Push(frame, pointer);
}

void ReadPtr(StackFrame &frame, MemoryManager &manager)
{

  VMObject offset = Pop(frame);
  VMObject pointer = Pop(frame);
  Push(frame, pointer);
  Push(frame, manager.Read(pointer, offset.value.integer*sizeof(VMObject)));
}

void LoadVariable(StackFrame &frame, const VMObject &o)
{
  int variable_id = o.value.integer;
  Push(frame, frame.GetVariable(variable_id));
}

void StoreVariable(StackFrame &frame, const VMObject &o)
{
  int variable_id = o.value.integer;
  VMObject value = Pop(frame);
  frame.SetVariable(variable_id, value);
}


void Push(StackFrame &frame, const VMObject &o)
{
  frame.Push(o);
}

VMObject Pop(StackFrame &frame)
{
  return frame.Pop();
}

void Swap(StackFrame &frame)
{
  auto o1 = Pop(frame);
  auto o2 = Pop(frame);

  Push(frame, o1);
  Push(frame, o2);
}