#ifndef _CPU_H_
#define _CPU_H_

// ============================================================================
// extern functions
// ============================================================================
extern unsigned long _read_cr0(void);
extern unsigned long _read_cr4(void);
extern void _write_cr0(unsigned long);
extern void _write_cr4(unsigned long);

#endif	// _CPU_H_