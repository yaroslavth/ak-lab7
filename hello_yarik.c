#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/list.h>

struct list_item {
	ktime_t time;
	struct list_head list_node;
};

static struct list_head my_list_head = LIST_HEAD_INIT(my_list_head);

MODULE_AUTHOR("Drovnin Pavlo IV-83");
MODULE_DESCRIPTION("Lab7");
MODULE_LICENSE("Dual BSD/GPL");

static uint amount = 1;
module_param(amount, uint, 0444);
MODULE_PARM_DESC(amount, " Number of 'Hello, world'");


static int __init hello_init(void)
{
	uint i = 0;
	struct list_item *tail;
	

	pr_info("amount: %d\n", amount);

	if (amount == 0 || (amount >= 5 && amount <= 10))
		pr_warn("WARNING\nThe parameter is between 5 and 10 or 0");
	BUG_ON(amount > 10);

	for (i = 0 ; i < amount; i++) 
	{
		
		tail = kmalloc(sizeof(struct list_item), GFP_KERNEL); 
		if (i == 5)
		    tail = 0;
		if(ZERO_OR_NULL_PTR(tail))
			goto error;
		tail->time = ktime_get();
		list_add_tail(&(tail->list_node), &my_list_head);
		pr_info("Hello world\n");
	}

	return 0;

error:
	{
		struct list_item *md, *tmp;
		pr_err("kmalloc out of memory");
		list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
			list_del(&md->list_node);
			kfree(md);
		}
		BUG();
		return -ENOMEM;
	}
	
}

static void __exit hello_exit(void)
{
	struct list_item *md, *tmp;

	list_for_each_entry_safe(md, tmp, &my_list_head, list_node) {
		pr_info("time: %lld\n", md->time);
		list_del(&md->list_node);
		kfree(md);
	}
	BUG_ON(!list_empty(&my_list_head));
}

module_init(hello_init);
module_exit(hello_exit);


