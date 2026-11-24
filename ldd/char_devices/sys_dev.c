#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/device.h>
#include<linux/err.h>

static dev_t device_number;
static struct class *device_class;

static int __init my_init(void){
    struct device *devices;
    int ret;

    ret = alloc_chrdev_region(&device_number,0,1,"mydevice");

    if(ret<0){
        return ret;
    }

    device_class = class_create("mydevice_class");

    if(IS_ERR(device_class))
    {
       ret = PTR_ERR(device_class);
       goto release_number; 
    }

    devices = device_create(device_class,NULL,device_number,NULL,"mydevice_0");

    if(IS_ERR(devices))
    {
        ret = PTR_ERR(devices);
        goto destroy_class;
    }

    pr_info("character_devices : Major : %u and Minor %u\n",MAJOR(device_number),MINOR(device_number));

    destroy_class:
        class_destroy(device_class);

    release_number:
        unregister_chrdev_region(device_number,1);
        return ret;

}

static int __exit my_exit(void){
    device_destroy(device_class,device_number);
    class_destroy(device_class);
    unregister_chrdev_region(device_number,1);
    pr_info("Character devices  : cleanup complete");

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC");
MODULE_DESCRIPTION("Classes and objects");