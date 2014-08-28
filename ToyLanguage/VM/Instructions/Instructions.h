#pragma once
class VM;
class StackFrame;
struct VMObject;

void Add(StackFrame &frame);
void Sub(StackFrame &frame);
void Mul(StackFrame &frame);
void Div(StackFrame &frame);


void CallFunction(VM *vm, StackFrame &frame);
void Return(VM *vm, StackFrame &frame);

void Print(StackFrame &frame); 
void PrintLine(StackFrame &frame);

void Push(StackFrame &frame, const VMObject &o);
void PushVariable(StackFrame &frame, const VMObject &o);
VMObject Pop(StackFrame &frame);