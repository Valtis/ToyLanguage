#include "MemoryManager.h"
#include "VMPointer.h"
#include "MemoryError.h"

#include <cstdlib>
#include <string>

#define DEFAULT_BLOCK_COUNT 1000
#define DEFAULT_BLOCK_SIZE 4096
#define DEFAULT_GROWTH_FACTOR 1.5

MemoryManager::MemoryManager(uint32_t maximum_heap_size) : m_memory(nullptr), m_heap_size(DEFAULT_BLOCK_SIZE*DEFAULT_BLOCK_COUNT),
m_maximum_heap_size(maximum_heap_size), m_next_free_block(0), m_block_size(DEFAULT_BLOCK_SIZE),
m_growth_factor(DEFAULT_GROWTH_FACTOR)
{
  VerifyHeapSizeLimit();
  AllocateHeapMemory();

}


void MemoryManager::VerifyHeapSizeLimit()
{
  if (m_heap_size > m_maximum_heap_size)
  {
    throw HeapSizeExeedsLimitError("Requested heap size exceeds limit");
  }
}


void MemoryManager::AllocateHeapMemory()
{
  uint8_t *new_memory = (uint8_t *)realloc(m_memory, m_heap_size);

  if (new_memory == nullptr)
  {
    throw HeapGrowthError("Could not allocate more memory for heap");
  }

  m_memory = new_memory;
}


void MemoryManager::IncreaseHeapSize()
{
  m_heap_size = (uint32_t)(m_heap_size * m_growth_factor);
  VerifyHeapSizeLimit();
  AllocateHeapMemory();
}


VMObject MemoryManager::Allocate(uint32_t size)
{
  if (size == 0)
  {
    VMObject o;
    o.type = VMObjectType::POINTER;
    o.value.pointer = nullptr;
    return o;
  }

  EnsureEnoughMemory(size);
  VMPointer *pointer = new VMPointer;
    
  pointer->pointer = m_next_free_block;
  pointer->size = size;
  pointer->marked = false;

  m_next_free_block += size;
  m_allocated_pointers.push_back(pointer);
  
  VMObject o;
  o.type = VMObjectType::POINTER;
  o.value.pointer = pointer;
  return o;
}


void MemoryManager::Write(VMObject pointer, uint32_t offset, VMObject value)
{
  EnsureValidPointer(pointer);

  uint32_t address = pointer.value.pointer->pointer + offset;
  
  EnsureValidAddress(address);

  memcpy(m_memory + address, &value, sizeof(VMObject));
}

VMObject MemoryManager::Read(VMObject pointer, uint32_t offset)
{
  EnsureValidPointer(pointer);

  uint32_t address = pointer.value.pointer->pointer + offset;

  EnsureValidAddress(address);

  VMObject value;
  memcpy(&value, m_memory + address, sizeof(VMObject));
  return value;
}




void MemoryManager::EnsureValidPointer(VMObject &pointer)
{
  if (pointer.type != VMObjectType::POINTER)
  {
    throw NotAPointerError("Attempting to access a non-pointer");
  }

  if (pointer.value.pointer == nullptr)
  {
      throw NullPointerError("Attempting to access memory through null pointer");
  }
}

void MemoryManager::EnsureValidAddress(uint32_t address)
{
  if (address > m_heap_size)
  {
    throw MemoryBoundaryError("Attempting to access memory outside heap boundary: Address " + std::to_string(address));
  }
}



void MemoryManager::EnsureEnoughMemory(uint32_t size)
{
  auto DoesNotFit = [](uint32_t next_block, uint32_t size, uint32_t allocation_size) { return next_block + size > allocation_size;  };

  if (DoesNotFit(m_next_free_block, size, m_heap_size))
  {
    RunGc();
    if (DoesNotFit(m_next_free_block, size, m_heap_size))
    {
      IncreaseHeapSize();
      if (DoesNotFit(m_next_free_block, size, m_heap_size))
      {
        throw OutOfMemoryError("Could not allocate requested " + std::to_string(size) + " bytes - out of memory");
      }
    }
  }
}



void MemoryManager::RunGc()
{
  // mark-sweep-compress heap
}