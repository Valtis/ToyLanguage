#pragma once

enum class VMObjectType : int { NONE, NUMBER };

struct VMObject
{
  
  VMObject()
  {
    type = VMObjectType::NONE; 
  }

  union
  {
    double number;

  } value;

  VMObjectType type;
};