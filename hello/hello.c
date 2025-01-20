#include <linux/init.h>
#include <linux/module.h>

int my_init(void) {
    printk("hello - Hello, Kernel!\n");
    return 0; // loading successful
}

void my_exit(void) { printk("hello - Goodbye, Kernel!\n"); }

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Arch");
// MODULE_DESCRIPTION("Hello, Kernel!");
