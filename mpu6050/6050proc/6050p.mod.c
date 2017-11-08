#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
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
	{ 0x2bf17983, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xe56409ae, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0xfb0a347f, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0x77cef10c, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x4a4d716f, __VMLINUX_SYMBOL_STR(proc_create_data) },
	{ 0x191867dc, __VMLINUX_SYMBOL_STR(proc_mkdir) },
	{ 0x8ffa0c19, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xbce9d67a, __VMLINUX_SYMBOL_STR(remove_proc_entry) },
	{ 0x277a5de7, __VMLINUX_SYMBOL_STR(i2c_smbus_read_word_data) },
	{ 0xf4fa543b, __VMLINUX_SYMBOL_STR(arm_copy_to_user) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x2196324, __VMLINUX_SYMBOL_STR(__aeabi_idiv) },
	{ 0xa65074b, __VMLINUX_SYMBOL_STR(i2c_smbus_write_byte_data) },
	{ 0xa87fe97d, __VMLINUX_SYMBOL_STR(i2c_smbus_read_byte_data) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:gl_mpu6050");

MODULE_INFO(srcversion, "8E15D0202B750C7B8E12B91");
