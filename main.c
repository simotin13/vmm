#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "vmm.h"
#include <sys/ioctl.h>

int main(int argc, char **argv) {
	int fd;
	int ret;
	VmmCtrl ctrl;
	fd = open("/dev/vmm0", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	ctrl.reg = 0x01;
	ctrl.val = 0x02;
	ret = ioctl(fd, VMM_READ_REG, &ctrl);
	if (ret != 0) {
		fprintf(stderr, "ret:[%d]\n", ret);
		close(fd);
		return -1;
	}
	fprintf(stdout, "reg:[0x%llx], val:[0x%llx]\n", ctrl.reg, ctrl.val);

	close(fd);
	return 0;
}
