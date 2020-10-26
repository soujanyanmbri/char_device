
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

volatile static int is_open = 0;

static char message[10024];
int num_bytes = 0;
static char enc_message[10024];
/* ACCESS TO ADDRESS SPCAE OF A PROCESS 



*/

int mod9_open(struct inode *pinode, struct file *pfile)
{

	if (is_open == 1)
	{
		printk(KERN_INFO "Error - device already open\n");
		return -EBUSY;
	}
	is_open = 1;
	try_module_get(THIS_MODULE);
	return 0;
}

ssize_t mod9_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{

	int bytes_read = 0;
	if (offset == NULL)
		return -1;

	if (*offset >= num_bytes)
		return 0;

	copy_to_user(buffer, enc_message, length);
	return length;
}

// USING AFFINE CIPHER TO ENCODE THE MESSAGE. ENCODING IS DONE HERE, E = (ax + b) mod m
// a and b are the keys of the cipher
// a must be chosen such that the a and m are coprime

ssize_t mod9_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	int i = 0;
	if (offset == NULL)
		return -EINVAL;
	if (*offset >= num_bytes)
		return -EINVAL;

	int enc_code;
	// this takes n bytes from kernel space to user space.
	if (copy_from_user(message, buffer, length))
		return -EFAULT;
	int a = 17, b = 5;
	for (i = 0; i < length; i++)
	{
		if (message[i] >= 'A' && message[i] <= 'Z')
		{
			if (message[i] != ' ')
			{
				enc_code = ((((a * (message[i] - 'A')) + b) % 26) + 'A');
				enc_message[i] = enc_code;
			}
			else
				enc_message[i] = ' ';
		}
		else
		{
			if (message[i] != ' ')
			{
				enc_code = ((((a * (message[i] - 'a')) + b) % 26) + 'a');
				enc_message[i] = enc_code;
			}
			else
				enc_message[i] = ' ';
		}
	}

	printk(KERN_INFO "(%s)\n", enc_message);
	return length;
}

int mod9_close(struct inode *pinode, struct file *pfile)
{

	if (is_open == 0)
	{
		printk(KERN_INFO "Error - device wasn't opened\n");
		return -EBUSY;
	}
	is_open = 0;

	module_put(THIS_MODULE);
	return 0;
}

struct file_operations mod9_file_operations = {
	.owner = THIS_MODULE,
	.open = mod9_open,
	.read = mod9_read,
	.write = mod9_write,
	.release = mod9_close,
};

int module9_init(void)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

	strncpy(message, "hello world.", 10023);
	num_bytes = strlen(message);

	register_chrdev(239, "Simple Char Drvr 2", &mod9_file_operations);

	return 0;
}

void module9_exit(void)
{
	printk(KERN_ALERT "Inside the %s function", __FUNCTION__);
	unregister_chrdev(239, "Simple Char Drvr 2");
}
module_init(module9_init);
module_exit(module9_exit);
