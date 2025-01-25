#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#define MYMAJOR 90
/* Metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arch");
MODULE_DESCRIPTION("In progress");

/**
 * @brief This function is called when device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
    printk("Device file opened\n");
    return 0;
}

/**
 * @brief This function is called when device file is closed
 */
static int driver_close(struct inode *device_file, struct file *instance) {
    printk("Device file closed\n");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE, .open = driver_open, .release = driver_close};
/**
 * @brief This function is called when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
    int retval;
    printk("Hello, Kernel!\n");

    // unregister_chrdev(MYMAJOR, "my_dev_nr");

    retval = register_chrdev(MYMAJOR, "my_dev_nr", &fops);
    if (retval == 0) {
        printk("dev_nr - registered device number major: %d, minor: %d\n",
               MYMAJOR, 0);
    } else if (retval > 0) {
        printk("dev_nr - registered device number major: %d, minor: %d\n",
               retval >> 20, retval & 0xfffff);
    } else {
        printk("dev_nr - failed to register device number\n");
        return -1;
    }
    return 0; // loading successful
}

/**
 * @brief This function is called when the module is re,oved from the kernel
 */
static void __exit ModuleExit(void) {
    unregister_chrdev(MYMAJOR, "my_dev_nr");
    printk("Goodbye, Kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
