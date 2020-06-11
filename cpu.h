#ifndef _CPU_H_
#define _CPU_H_

// ============================================================================
// extern functions
// ============================================================================
extern int _vmxon(void);
extern int _vmxoff(void);

extern unsigned long _read_cr0(void);
extern unsigned long _read_cr4(void);
extern void _write_cr0(unsigned long);
extern void _write_cr4(unsigned long);

extern unsigned long _read_msr(unsigned int);

extern int _cpuid_eax(unsigned int);
extern int _cpuid_ebx(unsigned int);
extern int _cpuid_ecx(unsigned int);
extern int _cpuid_edx(unsigned int);

#endif	// _CPU_H_