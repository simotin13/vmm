#ifndef _VMX_H_
#define _VMX_H_

extern int open_vmx_mod(void);
extern void close_vmx_mod(void);
extern int reset_vmx(void);
extern int is_vmx_enable(void);
extern int get_cr4_VMXE(void);

#endif  // _VMX_H_