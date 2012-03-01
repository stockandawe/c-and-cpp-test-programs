#if 0
#define MODULE
#include <linux/module.h>

int init_module(void) 
{
    printk("<1>Hello, world\n"); 
    return 0;
}

void cleanup_module(void)
{
    printk("<1>Goodbye cruel world\n");
}
#endif
#include <linux/module.h>  /* Needed by all modules */
#include <linux/kernel.h>  /* Needed for KERN_ALERT */


int init_module(void)
{
   printk("<1>Hello world 1.\n");
	
   // A non 0 return means init_module failed; module can't be loaded.
   return 0;
}


void cleanup_module(void)
{
  printk(KERN_ALERT "Goodbye world 1.\n");
}  
