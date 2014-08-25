#pragma once
#include "..\StackFrame.h"

void Add(StackFrame &frame);
void Sub(StackFrame &frame);
void Mul(StackFrame &frame);
void Div(StackFrame &frame);

void Print(StackFrame &frame); 


void Push(StackFrame &frame, const VMObject &o);
VMObject Pop(StackFrame &frame);