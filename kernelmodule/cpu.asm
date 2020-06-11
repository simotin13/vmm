section .text

global _vmxon
global _vmxoff
global _read_cr0
global _read_cr4
global _write_cr4
global _write_cr0

global _read_msr

global _cpuid_eax
global _cpuid_ebx
global _cpuid_ecx
global _cpuid_edx

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

_read_msr:
    push rcx
    mov ecx, edi
    rdmsr
    mov ebx, edx
    shl rbx, 32
    or rax, rbx
    pop rcx
    ret

; =============================================================================
; CPUID
; =============================================================================
_cpuid_eax:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    mov eax, edi
    cpuid
    ret

_cpuid_ebx:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    mov eax, edi
    cpuid
    mov eax, ebx
    ret

_cpuid_ecx:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    mov eax, edi
    cpuid
    mov eax, ecx
    ret

_cpuid_edx:
    xor rax, rax
    xor rbx, rbx
    xor rcx, rcx
    xor rdx, rdx
    mov eax, edi
    cpuid
    mov eax, edx
    ret
