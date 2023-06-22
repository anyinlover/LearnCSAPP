#include <arpa/inet.h>
#include <stdio.h>
#define MAXLEN 15

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("need one param!\n");
    return 1;
  }
  uint32_t d;
  if (inet_pton(AF_INET, argv[1], &d) != 1) {
    printf("bad format/n");
    return 2;
  }
  printf("0x%x\n", htonl(d));
  return 0;
}