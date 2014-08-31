#pragma once

enum class VMObjectType : int { NONE, NUMBER, INTEGER, BOOLEAN };

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
  } value;

  VMObjectType type;
};