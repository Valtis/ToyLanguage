#pragma once
#include <stdexcept>

class MemoryError : public std::runtime_error
{
public:

  MemoryError(const std::string &what) : std::runtime_error(what.c_str())
  {

  }
private:

};


#define MEMORY_ERROR(NAME__) class NAME__ : public MemoryError { \
public: \
NAME__(const std::string &what) : MemoryError(what) {} \
}; 


MEMORY_ERROR(HeapGrowthError)
MEMORY_ERROR(OutOfMemoryError)
MEMORY_ERROR(HeapSizeExeedsLimitError)
MEMORY_ERROR(NotAPointerError)
MEMORY_ERROR(NullPointerError)
MEMORY_ERROR(MemoryBoundaryError)