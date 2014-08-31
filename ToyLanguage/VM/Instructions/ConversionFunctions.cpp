#include "ConversionFunctions.h"

std::string as_string(const VMObject &o)
{
  switch (o.type)
  { 
  case VMObjectType::NUMBER:
    return std::to_string(o.value.number);

  case VMObjectType::INTEGER:
    return std::to_string(o.value.integer);

  case VMObjectType::BOOLEAN:
    return o.value.boolean ? "true" : "false";

  case VMObjectType::NONE:
  default:
    return "";
  }
}

bool as_boolean(const VMObject &o)
{
  switch (o.type)
  {
  case VMObjectType::NUMBER:
    return o.value.number != 0;
  case VMObjectType::INTEGER:
    return o.value.integer != 0;
  case VMObjectType::BOOLEAN:
    return o.value.boolean;
  case VMObjectType::NONE:
  default:
    return false;
  }
}