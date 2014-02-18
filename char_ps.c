/* description:
 * 按照进程树的关系来打印进程
 */

#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/cdev.h>
#include <linux/io.h>
#include <linux/slab.h>
//#include <asm/system.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/list.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("He xingjie");

#define BUFFER_SIZE 0x2000 /*缓冲区最大8KB*/
#define CHAR_PS_MAJOR 250 /*char_ps的主设备号*/
#define PROCESSTREE 0X01
#define THREADGROUP 0x02
#define MEMSTAT 0X03

static int char_ps_major = CHAR_PS_MAJOR;

/*char_ps 设备结构体*/
struct char_ps_dev{
	struct cdev cdev;  /*cdev结构体*/
	unsigned char buff[BUFFER_SIZE];  /*全局内存*/
};

struct char_ps_dev *char_ps_devp;   /*设备结构体指针*/

char tmp[128];
char str[] = "abcd";
//static char buf[1024*8] = {0};

/*遍历每个进程，并按照进程树的形式把
 * 进程sprintf到char_ps_devp->buff内存当中
 */
void ps_tree(struct task_struct *p, int blank)
{
	int i;
	for(i = 1; i <= blank; i++)
	{
		sprintf(tmp, " ");
		printk(" ");
	}

	sprintf(tmp, "|---%s---%d---%d\n", p->comm, p->pid,
			p->parent->pid);

	printk("|---%s---%d---%d\n", p->comm, p->pid,
			p->parent->pid);


	strcat(char_ps_devp->buff, tmp);
	memset(tmp, 0, sizeof(tmp));
	
	/*
	struct list_head *l;
	for (l = p->children.next; l != &(p->children); l = l->next)
	{
		struct task_struct *t = list_entry(l, struct task_struct, sibling);
		ps_tree(t, blank+1);
	}
	*/

	struct list_head *list;
	list_for_each(list, &p->children)
	{
		struct task_struct *t = list_entry(list, struct task_struct, sibling);
		ps_tree(t, blank+1);
	}
}


int char_ps_open(struct inode *inode, struct file *filp)
{
	/*将设备结构体指针赋值给文件私有数据指针*/
	filp->private_data = char_ps_devp;
	printk(KERN_INFO"char_ps_open");

	return 0;
}

int char_ps_ioctl(struct file *filp,
		unsigned int cmd, unsigned long arg)
{
	/*获得设别结构体指针*/
	struct char_ps_dev *dev = filp->private_data;

	printk(KERN_INFO"ioctl receive cmd %d",cmd);

	switch(cmd){
		case PROCESSTREE:
			{
			struct task_struct *task = &init_task;
			int blank = 0;
			int count = 0;

			printk(KERN_INFO"Before ps_tree\n");
		    ps_tree(task, blank);
			printk(KERN_INFO"After ps_tree\n");

			printk(KERN_INFO"ARG:%x\n", arg);
			printk(KERN_INFO"dev->buff:%s\n", dev->buff);
			printk(KERN_INFO"strlen(dev->buff):%d\n", strlen(dev->buff));

			count = strlen(dev->buff);
			if (count > 4112)
			{
				count = 4112;
			}

#if 0
			/*内核空间->用户空间*/
			if (copy_to_user((char *) arg, dev->buff, 40) != 0)
				{
					printk("copyt_to_user failed!\n");
					return -EFAULT;
				}
#endif
			/*内核空间->用户空间*/
			if (copy_to_user((char *) arg, str, 5) != 0)
				{
					printk("copyt_to_user failed!\n");
					return -EFAULT;
				}
			}

			printk(KERN_INFO"Before break\n");

		break;
	default:
		printk(KERN_ALERT"IOCTL_CMD_ERROR\n");
		return - EINVAL;
	}

	return 0;
}

/*文件操作结构体*/
static const struct file_operations char_ps_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = char_ps_ioctl,
	.open = char_ps_open,
};


/*初始化并注册cdev*/
static void char_ps_setup_cdev(struct char_ps_dev *dev, int index)
{
	int err, devno = MKDEV(char_ps_major, index);

	cdev_init(&dev->cdev, &char_ps_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	if (err)
		printk(KERN_NOTICE "Error %d adding char_ps %d",
				err, index);
}

/*设备驱动模块加载函数*/
static __init int char_ps_init(void)
{
	int result;
	dev_t devno = MKDEV(char_ps_major, 0);

	/*申请设备号*/
	if (char_ps_major)
		result = register_chrdev_region(devno, 1, "char_ps");
	else{
		/*动态申请设备号*/
		result = alloc_chrdev_region(&devno, 0, 1, "char_ps");
		char_ps_major = MAJOR(devno);
	}
	if (result < 0)
		return result;

	/*动态申请设备结构体的内存*/
	char_ps_devp = kmalloc(sizeof(struct char_ps_dev), GFP_KERNEL);
	if (!char_ps_devp)
	{
		/*申请失败*/
		result = -ENOMEM;
		printk(KERN_INFO"kmalloc in char_ps_init failed!\n");
		goto fail_malloc;
	}

	memset(char_ps_devp, 0, sizeof(struct char_ps_dev));
	
	char_ps_setup_cdev(char_ps_devp, 0);

	printk(KERN_INFO"char_ps_init success!\n");
	return 0;

fail_malloc:
	unregister_chrdev_region(devno, 1);
	return result;

}

/*模块卸载函数*/
static __exit void char_ps_exit(void)
{
	/*注销cdev*/
	cdev_del(&char_ps_devp->cdev);

	/*释放设备结构体内存*/
	kfree(char_ps_devp);

	/*释放设备号*/
	unregister_chrdev_region(MKDEV(char_ps_major, 0), 1);

	printk(KERN_INFO"char_ps_exit,GoodBye,Kernel!\n");

}

module_init(char_ps_init);
module_exit(char_ps_exit);

