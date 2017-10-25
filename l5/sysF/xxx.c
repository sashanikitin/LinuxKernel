#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/version.h>
#include <linux/init.h>

#include <linux/timer.h>

#define LEN_MSG 160
static char buf_msg[ LEN_MSG + 1 ] = "Hello from module!\n";
static u64 second=0;
static u64 first=1;
///////////////////////////////////////////////////////////////////////////////////////////////////
struct timer_list myTimer;

void timerFun (unsigned long arg) {
    u64 res=0;
	//printk (KERN_INFO "Called timer %d times\n", second++); 
	myTimer.expires = jiffies + 1000;
    add_timer (&myTimer); /* setup the timer again */
    res= first + second;
    first =second;
    second=res;
   // printk (KERN_INFO " %d \n", res);


}
///////////////////////////////////////////////////////////////////////////////////////////////////
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
static ssize_t xxx_show( struct class *class, struct class_attribute *attr, char *buf ) {
#else
	static ssize_t x_show( struct class *class, char *buf ) {
#endif


   //sprintf(buf,"first = %d and second= %d\n", ++second , first++);если поменять местами то не работает
		sprintf(buf,"res = %d \n",second); 
		return strlen( buf );
	}


/* sysfs store() method. Calls the store() method corresponding to the individual sysfs file */
#if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,32)
	static ssize_t xxx_store( struct class *class, struct class_attribute *attr, const char *buf, size_t count ) {
#else
		static ssize_t x_store( struct class *class, const char *buf, size_t count ) {
#endif
			printk( "write %ld\n", (long)count );
			strncpy( buf_msg, buf, count );
			buf_msg[ count ] = '\0';
			return count;
		}

		CLASS_ATTR_RW( xxx );

		static struct class *x_class;

		int __init x_init(void) {
			int res;
			x_class = class_create( THIS_MODULE, "x-class" );
			if( IS_ERR( x_class ) ) printk( "bad class create\n" );
			res = class_create_file( x_class, &class_attr_xxx );


			init_timer (&myTimer);
			myTimer.function = timerFun;
			myTimer.expires = jiffies+1000;
			myTimer.data = 0;

			add_timer (&myTimer);
			printk (KERN_INFO "timer added \n");
			printk( "'xxx' module initialized\n" );
			return res;
		}

		void x_cleanup(void) {

			class_remove_file( x_class, &class_attr_xxx );
			class_destroy( x_class );

			if (!del_timer (&myTimer)) {
				printk (KERN_INFO "Couldn't remove timer!!\n");
			}
			else {
				printk (KERN_INFO "timer removed \n");
			}
			return;
		}

		module_init( x_init );
		module_exit( x_cleanup );
		MODULE_LICENSE( "GPL" );
