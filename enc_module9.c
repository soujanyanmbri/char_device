#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include<linux/ioctl.h>


volatile static int is_open = 0;
static char message[10024];
static char dec_message[10024];
static char enc_message[10024];
static int mode = 0;

int num_bytes = 0;
#define MAJOR_NUM 239

#define IOCTL_DEC _IO(MAJOR_NUM, 0)
#define IOCTL_ENC _IO(MAJOR_NUM, 1)


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

	if(copy_to_user(buffer, enc_message, length))
		return -EFAULT;
	return length;
}

// USING AFFINE CIPHER TO ENCODE THE MESSAGE, E = (ax + b) mod m, where m = 26, a = 17, b = 5;
// DECODING OF THE CIPHER IS DONE THROUGH D ( x ) = a^-1 ( x - b ) mod m
// a and b are the keys of the cipher
// a must be chosen such that the a and m are coprime

ssize_t mod9_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	int i = 0;
	if (offset == NULL)
		return -EINVAL;
	if (*offset >= num_bytes)
		return -EINVAL;

	int enc_code, dec_code;
	// this takes n bytes from kernel space to user space.
	if (copy_from_user(message, buffer, length))
		return -EFAULT;
	int a = 19, b = 5;
	printk(KERN_ALERT "%d", mode);
	if(mode == 0){
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
		printk(KERN_INFO "Encrypted message:");
		printk(KERN_INFO "(%s)\n", enc_message);
	}
	else
	{
		int a_inv = 0;
		int flag = 0;
		for (i = 0; i < 26; i++)
		{
			flag = (a * i) % 26;
			if (flag == 1)
				a_inv = i;
		}
		for (i= 0; i < 26; i++)
		{
			if(message[i] >= 'A' && message[i] <= 'Z'){
				if (message[i] != ' ')
				{
					dec_code = (((a_inv * ((message[i] + 'A' - b)) % 26)) + 'A');
					dec_message[i] = dec_code;
				}
				else
					dec_message[i] = ' ';
			}
			else{
				if (message[i] != ' ')
				{
					dec_code = (((a_inv * ((message[i] + 'a' - b)) % 26)) + 'a');
					dec_message[i] = dec_code;
				}
				else
					dec_message[i] = ' ';
			}
		}
		printk(KERN_INFO "Decrypted message:");
		printk(KERN_INFO "(%s)\n", dec_message);
	}
	
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
// IF MOD = 0 ==> ENCRYPT ELSE DECRYPT
long mod9_ioctl(struct file *pfile, unsigned int ioctl_num, unsigned long arg)
{
	switch(ioctl_num){
		case(IOCTL_DEC):
			mode = 0;
			break;
		case(IOCTL_ENC):
			mode = 1;
			break;
		default:
			return -ENOTTY;
	}

	printk(KERN_INFO "IOCTL set mode %d\n.",mode);

	return 0;
}
struct file_operations mod9_file_operations = {
	.owner = THIS_MODULE,
	.open = mod9_open,
	.read = mod9_read,
	.write = mod9_write,
	.release = mod9_close,
	.unlocked_ioctl = mod9_ioctl
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
