#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/fdtable.h>
#include <linux/fs.h> 
#include <linux/fs_struct.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Emir Acimis && Baris Ardic");

static int myPid = 1; 
module_param(myPid, int, 0000);
void project_function(struct task_struct *task)
{   
	struct task_struct *child;
    	struct task_struct *match;
    	struct list_head *list; 
    	struct fdtable *files_table;
    	int count = 0;
	char buf[100];
	struct path *path;
	char* name = NULL;


	//PART3.1
	printk("****The Processes****\n");
	printk("The input: %d\n", myPid);
    	for_each_process(task){
    		printk("****\npid: %d, name: %s, parent_pid: %d\n****\n", task->pid, task->comm, task->parent->pid);
  
    		if(task->pid == myPid){
			printk("****Found: %s\n****", task->comm);
    			match = task;
   	 	}
 
    		list_for_each(list, &task->children) {        
			child = list_entry(list, struct task_struct, sibling);
    		}
	}//for each end
	printk("****The Virtual Adresses****\n");
	//PART3.2
	if(match->mm->mmap){
		printk("Virtual Memory Start Adress: %lx\n",match->mm->mmap->vm_start);
		printk("Virtual Memory End Adress: %lx\n",match->mm->mmap->vm_end);
		printk("Virtual Memory Size: %lu\n",(match->mm->mmap->vm_end - match->mm->mmap->vm_start));
	}
	//PART3.3
	printk("****File Information****\n");	
	if(match->files != NULL){
		files_table = files_fdtable(match->files);
		if(files_table != NULL && files_table -> fd != NULL){
			printk("Open files number: %d\n", files_table -> max_fds);
			while(count < files_table -> max_fds){
				if(match->files->fd_array[count] != NULL && match->files->fd_array[count]->f_inode != NULL){
					path = &(match -> files ->fd_array[count]->f_path);
					name = d_path(path, buf, sizeof(buf));
					printk("File Name: %s, File Size: %lld, File Block Bits: %hu, File Version: %llu Count: %d\n",name , match->files->fd_array[count]->f_inode->i_size, match->files->fd_array[count]->f_inode->i_blkbits ,match->files->fd_array[count]->f_inode->i_version, count);
					printk("**********\n");
		   		}
				count = count + 1;
			}
		}
    	}
}
    
int task_lister_init(void)
{
	printk(KERN_INFO "***Module Loading***\n");
    	project_function(&init_task);
    	return 0;
}

void task_lister_exit(void)
{
	printk(KERN_INFO "***Module Removing***\n");
}

module_init(task_lister_init);
module_exit(task_lister_exit);
