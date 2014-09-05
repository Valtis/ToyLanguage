#pragma once
class VM;
class StackFrame;
class MemoryManager;
struct VMObject;

void Add(StackFrame &frame);
void Sub(StackFrame &frame);
void Mul(StackFrame &frame);
void Div(StackFrame &frame);

void JumpIfTrue(StackFrame &frame, const VMObject &location);
void JumpIfFalse(StackFrame &frame, const VMObject &location);
void Jump(StackFrame &frame, const VMObject &location);

void Equals(StackFrame &frame);

void CallFunction(VM *vm, StackFrame &frame);
void Return(VM *vm, StackFrame &frame);

void Print(StackFrame &frame); 
void PrintLine(StackFrame &frame);

void AllocatePtr(StackFrame &frame, MemoryManager &manager);
void WritePtr(StackFrame &frame, MemoryManager &manager);
void ReadPtr(StackFrame &frame, MemoryManager &manager);

void Push(StackFrame &frame, const VMObject &o);
void PushVariable(StackFrame &frame, const VMObject &o);
VMObject Pop(StackFrame &frame);