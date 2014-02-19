#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xb1fd93d, "module_layout" },
	{ 0xc8739ca6, "cdev_del" },
	{ 0x2039258, "cdev_init" },
	{ 0xd0d8621b, "strlen" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xe57a3da4, "find_vpid" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x4aabc7c4, "__tracepoint_kmalloc" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x2de677ea, "current_task" },
	{ 0xb72397d5, "printk" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0x12c9f780, "pid_task" },
	{ 0x61651be, "strcat" },
	{ 0xebfb97aa, "cdev_add" },
	{ 0x5953e9a5, "init_task" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x37a0cba, "kfree" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "EDE0D9C21F57EAEFF03EE13");
