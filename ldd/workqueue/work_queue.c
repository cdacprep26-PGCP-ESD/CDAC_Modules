#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/workqueue.h>
#include<linux/delay.h>
#include<linux/sched.h>
#include<linux/smp.h>

static struct work_struct my_work;


static void work_handler(struct work_struct *work)
{
	pr_info("Work_handler has started\n");
	pr_info("Current_PID : %d, Name : %s, CPU : %d\n",current->pid,current->comm,raw_smp_processor_id());
	
	msleep(3000); 
	pr_info("Current_PID : %d, Name : %s, CPU : %d\n",current->pid,current->comm,raw_smp_processor_id());
	pr_info("Work_handler has completed\n");
}


static int __init my_init(void)
{
	pr_info("Initialization process has started\n");
	pr_info("current_PID : %d, Name : %s, CPU : %d\n",current->pid,current->comm,raw_smp_processor_id());
	
	
	//my_work.func = work_handler;
	INIT_WORK(&my_work,work_handler);
	schedule_work(&my_work);
	
	pr_info("Initialization has ended\n");
	
	return 0;
}

static void __exit my_exit(void)
{
	pr_info("Exit process initiated\n");
	cancel_work_sync(&my_work);
	pr_info("Exit process ended\n");
}


module_init(my_init);
module_exit(my_exit);

module_LICENSE("GPL");

