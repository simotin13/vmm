section .text

global _vmxon
global _vmxoff

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
