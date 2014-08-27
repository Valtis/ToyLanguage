#include "ConversionFunctions.h"

std::string as_string(const VMObject &o)
{
  switch (o.type)
  { 
  case VMObjectType::NUMBER:
    return std::to_string(o.value.number);

  case VMObjectType::NONE:
  default:
    return "";


  }
}


