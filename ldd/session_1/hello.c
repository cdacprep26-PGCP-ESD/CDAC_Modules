#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/moduleparam.h>
#include<linux/errno.h>

static char *name = "Student";
static int count = 1;

//All below function lokking things are actually Macros
static int __init hello_init(void)
{
    if(count > 7)
    {
        pr_err("Error\n");
    }
    for(int i = 1; i<=count; i++)
    {
        pr_info("%s\n",name);
    }
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Bye linux world\n");
}

/*
module_param(name, type, permission); 
	type = bool, int, short, uint, charp(character ptr)
	permission = S_IWUSR, S_IRUSR, S_IXUSR, S_IXUSR, S_IRGRP, S_IIWGRP
*/	
module_param(name, charp, 0444);
module_param(count, int, 0444);
module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BASIC linux kernel module");
