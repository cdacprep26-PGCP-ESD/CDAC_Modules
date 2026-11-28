#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x4f23b8cf, "cdev_add" },
	{ 0xcfc2eaec, "class_create" },
	{ 0x33b4784d, "device_create" },
	{ 0x122c3a7e, "_printk" },
	{ 0xea82d349, "hrtimer_init" },
	{ 0xc0b7c197, "hrtimer_start_range_ns" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x82ee90dc, "timer_delete_sync" },
	{ 0xc568eb41, "device_destroy" },
	{ 0xa3bd766f, "class_destroy" },
	{ 0xd224de25, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x102fe6de, "hrtimer_cancel" },
	{ 0x69acdf38, "memcpy" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x4c03a563, "random_kmalloc_seed" },
	{ 0x898cbdaa, "kmalloc_caches" },
	{ 0x4756e26b, "kmalloc_trace" },
	{ 0x65487097, "__x86_indirect_thunk_rax" },
	{ 0x135bb7ec, "hrtimer_forward" },
	{ 0x68c4768a, "param_ops_uint" },
	{ 0xd5049340, "param_ops_int" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x15ba50a6, "jiffies" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x37a0cba, "kfree" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfc4790dc, "cdev_init" },
	{ 0xaa8994e1, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7CD115F0D88838816D5B936");
