#include <arpa/inet.h>
#include <stdio.h>
#define MAXLEN 16

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("need one param!\n");
    return 1;
  }
  uint32_t h;
  char r[MAXLEN];
  if (sscanf(argv[1], "%x", &h) != 1) {
    printf("bad format\n");
    return 2;
  }
  h = ntohl(h);
  const char *s;
  if (inet_ntop(AF_INET, &h, r, MAXLEN) == NULL) {
    printf("not valid addr\n");
    return 3;
  }
  printf("%s\n", r);
}
