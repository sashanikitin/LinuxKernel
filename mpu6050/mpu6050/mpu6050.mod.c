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
	{ 0xee626e98, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x35eb4d18, __VMLINUX_SYMBOL_STR(class_remove_file_ns) },
	{ 0xe56409ae, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0x8a1d3880, __VMLINUX_SYMBOL_STR(class_create_file_ns) },
	{ 0xf9b3f2c5, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0x8ffa0c19, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0xa65074b, __VMLINUX_SYMBOL_STR(i2c_smbus_write_byte_data) },
	{ 0xa87fe97d, __VMLINUX_SYMBOL_STR(i2c_smbus_read_byte_data) },
	{ 0x277a5de7, __VMLINUX_SYMBOL_STR(i2c_smbus_read_word_data) },
	{ 0x97255bdf, __VMLINUX_SYMBOL_STR(strlen) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x2196324, __VMLINUX_SYMBOL_STR(__aeabi_idiv) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:gl_mpu6050");

MODULE_INFO(srcversion, "1E84470484D0E6191A2FA70");
