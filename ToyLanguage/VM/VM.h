#pragma once

#include <vector>
#include <unordered_map>
#include "StackFrame.h"
#include "VMFunction.h"
class VM
{
public:

  void Initialize(std::unordered_map<int, VMFunction> code);
  void Execute();
  
  void PushFrame(StackFrame &frame)
  {
    m_frames.push_back(frame);
  }

  StackFrame PopFrame()
  {
    auto ret = m_frames.back();
    m_frames.pop_back();
    return ret;
  }

  StackFrame &CurrentFrame()
  {
    return m_frames.back();
  }

  VMFunction &Function(int id)
  {
    return m_functions[id];
  }

private:
  bool m_running = true;
  int m_frame_pointer;
  std::vector<StackFrame> m_frames;

  std::unordered_map<int, VMFunction> m_functions;
};