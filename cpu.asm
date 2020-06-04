section .text

global _vmxon
global _vmxoff
global _read_cr0
global _read_cr4
global _write_cr4
global _write_cr0

_vmxon:
    vmxon [rdi]
    jbe _vmx_failure
    jmp _vmx_success

_vmxoff:
    vmxoff
    jbe _vmx_failure
    jmp _vmx_success

_vmx_failure:
    mov rax, 0x00
    ret

_vmx_success:
    mov rax, 0xFF
    ret

; =============================================================================
; Read/Write Controll Register
; =============================================================================
_read_cr0:
    mov rax, cr0
    ret

_read_cr4:
    mov rax, cr4
    ret

_write_cr0:
    mov cr0, rdi
    ret

_write_cr4:
    mov cr4, rdi
    ret