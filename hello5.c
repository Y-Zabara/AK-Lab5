#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("y_zabara_io14");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint howmany = 1;

module_param(howmany, uint, S_IRUGO);
MODULE_PARM_DESC(howmany, "Number of printed lines 'Hello, world!'");

struct hello_list {
	struct list_head list_node;
	ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{
	uint i;
	struct hello_list *md;

	if (howmany == 0 || (howmany > 5 && howmany <= 10))
		printk(KERN_EMERG "Warning: howmany is 0 or between 5 and 10.\n");

	BUG_ON(howmany > 10);
	
	for (i = 0; i < howmany; ++i) {
		md = kmalloc(sizeof(struct hello_list), GFP_KERNEL);
		
		if (i == 5) {
			md = 0;
		}
		
		md->time = ktime_get();
		list_add_tail(&md->list_node, &my_list_head);
		printk(KERN_EMERG "Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_list *md, *tmp;

	list_for_each_entry_safe(md, tmp, &my_list_head, list_node)	{
		printk(KERN_EMERG "md time: %lld ns\n", ktime_to_ns(md->time));
		list_del(&md->list_node);
		kfree(md);
	}

	BUG_ON(!list_empty(&my_list_head));
	printk(KERN_EMERG "Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);
