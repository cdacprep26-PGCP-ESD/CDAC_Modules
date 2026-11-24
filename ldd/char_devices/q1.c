#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>

static int first_device;

static int __init my_init(void)
{
    int ret;
    int i;
    int current_device;
    ret = alloc_chrdev_region(&first_device, 5, 4, "My Second Char device");

    if(ret < 0)
    {
        return ret;
    }

    for(i = 0; i<4; i++)
    {
        current_device = first_device + i;
        pr_info("The MAJOR: %d MINOR: %d\n",MAJOR(current_device), MINOR(current_device));
    }

    return 0;
}

static void __exit my_exit(void)
{       
        unregister_chrdev_region(first_device, 4);
        pr_info("Character Device Released\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");