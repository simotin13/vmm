#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "log.h"
#include "cpu.h"
#include "kernelmod.h"
#include "vmx.h"

int main(int argc, char **argv){
    int ret;

    // check if vmx enable
    ret = is_vmx_enable();
    if (ret == 0) {
        ERROR_LOG("VMX is not supported in this CPU...");
        return -1;
    }

    // open vmx kernel module device file
    ret = open_vmx_mod();
    if (ret < 0) {
        ERROR_LOG("open_mod failed:[%d]\n", ret);
        return -1;
    }

    reset_vmx();

    // close vmx kernel module device file
    close_vmx_mod();
    return 0;
}