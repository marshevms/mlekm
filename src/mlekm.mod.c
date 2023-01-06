#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xf704969, "module_layout" },
	{ 0x4a165127, "kobject_put" },
	{ 0xfc4152fc, "ec_read" },
	{ 0xd731cdd9, "kmalloc_caches" },
	{ 0xeb233a45, "__kmalloc" },
	{ 0x54b1fac6, "__ubsan_handle_load_invalid_value" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x6fabae87, "kobject_create_and_add" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xb941650a, "firmware_kobj" },
	{ 0x6a6e05bf, "kstrtou8" },
	{ 0xed9d1a14, "sysfs_create_group" },
	{ 0x1a45cb6c, "acpi_disabled" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x7c797b6, "kmem_cache_alloc_trace" },
	{ 0x37a0cba, "kfree" },
	{ 0x7d628444, "memcpy_fromio" },
	{ 0xc708f1fe, "ec_write" },
	{ 0xf3ce59aa, "sysfs_create_file_ns" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "C5DE77D19F0AC86F30551D9");
