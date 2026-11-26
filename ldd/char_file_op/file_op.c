#include<linux/module.h>	//MOdule entry points and parameters
#include<linux/fs.h>	//File objs
#include<linux/cdev.h>	//struct cdev, cdev_init(), cdev_add()
#include<linux/device.h>	//class_create, device_create
#include<linux/err.h>	//IS_ERR, PT_ERR
#include<linux/kernel.h>
//#include<linux/container_of.h>	//Recover device object address
#include<linux/slab.h>	//Allocate free memory
#include<linux/uaccess.h>	//copy across the userspace
#include<linux/ioctl.h>
#include<linux/types.h>


#define DEVICE_NAME "beginner"
#define CLASS_NAME "beginner_class"
#define BEGINNER_GET_LENGTH _IOR('B',1,u32) 

#define CAPACITY 1024    //maximum bytes in one session

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
	struct session *s;  //This will  point to open alloction
	struct demo_device *dev;
	
	static const char initial[] = "Hello from kernel\n";
	
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
		return -ENOMEM;
	}
	
	memcpy(s->data, initial, sizeof(initial)-1);
	s->length = sizeof(initial) -1;
	s->owner = dev;
	file->private_data = s;
	
	return 0;
}

static int my_release(struct inode* inode, struct file* file)
{
	kfree(file->private_data);
	file->private_data = NULL;
	return 0;
}

static ssize_t my_read(struct file * file, char __user *buf, size_t count, loff_t *ppos)
{
	struct session * s = file->private_data;
	size_t pos, available, wanted, copied;
	
	if(!count)
		return 0;
	if(*ppos >= s->length)
		return 0;
	if(*ppos < 0)
		return -EINVAL;
	
	pos = (size_t)*ppos;
	available = s->length - pos;
	wanted = min(count, available);
	copied = wanted - copy_to_user(buf, s->data+pos, wanted); // copy_to_user returns bytes not copied to the user space
	
	if(!copied)
		return -EFAULT;
	*ppos +=copied;
	
	return copied;	
		
}
static ssize_t my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{

	struct session *s = file->private_data;
	u8 temporary[CAPACITY];
	size_t pos, available, wanted, copied;
	
	if(file->f_flags & O_APPEND)
		return -EINVAL;
	if(!count)
		return 0;
	if(*ppos >= CAPACITY)
		return -ENOSPC;
	pos = (size_t)*ppos;
	available = CAPACITY - pos;
	wanted = min(count, available);
	copied = wanted - copy_from_user(temporary, buf, wanted);
	
	if(!copied)
		return -EFAULT;
	memcpy(s->data+pos, temporary, copied);
	*ppos += copied;
	if(*ppos > s->length)
	{
		s->length = *ppos;
	}
	
	return copied;
	
}

static loff_t my_seek(struct file *file, loff_t offset, int whence)
{
	struct session *s = file->private_data;
	loff_t max_pos = CAPACITY-1;
	loff_t base;
	loff_t new_pos;
	
	switch(whence)
	{
		case SEEK_SET:
			base = 0;
			break;
		case SEEK_CUR:
			base = file->f_pos;
			break;
		case SEEK_END:
			base = s->length;
			break;
		default:
			return -EINVAL;
	}
	
	if(offset < -base || offset >max_pos - base)
		return -EINVAL;
		
	new_pos = base + offset;
	file->f_pos = new_pos;
	return new_pos;
}

static long my_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
	struct session *s;
	s = file->private_data;
	
	__u32 len;
	
	switch(cmd)
	{
		case BEGINNER_GET_LENGTH:
				len = READ_ONCE(s->length);
				
				if(copy_to_user((u32 __user*)arg, &len, sizeof(len)))
				{
								return -EFAULT;
				}
								break;
								
		default:
			return -EINVAL;
	}
		return 0;					
}
	
		
static const struct file_operations demo_operation = {
	
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_release,
	.read = my_read,
	.write = my_write,
	.llseek = my_seek,
	.unlocked_ioctl = my_ioctl
};

static int __init my_init(void)
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
	cdev_init(&demo.cdev, &demo_operation);
	demo.cdev.owner = THIS_MODULE;
	
	/*Register the initialized character device with kernel's character device
	 subsystem and associates it with a range of device number */
	ret = cdev_add(&demo.cdev, demo.number, 1);	
	if(ret < 0)
	{
		goto err_numbers;
	}
	
	demo.class = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(demo.class))
	{
		ret = PTR_ERR(demo.class);
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



















	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
