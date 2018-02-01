// Test syscall #334 (sys_csci3753_add)

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>

int main() {
  int number1 = 12;
  int number2 = 8;
  int result = 0;

  long res = syscall(334, number1, number2, &result);

  printf("Syscall returned: %ld\n", res);
  if(res == 0) {
    printf("%d + %d = %d\n", number1, number2, result);
  }
  return 0;
}
