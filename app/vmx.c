#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "cpu.h"
#include "kernelmod.h"
#include "log.h"
#include "cpu.h"

// cpuid
#define CPUID_IDX_FEATURE_FLAGS         (1)
#define CPUID_FEATURE_FLAGS_MASK_VMX    (0x20)
#define CPUID_FEATURE_FLAGS_BIT_VMX     (5)

// cr4
#define CR4_MASK_VMXE                   (0x2000)
#define CR4_BIT_VMXE                    (13)

#define VMX_DEV_FILE_PATH   "/dev/vmm0"

static int s_fd;
static uint8_t s_rev_id[1024];

int open_vmx_mod(void)
{
    TRACE_LOG("%s in.", __FUNCTION__);
    s_fd = open(VMX_DEV_FILE_PATH, O_RDWR);
    DEBUG_LOG("s_fd:%d", s_fd);
    if (s_fd < 0) {
        ERROR_LOG("%s open failed", VMX_DEV_FILE_PATH);
        return -1;
    }

    return 0;
}

void close_vmx_mod(void)
{
    TRACE_LOG("%s in.", __FUNCTION__);
    close(s_fd);
}

int reset_vmx(void)
{
    int ret;
    int reg;
    VmmCtrl ctrl;

    TRACE_LOG("%s in.", __FUNCTION__);
    DEBUG_LOG("s_fd:%d", s_fd);

    ret = ioctl(s_fd, VMM_READ_CR4, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }
    reg = (ctrl.val & CR4_MASK_VMXE) >> CR4_BIT_VMXE;
    if (reg) {
        _vmxoff();
    }

    return 0;
}

int enable_vmx(void)
{
    int ret;
    int reg;
    VmmCtrl ctrl;

    TRACE_LOG("%s in.", __FUNCTION__);
    DEBUG_LOG("s_fd:%d", s_fd);

    // read current cr4 value
    ret = ioctl(s_fd, VMM_READ_CR4, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }

    // set VMX enable bit
    ctrl.val = ctrl.val | CR4_MASK_VMXE;
    ret = ioctl(s_fd, VMM_WRITE_CR4, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }

    return 0;
}

int setup_msr(void)
{
    int ret;
    VmmCtrl ctrl;

    // MSR 58(3A) のBIT2を1に設定

    ctrl.addr = IA32_FEATURE_CONTROL;
    ret = ioctl(s_fd, VMM_READ_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }
    DEBUG_LOG("read msr:0x%llx", ctrl.val);

    // unlock IA32_FEATURE_CONTROL
    ctrl.addr = IA32_FEATURE_CONTROL;
    ctrl.val = ctrl.val ^ MSR_MASK_LOCK_IA32_FEATURE_CONTROL;
    DEBUG_LOG("write val:0x%llx", ctrl.val);
    ret = ioctl(s_fd, VMM_WRITE_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }

    ctrl.addr = IA32_FEATURE_CONTROL;
    ret = ioctl(s_fd, VMM_READ_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }
    DEBUG_LOG("read msr:0x%llx", ctrl.val);

    // Enable VMX outside SMX operation bit:2
    ctrl.addr = IA32_FEATURE_CONTROL;
    ctrl.val = ctrl.val | MSR_MASK_ENABLE_VMX_OUTSIDE_SMX;
    ret = ioctl(s_fd, VMM_WRITE_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }

    return 0;

    // Lock IA32_FEATURE_CONTROL
    ctrl.addr = IA32_FEATURE_CONTROL;
    ctrl.val = ctrl.val | MSR_MASK_LOCK_IA32_FEATURE_CONTROL;
    ret = ioctl(s_fd, VMM_WRITE_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }

    ctrl.addr = IA32_FEATURE_CONTROL;
    ret = ioctl(s_fd, VMM_READ_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }
    DEBUG_LOG("read msr:0x%llx", ctrl.val);

    return 0;
}
int is_vmx_supported(void)
{
    int reg = _cpuid_ecx(CPUID_IDX_FEATURE_FLAGS);
    return (reg & CPUID_FEATURE_FLAGS_MASK_VMX) >> CPUID_FEATURE_FLAGS_BIT_VMX;
}

int get_vmcs_rev_id(void)
{
    int ret;
    VmmCtrl ctrl;
    TRACE_LOG("%s in.", __FUNCTION__);
    DEBUG_LOG("s_fd:%d", s_fd);

    ctrl.addr = MSR_REG_ADDR_IA32_VMX_BASIC;
    ret = ioctl(s_fd, VMM_READ_MSR, &ctrl);
    if (ret != 0) {
        ERROR_LOG("ioctl failed:[%d]", ret);
        return ret;
    }
    DEBUG_LOG("basic:0x%llx", ctrl.val);
    return 0;
}
