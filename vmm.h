#ifndef _VMM_H_
#define _VMM_H_
#include <linux/ioctl.h>

// for ioctl
typedef struct _VmmCtrl{
    unsigned long long reg;
    unsigned long long val;
} VmmCtrl;

#define IOC_MAGIC 'v'
#define VMM_READ_REG    _IOR(IOC_MAGIC, 1, VmmCtrl)
#define VMM_WRITE_REG   _IOR(IOC_MAGIC, 2, VmmCtrl)

#endif	// _VMM_H
