#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/err.h>


#define DEVICE_COUNT 4

static dev_t first_device;
static struct class *device_class;

static int __init my_init(void)
{
	struct device *devices;
	int ret;
	int i;
	
	ret = alloc_chrdev_region(&first_device, 0, DEVICE_COUNT, "first_device");
	
	if(ret < 0)
	{
		return ret;
	}
	
	device_class = class_create("first_class");
	
	if(IS_ERR (device_class))
	{
		ret = PTR_ERR(device_class);
		goto release_numbers;
	}
	
	for(i = 0; i < DEVICE_COUNT; i++)
	{
		devices = device_create(device_class, NULL, first_device+i, NULL, "device%d", i);
		
		if(IS_ERR(devices))
		{
			ret = PTR_ERR(devices);
			goto destroy_device;
		}
		
		pr_info("character_device MAJOR: %d MINOR %d \n", MAJOR(first_device+i), MINOR(first_device+i));
	}
	
	return 0;
	
	destroy_device:
		while(i>0)
		{
			i--;
			device_destroy(device_class, first_device+i);
		}
		class_destroy(device_class);

	release_numbers:
				unregister_chrdev_region(first_device, DEVICE_COUNT);
				return ret;

}


static void __exit my_exit(void)
{
	int i ;
	for(i = DEVICE_COUNT -1; i> 0; i--)
	{
		device_destroy(device_class, first_device+i);
	}
	
	class_destroy(device_class);
	unregister_chrdev_region(first_device, DEVICE_COUNT);
	pr_info("character device : cleanup commmmplete");	
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Multiple Char Devices");





















