#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "cpu.h"
#include "cpuinfo.h"
#include "kernelmod.h"

int main(int argc, char **argv){
    bool bRet;
    int fd;
    int ret;
    unsigned int reg;
	VmmCtrl ctrl;

    bRet = is_vmx_enable();
    if (bRet != true) {
        fprintf(stderr, "VMX is not supported\n");
        close(fd);
        return -1;
    }

    fd = open("/dev/vmm0", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "/dev/vmm0 open failed\n");
        return -1;
    }

    ret = ioctl(fd, VMM_READ_CR4, &ctrl);
    if (ret != 0) {
        fprintf(stderr, "ret:[%d]\n", ret);
        close(fd);
        return -1;
    }
    reg = ctrl.val;

    // BIT13 VMXE
    ctrl.val = reg | 0x2000;
    ret = ioctl(fd, VMM_WRITE_CR4, &ctrl);
    if (ret != 0) {
        fprintf(stderr, "ret:[%d]\n", ret);
        close(fd);
        return -1;
    }

    fprintf(stdout, "vmxoff");
	ctrl.val = 0xFF;
	ret = ioctl(fd, VMM_VMXOFF, &ctrl);
	if (ret != 0) {
		fprintf(stderr, "ret:[%d]\n", ret);
		close(fd);
		return -1;
	}
	fprintf(stdout, "ret:[0x16%llx]\n", ctrl.val);

    close(fd);
    return 0;
}