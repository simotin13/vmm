#ifndef _VMX_H_
#define _VMX_H_

extern int open_vmx_mod(void);
extern void close_vmx_mod(void);
extern int reset_vmx(void);
extern int enable_vmx(void);
extern int is_vmx_supported(void);
extern int get_cr4_VMXE(void);
extern int get_vmcs_rev_id(void);

#endif  // _VMX_H_