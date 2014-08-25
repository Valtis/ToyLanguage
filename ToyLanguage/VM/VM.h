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
  
private:
  bool m_running = true;
  int m_frame_pointer;
  std::vector<StackFrame> m_frames;

  std::unordered_map<int, VMFunction> m_functions;
};