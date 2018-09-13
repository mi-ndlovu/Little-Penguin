//SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <asm/uaccess.h>

MODULE_AUTHOR("Mbongeni Ndlovu");
MODULE_DESCRIPTION("debugfs driver\n");
MODULE_LICENSE("GPL");

struct semaphore my_sem;
char kbuffer[] = "debugfs driver is working.";
struct dentry *my_dirent;

static ssize_t debugfs_read(struct file *file, char *buf,
		size_t lbuf, loff_t *ppos)
{
	int stat = -1;

	if (down_interruptible(&my_sem) == 0) {
		stat = simple_read_from_buffer(buf,
				strlen(kbuffer), ppos, kbuffer,
				strlen(kbuffer));
		up(&my_sem);
	}
	return stat;
}

static ssize_t debugfs_write(struct file *file,
		const char *buf, size_t lbuf, loff_t *ppos)
{
	int stat = -1;

	if (lbuf > strlen(kbuffer)) {
		printk(KERN_INFO "Write: returning EINVAL\n");
		return -EINVAL;
	}
	if (down_interruptible(&my_sem) == 0) {
		stat = simple_write_to_buffer(kbuffer, strlen(buf),
				ppos, buf, lbuf);
		kbuffer[lbuf] = '\0';
		up(&my_sem);
	}
	return stat;
}
struct file_operations fops_debugfs = {
	.read = debugfs_read,
	.write = debugfs_write,
};

static int __init hello_init(void)
{
	sema_init(&my_sem, 1);
	/**
	 * Creates a folder with this path :/sys/kernel/debug/fortytwo
	 **/
	my_dirent = debugfs_create_dir("fortytwo", NULL);
	/**
	 *  Create files inside fortytwo folder 
	 **/
	debugfs_create_file("id", 0666, my_dirent, (void *)kbuffer,
			&fops_debugfs);
	debugfs_create_file("jiffies", 0666, my_dirent, (void *)kbuffer,
			&fops_debugfs);
	debugfs_create_file("foo", 0666, my_dirent, (void *)kbuffer,
			&fops_debugfs);
	printk(KERN_INFO "Hello World!!!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning the module.\n");
	printk(KERN_INFO "kernel buffer is: %s\n", kbuffer);
	debugfs_remove_recursive(my_dirent);
}

module_init(hello_init);
module_exit(hello_exit);
