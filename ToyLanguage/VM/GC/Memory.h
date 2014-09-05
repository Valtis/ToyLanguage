#pragma once
#include <cstdint>
struct VMPointer;

class Memory
{
public:
  Memory(uint32_t maximum_allocation_size);

private:
  
  void VerifyHeapSizeLimit();
  void AllocateHeapMemory();
  void IncreaseHeapSize();


  uint8_t *m_memory;
  uint32_t m_allocation_size;
  const uint32_t m_maximum_allocation_size;
  uint32_t m_next_free_block;
  uint32_t m_block_size;
  double m_growth_factor;



};