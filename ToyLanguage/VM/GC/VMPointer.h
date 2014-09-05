#pragma once
#pragma once
#include <cstdint>

struct VMPointer
{
  uint32_t pointer;
  uint32_t size;
  bool marked;
};