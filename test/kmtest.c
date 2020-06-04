#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "kernelmod.h"

int main(int argc, char **argv) {
	int fd;
	int ret;
	VmmCtrl ctrl;
	fd = open("/dev/vmm0", O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	ctrl.val = 0xFF;
	ret = ioctl(fd, VMM_READ_CR0, &ctrl);
	if (ret != 0) {
		fprintf(stderr, "ret:[%d]\n", ret);
		close(fd);
		return -1;
	}
	fprintf(stdout, "cr0 val:[0x8%llx]\n", ctrl.val);

	ctrl.val = 0xFF;
	ret = ioctl(fd, VMM_READ_CR4, &ctrl);
	if (ret != 0) {
		fprintf(stderr, "ret:[%d]\n", ret);
		close(fd);
		return -1;
	}
	fprintf(stdout, "cr4 val:[0x8%llx]\n", ctrl.val);
	close(fd);
	return 0;
}
