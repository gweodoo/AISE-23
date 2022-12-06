#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/keyboard.h>
#include <linux/types.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("adamj");
MODULE_DESCRIPTION("A First module");
MODULE_VERSION("1.0");

static int mycallback(struct notifier_block* s, unsigned long action, void* pointer)
{
	struct keyboard_notifier_param *param = (struct keyboard_notifier_param*)pointer;
	printk(KERN_INFO "a\n");
	if(action == KBD_KEYSYM)
	{
		printk(KERN_INFO "b\n");
		if(param->down)
		{
			printk(KERN_INFO "KEY %d ('%c') pressed !\n", param->value, param->value);
		}
	}
	return NOTIFY_OK;
}

static struct notifier_block s = {
		.notifier_call = mycallback
};

static int __init mymod_init(void){
	register_keyboard_notifier(&s);
	printk(KERN_INFO "MyMod: Hello there !\n");
	return 0;
}
static void __exit mymod_exit(void){
	printk(KERN_INFO "MyMod: Goodbye !\n");
	unregister_keyboard_notifier(&s);
}
module_init(mymod_init);
module_exit(mymod_exit);

