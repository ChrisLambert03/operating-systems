#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/proc_fs.h>
#include<linux/jiffies.h>
#include<linux/fs.h>

#define PROC_NAME "jiffies"
#define BUFFER_SIZE 256

struct proc_dir_entry *my_proc;

ssize_t proc_read(struct file *file,char __user *usr_buf,size_t count,loff_t *pos)
{
    int rv = 0;
    char buffer[BUFFER_SIZE];
    static int completed = 0;

    if (completed) {
        completed = 0;
        return 0;
    }

    completed = 1;
    rv = sprintf(buffer, "%lu\n", jiffies);

    copy_to_user(usr_buf, buffer, rv);
       

    return rv;
}

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};

int proc_init(void)
{
    proc_create(PROC_NAME,0666,NULL,&proc_ops);


    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

void proc_exit(void)
{
    remove_proc_entry(PROC_NAME,NULL);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple /proc file using jiffies");
MODULE_AUTHOR("Chris Lambert");