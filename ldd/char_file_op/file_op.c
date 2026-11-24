#include<linux/module.h>	//MOdule entry points and parameters
#include<linux/fs.h>	//File objs
#include<linux/cdev.h>	//struct cdev, cdev_init(), cdev_add()
#include<linux/device.h>	//class_create, device_create
#include<linux/err.h>	//IS_ERR, PT_ERR
#include<linux/container_of.h>	//Recover device object address
#include<linux/slab.h>	//Allocate free memory
#include<linux/uaccess.h>	//copy across the userspace


#define DEVICE_NAME "beginner"
#define CLASS_NAME "beginner_class"

#define CAPACITY 64    //maximum bytes in one session

static bool fail_open;	//Default value is false

struct demo_device
{
	dev_t number;	//Denotes major and minor numbers
	struct cdev cdev;	//Character device object
	struct class *class;	//Model class during init
	struct device *device;	//Model device during init
	bool fail_open;	//Test behaviour
};

static struct demo_device demo;

struct session
{
	struct demo_device *owner;	//Device associated with per open state
	size_t length;	//Valid data lenght
	u8 data[CAPACITY];	//Binary byte. Not a NULL terminated string
};

static int my_open(struct inode *inode, struct file* file)
{
	struct session *s  //This will  point to open alloction
	struct demo_device *dev;
	
	static const char inital[] = "Hello from kernel\n";
	
	dev = container_of(inode->i_cdev, struct demo_device, cdev);
	
	if(file->f_flags & (O_APPEND | O_TRUNC))
	{
		return -EINVAL;
	}
	
	if(dev->fail_open)
	{
		return -ENOMEM;
	}
	
	s = kzalloc(sizeof(*s), GFP_KERNEL); //allocate and zero
	if(!s)
	{
		reteurn -ENOMEM;
	}
	
	memcpy(s->data, initial, sizof(initial)-1);
	s->length = sizof(initial) -1;
	s->owner = dev;
	file->private_data = s;
	
	return 0;
}

static int my_relase(struct inode* inode, struct file* file)
{
	kfree(file->private_data);
	file->private_data = NULL;
	return 0;
}


static const struct file_operations demo_operation = {
	
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.write = my_write,
	.llseek = my_seek,
	.unlocked_ioctl = my_ioctl
};

statict int __init my_init(void)
{
	int ret;
	
	demo.fail_open = fail_open;	//Initalize callback-visible state
	
	//Allocate one device - Major and Minor numbers
	ret = alloc_chrdev_region(&demo.number, 0, 1, DEVICE_NAME);
	
	
	if(ret < 0)
	{
		return ret;
	}
	
	/*Initalize cdev structure and associate it with the drivers file operation structure. 
	It does not register the character device only initiates it */
	cdev_init(&demo.cdev, &demo_operations);
	demo.cdev.owner = THIS_MODULE;
	
	/*Register the initialized character device with kernel's character device
	 subsystem and associates it with a range of device number */
	ret = cdev_add(&demo.cdev, demo,number, 1);	
	if(ret < 0)
	{
		goto err_numbers;
	}
	
	demo.class = class_create(CLASS_NAME);
	if(IS_ERR(demo.class))
	{
		ret = PTR_ERR(demoe.class);
		goto err_cdev;
	}
	
	demo.device = device_create(demo.class, NULL, demo.number, &demo, DEVICE_NAME);
	
	if(IS_ERR(demo.device))
	{
		ret = PTR_ERR(demo.device);
		goto err_class;
	}
	
	pr_info("beginer register major %u and minor %u\n", MAJOR(demo.number), MINOR(demo.number));
	
	return 0;
	
err_class:
	class_destroy(demo.class);
	
err_cdev:
	cdev_del(&demo.cdev);
	
err_numbers:
	unregister_chrdev_region(demo.number, 1);
	return ret;
	
																																																				
}


static void __exit my_exit(void)
{

	device_destroy(demo.class, demo.number);
	class_destroy(demo.class);
	cdev_del(&demo.cdev);
	unregister_chrdev_region(demo.number, 1);


}


module_init(my_init);
module_exit(my_exit);


MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Manual cdev registration and file operation implementation");
MODULE_AUTHOR("Arati");



















	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
