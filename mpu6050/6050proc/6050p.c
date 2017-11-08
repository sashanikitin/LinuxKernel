#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include "mpu6050-regs.h"

#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

struct mpu6050_data
{
	struct i2c_client *drv_client;
	int accel_values[3];
	int gyro_values[3];
	int temperature;
};

static struct mpu6050_data g_mpu6050_data;

static int mpu6050_read_data(void)
{
	int temp;

	if (g_mpu6050_data.drv_client == 0)
		return -ENODEV;

	/* accel */
	g_mpu6050_data.accel_values[0] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_ACCEL_XOUT_H));
	g_mpu6050_data.accel_values[1] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_ACCEL_YOUT_H));
	g_mpu6050_data.accel_values[2] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_ACCEL_ZOUT_H));
	/* gyro */
	g_mpu6050_data.gyro_values[0] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_GYRO_XOUT_H));
	g_mpu6050_data.gyro_values[1] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_GYRO_YOUT_H));
	g_mpu6050_data.gyro_values[2] = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_GYRO_ZOUT_H));
	/* temp */
	/* Temperature in degrees C = (TEMP_OUT Register Value  as a signed quantity)/340 + 36.53 */
	temp = (s16)((u16)i2c_smbus_read_word_swapped(g_mpu6050_data.drv_client, REG_TEMP_OUT_H));
	g_mpu6050_data.temperature = (temp + 12420 + 170) / 340;

	printk(KERN_INFO "mpu6050: sensor data read:\n");
	printk(KERN_INFO "mpu6050: ACCEL[X,Y,Z] = [%d, %d, %d]\n",
		g_mpu6050_data.accel_values[0],
		g_mpu6050_data.accel_values[1],
		g_mpu6050_data.accel_values[2]);
	printk(KERN_INFO "mpu6050: GYRO[X,Y,Z] = [%d, %d, %d]\n",
		g_mpu6050_data.gyro_values[0],
		g_mpu6050_data.gyro_values[1],
		g_mpu6050_data.gyro_values[2]);
	printk(KERN_INFO "mpu6050: TEMP = %d\n", g_mpu6050_data.temperature);
	return 0;
}

static int mpu6050_probe(struct i2c_client *drv_client, const struct i2c_device_id *id)
{
	int ret;

	printk(KERN_INFO "mpu6050: i2c client address is 0x%X\n", drv_client->addr);

	/* Read who_am_i register */
	ret = i2c_smbus_read_byte_data(drv_client, REG_WHO_AM_I);
	if (IS_ERR_VALUE(ret)) {
		printk(KERN_ERR "mpu6050: i2c_smbus_read_byte_data() failed with error: %d\n", ret);
		return ret;
	}
	if (ret != MPU6050_WHO_AM_I) {
		printk(KERN_ERR "mpu6050: wrong i2c device found: expected 0x%X, found 0x%X\n", MPU6050_WHO_AM_I, ret);
		return -1;
	}
	printk(KERN_INFO "mpu6050: i2c mpu6050 device found, WHO_AM_I register value = 0x%X\n", ret);

	/* Setup the device */
	/* No error handling here! */
	i2c_smbus_write_byte_data(drv_client, REG_CONFIG, 0);
	i2c_smbus_write_byte_data(drv_client, REG_GYRO_CONFIG, 0);
	i2c_smbus_write_byte_data(drv_client, REG_ACCEL_CONFIG, 0);
	i2c_smbus_write_byte_data(drv_client, REG_FIFO_EN, 0);
	i2c_smbus_write_byte_data(drv_client, REG_INT_PIN_CFG, 0);
	i2c_smbus_write_byte_data(drv_client, REG_INT_ENABLE, 0);
	i2c_smbus_write_byte_data(drv_client, REG_USER_CTRL, 0);
	i2c_smbus_write_byte_data(drv_client, REG_PWR_MGMT_1, 0);
	i2c_smbus_write_byte_data(drv_client, REG_PWR_MGMT_2, 0);

	g_mpu6050_data.drv_client = drv_client;

	printk(KERN_INFO "mpu6050: i2c driver probed\n");
	return 0;
}

static int mpu6050_remove(struct i2c_client *drv_client)
{
	g_mpu6050_data.drv_client = 0;

	printk(KERN_INFO "mpu6050: i2c driver removed\n");
	return 0;
}

static const struct i2c_device_id mpu6050_idtable [] = {
	{ "gl_mpu6050", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, mpu6050_idtable);

static struct i2c_driver mpu6050_i2c_driver = {
	.driver = {
		.name = "gl_mpu6050",
	},

	.probe = mpu6050_probe ,
	.remove = mpu6050_remove,
	.id_table = mpu6050_idtable,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MODULE_TAG      "example_module_mpu6050"
#define PROC_DIRECTORY  "mpu6050"
#define PROC_FILENAME   "all"
#define BUFFER_SIZE     200

static struct proc_dir_entry *proc_dir;
static struct proc_dir_entry *proc_file;

static char *proc_buffer;
static size_t proc_msg_length;
static size_t proc_msg_read_pos;

static int read(struct file *file_p, char __user *buffer, size_t length, loff_t *offset);

static struct file_operations proc_fops = {
	.read  = read,
	
};

static int create_proc(void)
{
	proc_dir = proc_mkdir(PROC_DIRECTORY, NULL);
	if (NULL == proc_dir)
		return -EFAULT;

	proc_file = proc_create(PROC_FILENAME, S_IFREG | S_IRUGO | S_IWUGO, proc_dir, &proc_fops);
	if (NULL == proc_file)
		return -EFAULT;
    proc_buffer = kmalloc(BUFFER_SIZE, GFP_KERNEL);
    if (NULL == proc_buffer)
        return -ENOMEM;
    proc_msg_length = 0;
    proc_msg_read_pos=0;
	return 0;
}

static int read(struct file *file_p, char __user *buffer, size_t length, loff_t *offset)
{	


	size_t left;
	if(proc_msg_read_pos==0){
	mpu6050_read_data();	
	sprintf(proc_buffer, "{\"temp\": \"%d\",\"ax\": \"%d\",\"ay\": \"%d\",\"az\": \"%d\",\"gx\": \"%d\",\"gy\": \"%d\",\"gz\": \"%d\"}\n",
	 g_mpu6050_data.temperature,g_mpu6050_data.accel_values[0],g_mpu6050_data.accel_values[1],g_mpu6050_data.accel_values[2],
	 g_mpu6050_data.gyro_values[0],g_mpu6050_data.gyro_values[1],g_mpu6050_data.gyro_values[2]);
	}


    

    if (length > (strlen(proc_buffer) - proc_msg_read_pos))
        length = (strlen(proc_buffer) - proc_msg_read_pos);

    left = copy_to_user(buffer, &proc_buffer[proc_msg_read_pos], length);

    printk("left=%d", left);
    proc_msg_read_pos += length - left;

    if (left)
        printk(KERN_ERR MODULE_TAG "failed to read %u from %u chars\n", left, length);
    else{
        printk(KERN_NOTICE MODULE_TAG "read %u chars\n", length);
    
    }
    printk("pos = %d and length=%d", proc_msg_read_pos,strlen(proc_buffer));
    if(proc_msg_read_pos==strlen(proc_buffer)){
       proc_msg_read_pos=0;
       printk("zero");
       
    }

    return length - left;
}



static void cleanup_proc(void)
{
	if (proc_file)
	{
		remove_proc_entry(PROC_FILENAME, proc_dir);
		proc_file = NULL;
	}
	if (proc_dir)
	{
		remove_proc_entry(PROC_DIRECTORY, NULL);
		proc_dir = NULL;
	}

    if (proc_buffer) {
        kfree(proc_buffer);
        proc_buffer = NULL;
    }
    proc_msg_length = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
static int m6050_init(void)
{
	int ret;

	/* Create i2c driver */
	ret = i2c_add_driver(&mpu6050_i2c_driver);
	if (ret) {
		printk(KERN_ERR "mpu6050: failed to add new i2c driver: %d\n", ret);
		return ret;
	}
	printk(KERN_INFO "mpu6050: i2c driver created\n");

	/* Create class */
	
	ret = create_proc();
	if (ret)
		goto error;
    
    return 0;
	error:
	printk(KERN_ERR MODULE_TAG "failed to load\n");
	cleanup_proc();
	return ret;
	
	
}

static void m6050_exit(void)
{
	
	i2c_del_driver(&mpu6050_i2c_driver);
	printk(KERN_INFO "mpu6050: i2c driver deleted\n");

	printk(KERN_INFO "mpu6050: module exited\n");
	cleanup_proc();
}

module_init(m6050_init);
module_exit(m6050_exit);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("Aleksandr Nikitin");
MODULE_DESCRIPTION("Example for sysfs and mpu6050 read/write");
MODULE_VERSION("0.1");