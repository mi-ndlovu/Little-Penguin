// SPDX-License-Identifier: GPL-2.0+
#include <linux/module.h>   /* Needed by all modules */
#include <linux/kernel.h>   /* Needed for KERN_INFO */
#include <linux/init.h>     /* Needed for the macros */

MODULE_LICENSE("GPL");             /* This affects runtime behavior */
MODULE_AUTHOR("Mbongeni Ndlovu, <mndlovu@student.wethinkcode.co.za>");
MODULE_DESCRIPTION("A simple Hello world!");
MODULE_VERSION("0.1");

static int __init hello_init(void)
{
	printk(KERN_INFO "Hello world !\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
