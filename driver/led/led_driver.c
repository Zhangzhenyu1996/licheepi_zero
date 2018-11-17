#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/types.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/init.h>

#define PG0 192 //PG0 192=32*6+0
#define DEVICE "myled"
#define MAGIC_NUM 'k'			//定义幻数
#define LED_ON _IO(MAGIC_NUM, 0) //_IO    不带参数的ioctl
#define LED_OFF _IO(MAGIC_NUM, 1)

/*--------------------------------------------------------------------------
  函数名:      led_open
  参数:        struct inode *inode,struct file *filp
  返回值:      int
  描述:        open对应的驱动函数
 -------------------------------------------------------------------------*/

int led_open(struct inode *inode, struct file *filp)
{
   int ret;
   ret = gpio_request_one(PG0,GPIOF_OUT_INIT_HIGH,"myled");//配置为输出,并初始化为高电平,gpio.txt
   if(ret)
	{
		printk("error: cannot request gpio PG0 \n");
		printk("error ret = %d", ret);				
	}
	gpio_set_value(PG0,1); // GPIO 输出   void gpio_set_value(unsigned gpio, int value); 
	return 0;
}

/*--------------------------------------------------------------------------
	函数名:		led_release
	参数：		struct inode *inode,struct file *filp
	返回值：	int
	描述：		release对应的驱动函数
--------------------------------------------------------------------------*/

int led_release(struct inode *inode, struct file *filp)
{
	gpio_free(PG0);// 释放之前声明的 GPIO  void gpio_free(unsigned gpio);
	printk("LED dev release");
	return 0;
}

/*--------------------------------------------------------------------------
	函数名：	led_ioctl
	参数：		struct inode *inode,struct file *filp,unsigned int cmd, unsigned long arg
	返回值：	int
	描述：		led_ioctl函数 
--------------------------------------------------------------------------*/

long led_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case LED_ON:
			printk("GPIO LED set low.\n");
			gpio_set_value(PG0, 0);
			break;
		case LED_OFF:
			printk("GPIO LED set high.\n");
			gpio_set_value(PG0, 1);
			break;
		default:
			printk("error cmd .\n");
			break;
	}
	return 0;
}

/*--------------------------------------------------------------------------
	函数名:		file_opreations led_fops
	参数：		owner, open, release, ioctl		
	返回值:		null
	描述:		file_opreations结构体
--------------------------------------------------------------------------*/

struct file_operations led_fops ={
		.owner		= THIS_MODULE,
		.open		= led_open,
		.release	= led_release,
		.unlocked_ioctl		= led_ioctl,
};

/*--------------------------------------------------------------------------
	函数名:		miscdevie led_dev
	参数:		minor, name, fops
	返回值:		null
	描述:		杂项设备
--------------------------------------------------------------------------*/

struct miscdevice led_dev ={
		.minor		= MISC_DYNAMIC_MINOR,//minor是这个混杂设备的次设备号，若由系统自动配置，则可以设置为MISC_DYNANIC_MINOR
		.name		= DEVICE,			//.name 设备名
		.fops		= &led_fops
};

/*--------------------------------------------------------------------------
	函数名:		led_init
	参数:		void
	返回值:		int
	描述:		模块初始化参数,安装模块时执行
--------------------------------------------------------------------------*/

static int __init led_init(void)
{
		int ret;
		ret = misc_register(&led_dev);//注册驱动程序时采用misc_register函数注册，此函数中会自动创建设备节点，即设备文件
		if(ret)
		{		
			printk("error: cannot register misc.\n");
			return ret;
		}
		printk("misc-register %s\n", DEVICE);
		return 0;
}

/*--------------------------------------------------------------------------
		函数:		led_exit
		参数:		void
		返回值:		void
		描述:		模块卸载函数，卸载模块时执行
--------------------------------------------------------------------------*/

static void __exit led_exit(void)
{
		misc_deregister(&led_dev);//杂项设备的注销过程，其实就是注册过程的逆向过程
		printk("mise-deregister %s\n", DEVICE);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
