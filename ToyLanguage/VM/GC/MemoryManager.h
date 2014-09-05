#pragma once
#include <cstdint>
#include <list>

#include "Memory.h"
#include "..\VMObject.h"

struct VMPointer;

class MemoryManager
{
public:
  MemoryManager(uint32_t maximum_heap_size);

  VMObject Allocate(uint32_t size_in_bytes);

  void Write(VMObject pointer, uint32_t offset, VMObject value);

  void EnsureValidAddress(uint32_t address);

  void EnsureValidPointer(VMObject &pointer);

  VMObject Read(VMObject pointer, uint32_t offset);

  void RunGc();

private:
  void VerifyHeapSizeLimit();
  void AllocateHeapMemory();
  void IncreaseHeapSize();

  void EnsureEnoughMemory(uint32_t size);


  uint8_t *m_memory;
  uint32_t m_heap_size;
  const uint32_t m_maximum_heap_size;
  uint32_t m_next_free_block;
  uint32_t m_block_size;
  double m_growth_factor;


  std::list<VMPointer *> m_allocated_pointers;
};