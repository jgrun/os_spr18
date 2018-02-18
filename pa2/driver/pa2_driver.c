#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>

#define MAJOR_NUM 60
#define DEVICE_BUSY 1
#define SUCCESS 0
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#define BUFFER_SIZE 1024
#define OUT_OF_BOUNDS 1

static int device_open = 0;
static int open_counter = 0;
static int close_counter = 0;

static int pa2_open(struct inode *pinode, struct file *pfile);
static int pa2_close(struct inode *pinode, struct file *pfile);
static loff_t pa2_seek(struct file *pfile, loff_t offset, int whence);
static ssize_t pa2_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset);
static ssize_t pa2_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset);
static char * device_buffer;

struct file_operations pa2_fops = {
  .open = pa2_open,
  .release = pa2_close,
  .llseek = pa2_seek,
  .read = pa2_read,
  .write = pa2_write
};

static int pa2_open(struct inode *pinode, struct file *pfile) {
  if(device_open) {
    printk(KERN_INFO "Device busy\n");
    return DEVICE_BUSY;
  }

  device_open = 1;
  printk(KERN_INFO "Device opened %d times until now\n", open_counter++);
  return SUCCESS;
}

static int pa2_close(struct inode *pinode, struct file *pfile) {
  if(device_open == 0) {
    printk(KERN_INFO "No device open\n");
    return DEVICE_BUSY;
  }

  device_open = 0;
  printk(KERN_INFO "Device closed %d times until now\n", close_counter++);
  return SUCCESS;
}

static loff_t pa2_seek(struct file *pfile, loff_t offset, int whence) {
  off_t new_pos;
  switch(whence) {
    case SEEK_SET:
      new_pos = offset;
    break;
    case SEEK_CUR:
      new_pos = pfile->f_pos + offset;
    break;
    case SEEK_END:
      new_pos = BUFFER_SIZE - offset;
  }
  if(new_pos >= 1024 || new_pos < 0) {
    printk(KERN_INFO "Out of bounds seek\n");
    return OUT_OF_BOUNDS;
  }
  pfile->f_pos = new_pos;
  printk(KERN_INFO "File position moved to %d\n", new_pos);
  return new_pos;
}

static ssize_t pa2_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset) {
  if((int)length + (int)offset >= BUFFER_SIZE) {
    length -= (size_t)(BUFFER_SIZE - ((int)length + (int)offset));
  }
  printk(KERN_INFO "Read %d bytes\n", (int)length);
  copy_to_user(buffer, device_buffer, length);
  return length;
}

static ssize_t pa2_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset) {
  if((int)length + (int)offset >= BUFFER_SIZE) {
    length -= (size_t)(BUFFER_SIZE - ((int)length + (int)offset));
  }
  printk(KERN_INFO "Wrote %d bytes\n", (int)length);
  copy_from_user(device_buffer, buffer, length);
  return length;
}

static char * pa2_device_name = "pa2_device";

int pa2_init(void) {
  printk(KERN_INFO "Registering %s\n",__FUNCTION__);
  register_chrdev(MAJOR_NUM, pa2_device_name, &pa2_fops);
  device_buffer = (char *)kmalloc(BUFFER_SIZE, GFP_KERNEL);
  return 0;
}

void pa2_exit(void) {
  printk(KERN_INFO "Unregistering %s\n",__FUNCTION__);
  unregister_chrdev(MAJOR_NUM, pa2_device_name);
  kfree(device_buffer);
}

module_init(pa2_init);
module_exit(pa2_exit);
