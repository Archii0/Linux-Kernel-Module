#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#define MYMAJOR 90
#define DRIVER_NAME "dummy_driver"
#define DRIVER_CLASS "MyModuleClass"
/* Metadata */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Arch");
MODULE_DESCRIPTION("In progress");

/* Buffer for data */
static char buffer[255];
static size_t buffer_pointer;

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count,
                           loff_t *offs) {
    int to_copy, not_copied, delta;

    /* Get amount of data to copy */
    to_copy = min(count, buffer_pointer);

    /* Copy data to user */
    not_copied = copy_to_user(user_buffer, buffer, to_copy);

    /* Calculate delta */
    delta = to_copy - not_copied;

    return delta;
}

/**
 * @brief Write data to the buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer,
                            size_t count, loff_t *offs) {
    int to_copy, not_copied, delta;

    /* Get amount of data to copy */
    to_copy = min(count, sizeof(buffer));

    /* Copy data to user */
    not_copied = copy_from_user(buffer, user_buffer, to_copy);
    buffer_pointer = to_copy;

    /* Calculate delta */
    delta = to_copy - not_copied;

    return delta;
}

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

static struct file_operations fops = {.owner = THIS_MODULE,
                                      .open = driver_open,
                                      .release = driver_close,
                                      .read = driver_read,
                                      .write = driver_write};
/**
 * @brief This function is called when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
    int retval;
    printk("Hello, Kernel!\n");

    /* Allocate a device nr */
    if (alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
        printk("Device nr could not be allocated\n");
        return -1;
    }
    printk("read_write - Device nr, Major: %d. Minor: %d was registered\n",
           my_device_nr >> 20, my_device_nr & 0xfffff);

    /* Create device class */
    if ((my_class = class_create(DRIVER_CLASS)) == NULL) {
        printk("Device class could not be created\n");
        goto ClassError;
    }

    /* Create device file */
    if (device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) ==
        NULL) {
        printk("Device file could not be created\n");
        goto FileError;
    }

    /* Initialise device file */
    cdev_init(&my_device, &fops);

    /* Registering device to kernel */
    if (cdev_add(&my_device, my_device_nr, 1) == -1) {
        printk("Registering of device to kernel failed\n");
        goto AddError;
    }

    return 0; // loading successful
AddError:
    device_destroy(my_class, my_device_nr);
FileError:
    class_destroy(my_class);
ClassError:
    unregister_chrdev_region(my_device_nr, DRIVER_NAME);
    return -1;
}

/**
 * @brief This function is called when the module is re,oved from the kernel
 */
static void __exit ModuleExit(void) {
    cdev_del(&my_device);
    device_destroy(my_class, my_device_nr);
    class_destroy(my_class);
    unregister_chrdev_region(my_device_nr, DRIVER_NAME);
    printk("Goodbye, Kernel!\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
