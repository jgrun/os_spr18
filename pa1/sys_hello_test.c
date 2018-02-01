// Test syscall #333 (sys_helloworld)

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
  long res = syscall(333);
  printf("Syscall returned: %ld\n", res);
  return res;
}
