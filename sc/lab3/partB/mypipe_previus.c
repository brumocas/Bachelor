#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include "buff_helper.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("SC2022/2023");
MODULE_DESCRIPTION("Simple pipe character device driver");
MODULE_VERSION("0:0.1");

/** module functions **/
static int mypipe_init(void);
static void mypipe_exit(void);

/** device driver functions  **/
static int mypipe_open(struct inode *inode, struct file *filp);
static int mypipe_release(struct inode *inode, struct file *filp);
ssize_t mypipe_read(struct file *filep, char __user *buff, size_t count, loff_t *offp);
ssize_t mypipe_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp);

/**  global variables for the device driver  **/
static dev_t mydev;
static struct cdev *mycdev;
static struct file_operations myfops = {
    .owner = THIS_MODULE,
	.read = mypipe_read,
	.write = mypipe_write,
	.open = mypipe_open,
	.release = mypipe_release
};
/***********/
#define BUFFER_MAX_SIZE 100

char data_space[BUFFER_MAX_SIZE];
circ_buff_t mypipe_buffer = { 
    data_space,
    BUFFER_MAX_SIZE,
    0,
    0
};
/************/
static int mypipe_open(struct inode *inode, struct file *filp)
{	
	// link to device file´s private_data
	filp->private_data = mycdev;

	// para não se comportar como o ficheiro
	nonseekable_open(inode, filp);

	// Debug:
	printk(KERN_INFO "mypipe_open(): Returning\n");
    return 0;
}
/************/
static int mypipe_release(struct inode *inode, struct file *filp)
{
	// Debug:
    printk(KERN_INFO "mypipe_release(): Returning\n");
    return 0;
}
/************/
ssize_t mypipe_read(struct file *filep, char __user *buff, size_t count, loff_t *offp)
{
    // Should read data from circ_buffer and transfer it to user space.
	int i, rd_count = 0 ;
	char rd_data = 0 ;
	//char rd_data_str = rd_data + '0';

	for(i = 0; i < count ;i++)
	{	
		if(circ_buff_pop(&mypipe_buffer, &rd_data) != 0) //pop successful, read and up ----> return is 0 for OK
		{
			printk(KERN_INFO "Buffer is empty. Breaking loop...\n");
			break;
		}

		if(copy_to_user( (void __user *) buff + i, (const void *)&rd_data, (unsigned long) 1) != 0) // return is 0 for OK
		{
			printK(KERN_INFO "Error while copying to user space.\n");
			return -1; //error copying
		}
		
		rd_count ++;
		printK(KERN_INFO "mypipe_read(): %d bytes were read...\n",rd_count);

	} 

	if(rd_count == 0) // buffer empty from the start
	{
			printK(KERN_INFO "mypipe_read(): FIFO empty\n");
			return 0; //Signal error to reading process
	}
	else
	{
			return ((ssize_t) rd_count + 1); //something was read
	}
}
/************/
ssize_t mypipe_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp)
{	
    // Should write data from user space to circ_buffer.
	int i, wr_count = 0;
	char wr_data;

	for( i = 0; i < count; i++ )
	{	
		//wr_data = *(buff + i); // get input data
		if(copy_from_user((void *) &wr_data, (const void __user *) buff + i, (unsigned long) 1) != 0) // return is 0 for OK
		{
			printK(KERN_INFO "Error while copying from user space.\n");
			return -1; //error copying
		}

		if(circ_buff_push( &mypipe_buffer, wr_data )	 != 0) //retunr is 0 if OK
		{
			printK(KERN_INFO "String is larger than buffer size... Breaking loop...\n");
			break;
		}
		wr_count++;
		printK(KERN_INFO "mypipe_write(): %d bytes were written...\n",wr_count);
		

	}

	if(wr_count == 0 ) // buffer full from start
	{
		printK(KERN_INFO "mypipe_write(): FIFO full\n");
		return -1; //signal error 	to the writing process
	}
	else
	{	
		return((ssize_t)wr_count + 1); //something as written
	}
	
}
/**************/
static int mypipe_init(void)
{
	int result = -1;

	// Register device driver so that:
	// - Driver name is mypipe 
	// - Major number is dynamically assigned
	// - Minor number starts from 0
	// - Only one device needs to be managed by the driver


	result = alloc_chrdev_region(&mydev,0,1,"mypipe");
	if (result < 0){
		printk(KERN_ERR "Failed to register mypipe device driver\n");
		return result;
	}

    // Create struture to represent char devices
	mycdev = cdev_alloc();
	mycdev->owner = THIS_MODULE;
	mycdev->ops = &myfops;

    // Register character device into the kernel
	result = cdev_add(&mycdev, mydev, 1);
    if (result < 0){
        printk(KERN_ERR "Failed to register character device info\n");
        return result;
    }

	// Print "mypipe device driver registered with major number X"
	// to the kernel logging buffer so that:
	// - X is the obtained major number during registration
	// - Message printed using the informational log evel
	printk(KERN_INFO "mypipe device driver registered with major number %d and minor number %d\n",MAJOR(mydev),MINOR(mydev));
	return 0;
}
/************/
static void mypipe_exit(void)
{
    // deregister character device
	cdev_del(&mycdev);

    // deregister the device driver's device numbers
	unregister_chrdev_region(mydev, 1);

	// print "mypipe device driver deregistered" to the kernel
	// logginf buffer using the information log level
	printk(KERN_INFO "mypipe device driver deregistered\n");
}
/************/
module_init(mypipe_init);
module_exit(mypipe_exit);