#include "StackFrame.h"


StackFrame::StackFrame(int functionID) : m_functionID(functionID), m_instruction_pointer(0)
{
}


StackFrame::~StackFrame()
{
}
