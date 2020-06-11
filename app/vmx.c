#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
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

static int s_fd;

#define VMX_DEV_FILE_PATH   "/dev/vmm0"

int open_vmx_mod(void)
{
    TRACE_LOG("%s in.", __FUNCTION__);
    int s_fd = open(VMX_DEV_FILE_PATH, O_RDWR);
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

int is_vmx_enable(void)
{
  int reg = _cpuid_ecx(CPUID_IDX_FEATURE_FLAGS);
  return (reg & CPUID_FEATURE_FLAGS_MASK_VMX) >> CPUID_FEATURE_FLAGS_BIT_VMX;
}
