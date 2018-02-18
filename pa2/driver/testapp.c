#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

void printMenu(char * str);
void readDevice();
void writeDevice();
void seekDevice();

static int currentPosition;

void main() {
  char input[50];
  int loop = 1;
  currentPosition = 0;
  printMenu(input);
  while(loop == 1) {
    switch(input[0]) {
      case 'r':
        readDevice();
      break;
      case 'w':
        writeDevice();
      break;
      case 's':
        seekDevice();
      break;
      case 'e':
        loop = 0;
        printf("Exiting\n");
      break;
      default:
      break;
    }
    if(loop == 1) {
      printMenu(input);
    }
  }
}

void printMenu(char * str) {
  printf("\n[r] Read from device\n");
  printf("[w] Write to device\n");
  printf("[s] Seek into device\n");
  printf("[e] Exit\n");
  printf("Enter a command: ");
  scanf("%s", str);
}

void readDevice() {
  int length;
  printf("Enter the number of bytes you want read: ");
  scanf("%d", &length);
  char buffer[1024];
  int fd = open("/dev/pa2_device", O_RDONLY);
  int ret = read(fd, buffer, length);
  close(fd);
  printf("Data read from device:\n%s\n", buffer);
}

void writeDevice() {
  char str[1024];
  int i = 0;
  for(i = 0; i < 1024; i++) {
    str[i] = '\0';
  }
  printf("Enter data you want to write to the device: \n");
  scanf("%s", str);
  int length = 0;
  while(str[length] != '\0') {
    length++;
  }
  int fd = open("/dev/pa2_device", O_WRONLY);
  int ret = write(fd, str, length);
  close(fd);
}

void seekDevice() {
  int offset;
  int whence;
  printf("Enter an offset value: ");
  scanf("%d", &offset);
  printf("[0] SEEK_SET\n");
  printf("[1] SEEK_CUR\n");
  printf("[2] SEEK_END\n");
  printf("Enter a whence value: ");
  scanf("%d", &whence);
  int fd = open("/dev/pa2_device", O_RDWR);
  lseek(fd, offset, whence);
  close(fd);
}
