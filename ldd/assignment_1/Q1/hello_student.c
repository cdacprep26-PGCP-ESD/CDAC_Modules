#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/moduleparam.h>
#include<linux/errno.h>

static char *student_name = "World";

static int __init hello_student_init(void)
{
    pr_info("Hello %s, Welcome to the Linux Programming. Now your Life will be HELL\n",student_name);
    
    return 0;
}

static void __exit hello_student_exit(void)
{
    pr_info("Bye linux world\n");
}


module_param(student_name, charp, 0444);
module_init(hello_student_init);
module_exit(hello_student_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BASIC linux kernel module");
