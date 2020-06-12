#ifndef _CPU_H_
#define _CPU_H_

// ============================================================================
// function prototype
// ============================================================================
extern int _vmxon(void);
extern int _vmxoff(void);

extern int _cpuid_eax(unsigned int);
extern int _cpuid_ebx(unsigned int);
extern int _cpuid_ecx(unsigned int);
extern int _cpuid_edx(unsigned int);

#endif	// _CPU_H_