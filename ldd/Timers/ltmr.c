#include<linux/init.h>
#include<linux/module.h>
#include<linux/container_of.h>
#include<linux/fs.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/timer.h>
#include<linux/jiffies.h>
#include<linux/types.h>
#include<linux/hrtimer_types.h>
#include"ioctl.h"

#define LPTMR 0
#define HPTMR 1

int ch = LPTMR;
int count = 0;
uint ivl = 500;

struct my_device
{
	int cnt;
	dev_t device_id;
	struct cdev cdev;
	struct class *classes;
	struct device *devices;
	struct timer_list device_timer;
	struct hrtimer hrt;
	ktime_t interval;
};

static struct my_device tdev;


struct session
{
	struct my_device *dev_owner;
	size_t length;
	char buf[64];
};

static int my_open(struct inode *inode, struct file *file)
{
	struct session *s;
	struct my_device *d;

	d = container_of(inode->i_cdev, struct my_device, cdev);

	s = kzalloc(sizeof(*s), GFP_KERNEL);
	s->dev_owner = d;
	s->length = snprintf(s->buf, sizeof(s->buf),
		     "Hello from kernel\n");
	file->private_data = s;

	return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	struct session *s = file->private_data;
	size_t available, wanted, pos, copied;

	if (*ppos >= s->length)
		return 0;   
	if(!count)
	{
		return -EINVAL;
	}
	

	pos = (size_t)*ppos;
	available = s->length - pos;
	wanted = min(available, count);
	copied = wanted - copy_to_user(buf, s->buf + pos, wanted);
	*ppos += copied;

	return copied;
}

static ssize_t my_write(struct file* file, const char __user* buf, size_t count, loff_t* ppos)
{
	struct session *s = file->private_data;
	size_t available, pos, wanted, copied;
	u8 temp[64];
	if(file->f_flags & O_APPEND)
	{
		return -EINVAL;
	}
	if(!count)
	{
		return -EINVAL;
	}
	if(*ppos >= 64)
	{
		return 0;
	}
	
	pos = (size_t)*ppos;
	available = s->length -pos;
	wanted = s->length -pos;
	copied = min(available, count);
	copied = wanted - copy_from_user(temp, buf, wanted);
	if(!copied)
	{
		return -EINVAL;
	}
	memcpy(s->buf+pos, temp, copied);
	*ppos = *ppos + copied;
	if(*ppos > s->length)
	{
		s->length = *ppos;
	}
	
	return copied;
		
}

static loff_t my_seek(struct file* file, loff_t offset, int whence)
{
	struct session *s = file->private_data;
	size_t max = 63, pos = (size_t)file->f_pos	;
	int base;
	
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
	
	if(offset < -base || offset > 64 || offset > max - pos)
	{
		return -EINVAL;
	}
	
	file->f_pos = offset + base;
	return file->f_pos;
}

static long my_ioctl(struct file* file, unsigned int cmd, unsigned long arg)
{
	struct session* s = file->private_data;
	u32 dtcp, len, cnt;
	switch(cmd)
	{
		case GET_AVA_SPACE:	
				dtcp = 64 - READ_ONCE(s->length);
				if(copy_to_user((u32 __user*) arg, &dtcp, sizeof(dtcp)) )
				{
					return -EFAULT;
				}
				break;
		case GET_LEN:
				len = READ_ONCE(s->length);
				if( copy_to_user((u32 __user*)arg, &len, sizeof(len)) )
				{
					return -EFAULT;
				}	
				break;
		case GET_CNT:
				cnt = READ_ONCE(s->dev_owner->cnt);
				if( copy_to_user((u32 __user*)arg, &cnt, sizeof(cnt)) )
				{
					return -EFAULT;
				}	
				break;
		case SET_CNT:
				if(copy_from_user((int*)&s->dev_owner->cnt, (int*)&arg, sizeof(arg)))		
				{
					return -EFAULT;
				}
				if(ch == LPTMR)
				mod_timer(&tdev.device_timer, jiffies + msecs_to_jiffies(500));
				else
				hrtimer_start(&tdev.hrt, tdev.interval, HRTIMER_MODE_REL);
				
				break;
		case SET_INTERVAL:
				if(copy_from_user((ktime_t*)&s->dev_owner->interval, (ktime_t*)&arg, sizeof(arg)))		
				{
					return -EFAULT;
				}
				hrtimer_start(&tdev.hrt, tdev.interval, HRTIMER_MODE_REL);
				break;
																												
				
		default:
				return -EINVAL;
	}
	
	return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
	kfree(file->private_data);
	return 0;
}

static void timer_callback(struct timer_list *t)
{
	struct my_device *my_dev = container_of(t, struct my_device, device_timer);
	my_dev->cnt++;
	mod_timer(&my_dev->device_timer, jiffies + msecs_to_jiffies(500)); /* re-arm */
}

static enum hrtimer_restart hrtimer_callback(struct hrtimer* htm)
{
	struct my_device* dev = container_of(htm, struct my_device, hrt);
	dev->cnt++;
	hrtimer_forward_now(htm, dev->interval);
	return HRTIMER_RESTART;
}

static struct file_operations device_operations = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
	.unlocked_ioctl = my_ioctl,
	.write = my_write,
	.release = my_release,
	.llseek = my_seek,
};


static int __init my_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&tdev.device_id, 0, 1, "Character_Timer_Device");
	if (ret < 0)
		return ret;

	cdev_init(&tdev.cdev, &device_operations);
	tdev.cdev.owner = THIS_MODULE;
	cdev_add(&tdev.cdev, tdev.device_id, 1);

	tdev.classes = class_create("My_Class");
	tdev.devices = device_create(tdev.classes, NULL, tdev.device_id, &tdev,
				     "Character_Timer_Device");

	pr_info("MAJOR: %d MINOR: %d\n", MAJOR(tdev.device_id), MINOR(tdev.device_id));
	
	tdev.cnt = count;
	tdev.interval =(ktime_t) ivl;
	switch(ch)
	{
		case LPTMR:
			timer_setup(&tdev.device_timer, timer_callback, 0);
			mod_timer(&tdev.device_timer, jiffies + msecs_to_jiffies(500));
			break;
		case HPTMR:
			hrtimer_init(&tdev.hrt, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
			tdev.hrt.function = &hrtimer_callback;
			hrtimer_start(&tdev.hrt, tdev.interval, HRTIMER_MODE_REL);
			break;
		default: 
			return -EINVAL;
	}
			
	return 0;
}

static void __exit my_exit(void)
{
	if(ch == LPTMR)
	del_timer_sync(&tdev.device_timer);
	else
	hrtimer_cancel(&tdev.hrt);
	
	device_destroy(tdev.classes, tdev.device_id);
	class_destroy(tdev.classes);
	cdev_del(&tdev.cdev);
	unregister_chrdev_region(tdev.device_id, 1);
}

module_param(ch, int, 0444);
module_param(count, int, 0444);
module_param(ivl, uint, 0444);

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SID");
