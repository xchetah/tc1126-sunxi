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

MODULE_INFO(intree, "Y");

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc191ecac, "module_layout" },
	{ 0xf9a482f9, "msleep" },
	{ 0x17a142df, "__copy_from_user" },
	{ 0x98082893, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xde1a082b, "i2c_del_driver" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xc010d246, "malloc_sizes" },
	{ 0x15d0cf55, "hrtimer_cancel" },
	{ 0xcb5f3315, "i2c_transfer" },
	{ 0x20000329, "simple_strtoul" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x2346492d, "remove_proc_entry" },
	{ 0x33543801, "queue_work" },
	{ 0x432fd7f6, "__gpio_set_value" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x85df9b6c, "strsep" },
	{ 0xfbf9cde, "input_free_platform_resource" },
	{ 0x91715312, "sprintf" },
	{ 0xa25f3e77, "input_set_abs_params" },
	{ 0x5f22aece, "input_event" },
	{ 0xa7ecf324, "__init_waitqueue_head" },
	{ 0xe707d823, "__aeabi_uidiv" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x5f754e5a, "memset" },
	{ 0x11089ac7, "_ctype" },
	{ 0x7ab27c76, "dev_err" },
	{ 0x27e1a049, "printk" },
	{ 0x84b183ae, "strncmp" },
	{ 0x73e20c1c, "strlcpy" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xca7ce104, "input_mt_init_slots" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x59bdc133, "input_fetch_sysconfig_para" },
	{ 0x42160169, "flush_workqueue" },
	{ 0x2196324, "__aeabi_idiv" },
	{ 0x1948841a, "i2c_register_driver" },
	{ 0xd16733dd, "input_init_platform_resource" },
	{ 0x11f447ce, "__gpio_to_irq" },
	{ 0x5986d09f, "input_register_device" },
	{ 0x1000e51, "schedule" },
	{ 0xaa41b054, "hrtimer_start" },
	{ 0xb88a659, "input_free_device" },
	{ 0x143e35b4, "kmem_cache_alloc_trace" },
	{  0xf1338, "__wake_up" },
	{ 0x4f68e5c9, "do_gettimeofday" },
	{ 0xf4bcd4e2, "proc_create_data" },
	{ 0xfcec0987, "enable_irq" },
	{ 0x37a0cba, "kfree" },
	{ 0x9d669763, "memcpy" },
	{ 0x965e3c13, "input_unregister_device" },
	{ 0x69ff5332, "prepare_to_wait" },
	{ 0x820e70f3, "hrtimer_init" },
	{ 0xc0bfc2e6, "input_mt_report_slot_state" },
	{ 0xbc3d21af, "finish_wait" },
	{ 0x676bbc0f, "_set_bit" },
	{ 0xf20dabd8, "free_irq" },
	{ 0x22fb7dd2, "input_allocate_device" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("i2c:tc1126");
MODULE_ALIAS("i2c:cn1100");

MODULE_INFO(srcversion, "054D041ABC5E6D31513D2E4");
