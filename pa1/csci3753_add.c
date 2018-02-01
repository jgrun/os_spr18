#include <linux/kernel.h>
#include <linux/linkage.h>
//syscall 334
asmlinkage long sys_csci3753_add(int number1, int number2, int* result) {
  printk("Number 1: %d\n", number1);
  printk("Number 2: %d\n", number2);

  *result = number1 + number2;

  printk("Result: %d\n", *result);
  return 0;
}
