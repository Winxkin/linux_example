#include <linux/module.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <linux/device.h>


#define DRIVER_NAME "my-driver"
#define DRIVER_CLASS "myModuleClass"
#define DRIVER_AUTHOR "Huan Nguyen Duy"
#define DRIVER_VERSION "1.0"

static int driver_open(struct inode *device_file, struct file *instance);
static int driver_close(struct inode *device_file, struct file *instance);
static ssize_t driver_read(struct file *File, char *user_buffer, size_t size, loff_t *offs);
static ssize_t driver_write(struct file *File,const char *user_buffer, size_t size, loff_t *offs);

static  dev_t dev_num;
static  struct class *m_class;
static  struct cdev m_device;




static const struct  file_operations fops =
{
    .read = driver_read,
    .write = driver_write,
    .open = driver_open,
    .release =  driver_close
};



static int init_driver(void)
{
    printk("driver init...\n");
    if(alloc_chrdev_region(&dev_num,0,1,DRIVER_NAME) < 0)
    {
        printk("can't register device driver \n");
        return -1;
    }
    
    printk("my driver: Major: %d, Minor: %d\n",MAJOR(dev_num), MINOR(dev_num));

    if((m_class = class_create (THIS_MODULE, DRIVER_CLASS)) == NULL)
    {
        printk("can't register device driver \n");
        //unregister_chrdev_region(dev_num,1);
        goto rm_device_num;
    }

    if(device_create(m_class,NULL,dev_num,NULL, DRIVER_NAME) == NULL)
    {
        printk("can't register device driver \n");
        //class_destroy(m_class);
        goto rm_class;
    }

    cdev_init(&m_device, &fops);
    if(cdev_add(&m_device, dev_num,1) == 1)
    {
        printk("can't register device driver \n");
        //device_destroy(m_class, dev_num);
        goto rm_device;
    }
    printk("driver init success\n");
    return 0;

rm_device:
     device_destroy(m_class, dev_num);
rm_class:
    class_destroy(m_class);
rm_device_num:
    unregister_chrdev_region(dev_num,1);
    return -1;
}

static void exit_driver(void)
{
    printk("driver exit...\n");
    cdev_del(&m_device);
    device_destroy(m_class, dev_num);
    class_destroy(m_class);
    unregister_chrdev_region(dev_num,1);
    printk("driver goodbye !\n");
   
}


static int driver_open(struct inode *device_file, struct file *instance)
{
    printk("device open.\n");
    return 0;

}

static int driver_close(struct inode *device_file, struct file *instance)
{
     printk("device close.\n");
     return 0;
}


static ssize_t driver_read(struct file *File, char *user_buffer, size_t size, loff_t *offs)
{

    return size;
}

static ssize_t driver_write(struct file *File,const char *user_buffer, size_t size, loff_t *offs)
{   
    return size;
}




module_init(init_driver);
module_exit(exit_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_VERSION(DRIVER_VERSION);

