#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "log.h"
#include "cpu.h"
#include "vmx.h"

int main(int argc, char **argv){
    int ret;

    TRACE_LOG("vmm application started...");

    // check if vmx enable
    ret = check_cpu_regs();
    if (ret != 0) {
        ERROR_LOG("register check failed:[%d]");
        return -1;
    }

    // open vmx kernel module device file
    ret = open_vmx_mod();
    if (ret < 0) {
        ERROR_LOG("open_mod failed:[%d]", ret);
        return -1;
    }

    reset_vmx();

    get_vmcs_rev_id();

    enable_vmx();

    setup_msr();

    // close vmx kernel module device file
    close_vmx_mod();

    TRACE_LOG("vmm application finished...");
   return 0;
}