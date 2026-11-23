#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>

#define DEVICE_COUNT 4

static dev_t first_number;

static int __init my_init(void)
{
    int ret;
    dev_t current_device;
    int i;
    ret = alloc_chrdev_region(&first_number, 0, DEVICE_COUNT, "firt_character_device");

    if(ret<0)
    {
        return ret;
    }

    for(i=0; i<DEVICE_COUNT; i++)
    {
        current_device = first_number + i;
        pr_info("Character_device: Major = %u and Minor = %u\n",MAJOR(current_device), MINOR(current_device));
    }
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev_region(first_number, DEVICE_COUNT);

    pr_info("Character_device: device number released\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");