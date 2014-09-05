#pragma once
#include "GC/VMPointer.h"
enum class VMObjectType : int { NONE, NUMBER, INTEGER, BOOLEAN, POINTER };


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