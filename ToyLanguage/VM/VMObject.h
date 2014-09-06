#pragma once
#include "GC/VMPointer.h"
#include <cstdint>
enum class VMObjectType : uint8_t { NONE, NUMBER, INTEGER, BOOLEAN, POINTER };


struct VMObject
{
  
  VMObject()
  {
    type = VMObjectType::NONE; 
  }
  
  union
  {
    double number;
    int integer;
    bool boolean;
    VMPointer *pointer;
  } value;

  VMObjectType type;
};