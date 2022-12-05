#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("adamj");
MODULE_DESCRIPTION("A First module");
MODULE_VERSION("1.0");

static int __init mymod_init(void){
	printk(KERN_INFO "MyMod: Hello there !\n");
	return 0;
}
static void __exit mymod_exit(void){
	printk(KERN_INFO "MyMod: Goodbye !\n");
}
module_init(mymod_init);
module_exit(mymod_exit);

