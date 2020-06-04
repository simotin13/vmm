#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>		// kmalloc,kfree
#include <linux/device.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>    // copy_from_user, copy_to_user

#include "vmm.h"

MODULE_LICENSE("Dual BSD/GPL");

// ============================================================================
// extern functions
// ============================================================================
extern void _read_cr4(unsigned long long *val);

// ============================================================================
// function prototype
// ============================================================================
static int vmm_init(void);
static void vmm_exit(void);
static int vmm_open(struct inode *inode, struct file *filp);
static int vmm_close(struct inode *inode, struct file *filep);
static ssize_t vmm_read(struct file *fp, char __user *buf, size_t count, loff_t *f_pos);
static long vmm_ioctl(struct file *filep, unsigned int cmd, unsigned long arg);

#define MINOR_BASE      (0)
#define MINOR_NUMBER    (1)
#define DRIVER_NAME     "vmm"

typedef struct _VmmDevFile{
    unsigned char buf[16];
} VmmDevFile;

// driver major number, dynamically allocated
static int vmm_major = 0;
static struct cdev vmm_cdev;
static struct class *dev_class = NULL;

struct file_operations devone_fops =
{
    .open    = vmm_open,
    .release = vmm_close,
    .read = vmm_read,
    .unlocked_ioctl = vmm_ioctl,
    .compat_ioctl   = vmm_ioctl,   // for 32-bit App
};


static int vmm_init(void)
{
    dev_t dev;
    int alloc_ret = 0;
    int cdev_err = 0;
    int i;

    printk(KERN_DEBUG "%s in.", __FUNCTION__);

    alloc_ret = alloc_chrdev_region(&dev, MINOR_BASE, MINOR_NUMBER, DRIVER_NAME);
    if (alloc_ret) {
        printk(KERN_ERR "alloc_chrdev_region failed:[%d]\n", alloc_ret);
        return -1;
    }

    vmm_major = MAJOR(dev);
    dev = MKDEV(vmm_major, MINOR_BASE);

    //  initialize system call hander
    cdev_init(&vmm_cdev, &devone_fops);
    vmm_cdev.owner = THIS_MODULE;

    cdev_err = cdev_add(&vmm_cdev, dev, MINOR_NUMBER);
    if (cdev_err) {
        printk(KERN_ERR  "cdev_add failed:[%d]\n", cdev_err);
        unregister_chrdev_region(dev, MINOR_NUMBER);
        return -1;
    }

    dev_class = class_create(THIS_MODULE, DRIVER_NAME);
    if (IS_ERR(dev_class)) {
        cdev_del(&vmm_cdev);
        unregister_chrdev_region(dev, MINOR_NUMBER);
    }

    // create device file
    for (i = MINOR_BASE; i < (MINOR_BASE + MINOR_NUMBER); i++) {
        device_create(dev_class, NULL, MKDEV(vmm_major, i), NULL, "vmm%d", i);
    }

    printk(KERN_DEBUG "%s driver(vmm_major %d) installed\n", DRIVER_NAME, vmm_major);

    return 0;
}

static void vmm_exit(void)
{
    int i;
    dev_t dev = MKDEV(vmm_major, MINOR_BASE);

    printk(KERN_DEBUG "%s in.", __FUNCTION__);

    // delete device files
    for (i = MINOR_BASE; i < (MINOR_BASE+ MINOR_NUMBER); i++) {
        device_destroy(dev_class, MKDEV(vmm_major, i));
    }

    // delete /sys/class/vmm
    class_destroy(dev_class);

    // delete device driver
    cdev_del(&vmm_cdev);

    unregister_chrdev_region(dev, MINOR_NUMBER);

    printk(KERN_DEBUG "%s removed.\n", DRIVER_NAME);

}

static int vmm_open(struct inode *inode, struct file *filp)
{
    VmmDevFile *vmmDevFile;

	printk(KERN_DEBUG "%s in.", __FUNCTION__);

	vmmDevFile = kmalloc(sizeof(VmmDevFile), GFP_KERNEL);
    if (vmmDevFile == NULL) {
        printk(KERN_ERR  "kmalloc failed.\n");
        return -ENOMEM;
    }

    memset(vmmDevFile, 0, sizeof(VmmDevFile));

    filp->private_data = vmmDevFile;
    return 0;
}

static int vmm_close(struct inode *inode, struct file *filep)
{
    printk(KERN_DEBUG "%s in.", __FUNCTION__);
    if (filep->private_data) {
        kfree(filep->private_data);
        filep->private_data = NULL;
    }
    return 0;
}

static ssize_t vmm_read(struct file *fp, char __user *buf, size_t count, loff_t *f_pos)
{
    int i;
    unsigned char val = 0xff;
    int retval;
    printk(KERN_DEBUG "%s in.", __FUNCTION__);

    for (i = 0 ; i < count ; i++) {
        retval = copy_to_user(&buf[i], &val, 1);
        if (retval != 0) {
            break;
        }
    }
    if (retval == 0) {
        retval = count;
    }

    return retval;
}

static long vmm_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
    static VmmCtrl vmmCtrl;
    int ret;
	unsigned long long val = 0;

    printk(KERN_DEBUG "%s in.", __FUNCTION__);

    memset(&vmmCtrl, 0, sizeof(VmmCtrl));
    if (copy_from_user(&vmmCtrl, (void __user *)arg, sizeof(VmmCtrl))) {
        return -EFAULT;
    }

    printk(KERN_DEBUG "cmd:[%d], reg:[0x%llX], val:[0x%llX]\n", cmd, vmmCtrl.reg, vmmCtrl.val);

    switch(cmd) {
    case VMM_READ_REG:
    	//val = read_cr0();
    	_read_cr4(&val);
        printk(KERN_DEBUG "cr4 val:[0x%llx]", val);
        vmmCtrl.val = val;
        ret = copy_to_user((void __user *)arg, &vmmCtrl, sizeof(VmmCtrl));
        if (ret != 0) {
            ret = -EFAULT;
        }
        break;
    }

    return ret;
}
module_init(vmm_init);
module_exit(vmm_exit);
