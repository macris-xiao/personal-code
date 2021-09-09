#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/cdev.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/sched.h>
#include <linux/slab.h>

#define BUFFER_MAX	(10)
#define OK		(0)
#define ERROR		(-1)

struct cdev *gDev;
struct file_operations *gFile;

dev_t devNum;
unsigned int subDevNum = 1;
int reg_major = 232;
int reg_minor = 0;
char *buffer;
int flag = 0;

#define LEDBASE	0x56000010
#define LEDLEN	0x0c

int hello_open(struct inode *p, struct file *f)
{
	printk(KERN_EMERG"hello_open\r\n");

	return 0;
}

ssize_t hello_write(struct file*f, const char __user *u, size_t s, loff_t *l)
{
	printk(KERN_EMERG"hello_write\r\n");

	return 0;
}

ssize_t hello_read(struct file *f, char __user *u, size_t s, loff_t *l)
{
	printk(KERN_EMERG"hello_read\r\n");

	return 0;
}

static int hellodev_probe(struct platform_device *pdev)
{
	printk(KERN_INFO "hellodev_probe\n");

	/* Create dev number by MKDEV */
	devNum = MKDEV(reg_major, reg_minor);

	/* register dev number in kernel */
	if(OK == register_chrdev_region(devNum, subDevNum, "hello world")){
		printk(KERN_EMERG"register_chrdev_region ok \n");
	}else {
		printk(KERN_EMERG"register_chrdev_region error n");

		return ERROR;
	}

	printk(KERN_EMERG"hello driver init \n");
	/* cdev represent a character device */
	gDev = kzalloc(sizeof(struct cdev), GFP_KERNEL);
	/* file_operations include some functions about how to operate file */
	gFile = kzalloc(sizeof(struct file_operations), GFP_KERNEL);
	/* open read and write are function point, assigned values for them */
	gFile->open = hello_open;
	gFile->read = hello_read;
	gFile->write = hello_write;
	gFile->owner = THIS_MODULE;
	/* build contact between gDev and gFile */
	cdev_init(gDev, gFile);
	/* build contact between gDev and devNum */
	cdev_add(gDev, devNum, 1);

	return 0;
}

static int hellodev_remove(struct platform_device *pdev)
{
	printk(KERN_INFO "hellodev_remove \n");
	cdev_del(gDev);
	kfree(gFile);
	kfree(gDev);
	unregister_chrdev_region(devNum, subDevNum);

	return 0;
}

static void hello_plat_release(struct device *dev)
{
	return;
}

static struct resource hello_dev_resource[] = {
	[0] = {
		.start	= LEDBASE,
		.end	= LEDBASE + LEDLEN - 1,
	}
};

struct platform_device hello_device = {
	.name		= "hello-dev",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(hello_dev_resource),
	.resource	= hello_dev_resource,
	.dev = {
		.release = hello_plat_release,
	}
};

static struct platform_driver hellodev_driver = {
	.probe	= hellodev_probe,
	.remove	= hellodev_remove,
	.driver	={
		.owner	= THIS_MODULE,
		.name	= "hello-dev",
	},
};

int charDrvInit(void)
{
	platform_device_register(&hello_device);

	return platform_driver_register(&hellodev_driver);
}

void __exit charDrvExit(void)
{
	platform_device_unregister(&hello_device);
	platform_driver_unregister(&hellodev_driver);

	return;
}

/* the entry function of driver */
module_init(charDrvInit);
/* the exit function of driver */
module_exit(charDrvExit);
/* Copyright statement */
MODULE_LICENSE("GPL");
