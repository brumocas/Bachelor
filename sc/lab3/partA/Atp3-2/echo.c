/********************************
*
*   Device driver for an echo device (see lab assignment up to exercise 6)
*
*   Note that there are many possible solutions, this is just a possible one.
*
*********************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Me");
MODULE_DESCRIPTION("Simple echo character device driver");
MODULE_VERSION("0:0.1");

/** device driver functions  **/
static int echo_open(struct inode *inode, struct file *filp);
static int echo_release(struct inode *inode, struct file *filp);
ssize_t echo_read(struct file *filep, char __user *buff, size_t count, loff_t *offp);
ssize_t echo_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp);

/** module functions **/
static int echo_init(void);
static void echo_exit(void);

/**  globas variables for the device driver  **/
static dev_t mydev;
static struct cdev *mycdev;
static struct file_operations myfops = {
    .owner = THIS_MODULE,
	.read = echo_read,
	.write = echo_write,
	.open = echo_open,
	.release = echo_release,
	// TODO (Exercise 5): Make this a non-seekable device
	.llseek = no_llseek
};

// Here, the driver should define some variables for storing the number of echoed characters 
// since the last time a read instruction was called...

static int echo_open(struct inode *inode, struct file *filp)
{	

    // TODO (Exercise 5): Properly link file's private_data
	filp->private_data = mycdev;

    // TODO (Exercise 5): Make this a non-seekable device
	nonseekable_open(inode, filp);
		

	// for debug:
	printk(KERN_INFO "echo_open(): Returning\n");
	return 0;
}

static int echo_release(struct inode *inode, struct file *filp)
{
	// for debug:
	printk(KERN_INFO "echo_release(): Returning\n");
    return -1;
}

ssize_t echo_read(struct file *filep, char __user *buff, size_t count, loff_t *offp)
{
	// TODOs (Exercise 6): 
	// The read() should take the current number of echoed characters and send it to buff (in user space).
	// After that, it should also reset the counting of echoed characters.
	// Remember to return the total number of transferred bytes.

	// for debug:
	printk(KERN_INFO "echo_release(): Returning\n");
    return -1;
}

ssize_t echo_write(struct file *filep, const char __user *buff, size_t count, loff_t *offp)
{
	// TODOs (Exercise 6):
	// The write() should take the a string content stored on buff (from the user memory space) and store it in a local buffer (in the kernel memory space).
	// The transferred content can be shown in a printk function for debugging.
	// Remember to add a zero (null) character at the end of the string to signal its termination.
	// It should also count the number of stored characters and update the total number of echoed characters, for latter use by the read() function.
	// Finally, the total number of written bytes must be returned.

	// REMEMBER TO NEVER RETURN A CONSTANT 0 VALUE, OR THE CALLER COULD RETRY THE WRITE OPERATION ENDLESSLY.

	// debug:
	printk(KERN_INFO "echo_write(): Returning\n");
	// return count
	return -1;
}


/** module functions  **/
static int echo_init(void)
{
	int result = -1;	

	// TODOs (Exercise 2): Register the device driver so that:
	// - Driver name is echo 
	// - Major number is dynamically assigned
	// - Minor number starts from 0
	// - Only one device needs to be managed by the driver


	// TODO (Exercise 2) print "Echo device driver registered with major number X" to the kernel logging buffer so that:
	// - X is the obtained major number during registration
	// - Message printed using the informational log evel
	result = alloc_chrdev_region(&mydev,0,1,"echo");
	if ( result < 0)
	{
		printk(KERN_ERR "Failed to register echo device driver\n");
	}	
	
	printk(KERN_INFO "Echo device driver registered with major number %d and minor number %d\n", MAJOR(mydev), MINOR(mydev));


	// Initialize the character device
    mycdev = cdev_alloc();
	mycdev->owner = THIS_MODULE;
	mycdev->ops = &myfops;
	

    // TODO (Exercise 4): Register character device into the kernel
	// atention &
	result = cdev_add(mycdev, mydev, 1);
	if (result < 0){
		printk(KERN_ERR "Failed to register echo device driver\n");
		return result;
	}

	return 0;
}

static void echo_exit(void)
{
    // TODO (Exercise 4): Deregister character device
	// Prof não faz este passo
	cdev_del(mycdev);
		
    // TODO (Exercise 2): Deregister the device driver's device numbers
    unregister_chrdev_region(mydev, 1);

	// TODO (Exercise 2): Print "Echo device driver deregistered" to the kernel logginf buffer using the information log level
	printk(KERN_INFO "Echo device driver deregistered\n");
}

module_init(echo_init);
module_exit(echo_exit);
