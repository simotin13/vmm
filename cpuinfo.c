#include <stdbool.h>
#include "cpu.h"

#define CPUID_IDX_FEATURE_FLAGS	(1)

#define FEATURE_FLAGS_MASK_VMX	(0x20)
#define FEATURE_FLAGS_BIT_VMX	(5)

bool is_vmx_enable(void)
{
  unsigned int reg = _cpuid_ecx(CPUID_IDX_FEATURE_FLAGS);
  return (reg & FEATURE_FLAGS_MASK_VMX) >> FEATURE_FLAGS_BIT_VMX;
}
