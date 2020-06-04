#ifndef _KERNELMOD_H_
#define _KERNELMOD_H_
#include <linux/ioctl.h>

// for ioctl
typedef struct _VmmCtrl{
    unsigned long long val;
} VmmCtrl;

// dir:2bit, size:14bit, type:8bit, nr:8bit
#define IOC_MAGIC 'v'
#define VMM_READ_CR0    _IOR(IOC_MAGIC, 0x00, VmmCtrl)
#define VMM_READ_CR4    _IOR(IOC_MAGIC, 0x04, VmmCtrl)
#define VMM_WRITE_CR0   _IOW(IOC_MAGIC, 0x10, VmmCtrl)
#define VMM_WRITE_CR4   _IOW(IOC_MAGIC, 0x14, VmmCtrl)

#endif	// _KERNELMOD_H_
