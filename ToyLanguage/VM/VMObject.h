#pragma once

enum class VMObjectType : int { NONE, NUMBER, INTEGER };

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
  } value;

  VMObjectType type;
};