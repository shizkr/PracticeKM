/*
 *  procfs1.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */

#define procfs_name "helloworld"

/**
 * This structure hold information about the /proc file
 *
 */
struct proc_dir_entry *Our_Proc_File;

/* Put data into the proc fs file.
 * 
 * Arguments
 * =========
 * 1. The buffer where the data is to be inserted, if
 *    you decide to use it.
 * 2. A pointer to a pointer to characters. This is
 *    useful if you don't want to use the buffer
 *    allocated by the kernel.
 * 3. The current position in the file
 * 4. The size of the buffer in the first argument.
 * 5. Write a "1" here to indicate EOF.
 * 6. A pointer to data (useful in case one common 
 *    read for multiple /proc/... entries)
 *
 * Usage and Return Value
 * ======================
 * A return value of zero means you have no further
 * information at this time (end of file). A negative
 * return value is an error condition.
 *
 * For More Information
 * ====================
 * The way I discovered what to do with this function
 * wasn't by reading documentation, but by reading the
 * code which used it. I just looked to see what uses
 * the get_info field of proc_dir_entry struct (I used a
 * combination of find and grep, if you're interested),
 * and I saw that  it is used in <kernel source
 * directory>/fs/proc/array.c.
 *
 * If something is unknown about the kernel, this is
 * usually the way to go. In Linux we have the great
 * advantage of having the kernel source code for
 * free - use it.
 */

static int procfs_show( struct seq_file *m, void *v)
{
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	
	seq_printf(m, "Hello World!\n");

	return 0;
}

static int procfile_open(struct inode *inode, struct file *file)
{
	return single_open(file, procfs_show, NULL);
}

static const struct file_operations procfs_fops = {
     .owner	= THIS_MODULE,
     .open	= procfile_open,
     .read	= seq_read,
     .llseek	= seq_lseek,
     .release	= single_release,
};

int init_module()
{
	//Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);
	Our_Proc_File = proc_create(procfs_name, 0, NULL, &procfs_fops);
	if (!Our_Proc_File) {
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	printk(KERN_INFO "/proc/%s created\n", procfs_name);	
	return 0;	/* everything is ok */
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, NULL);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
